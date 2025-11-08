#include <ESP32-USB-Soft-Host.h>
#include <freertos/queue.h>

#include "USB.h"
#include "USBHIDKeyboard.h"

#include "amiga.h"
#include "pc.h"
#include "test_keys.h"
#include "key_map.h"

#undef DEBUG_USB_DATA

#define KEYBOARD_REPORT_SIZE 8
#define KEYBOARD_REPORT_QUEUE_SIZE 20

// http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0176.html
#define AMIGA_ACK_LOW_TIMEOUT_MS 145 // Slightly more than 143ms.
#define AMIGA_ACK_HIGH_TIMEOUT_MS 100 // Aribitrary.

#define NEO_RED pixels.Color(20, 0, 0)
#define NEO_GREEN pixels.Color(0, 20, 0)
#define NEO_BLUE pixels.Color(0, 0, 20)
#define NEO_PURPLE pixels.Color(20, 0, 20)
#define NEO_YELLOW pixels.Color(20, 20, 0)
#define NEO_OFF pixels.Color(0, 0, 0)

// USB Host Part (handles detection and input from the physical keyboard)
#define DP_P0 A5  // USB Host Data+ Pin (must be an analog pin)
#define DM_P0 A4  // USB Host Data- Pin (must be an analog pin)
#define FORCE_TEMPLATED_NOPS

USBHIDKeyboard Keyboard;
QueueHandle_t xQueue;

const usb_pins_config_t USB_Pins_Config = {
  DP_P0, DM_P0,
  -1, -1,
  -1, -1,
  -1, -1
};

static const uint8_t KBDATA = T11;
static const uint8_t KBCLK = T10;
static const uint8_t DEBUG = T9;

static bool usb_dev_begun = false;
static bool syncd = false;
static bool capslock_on = false;
static bool ctrl_amiga_amiga = false;
static uint8_t keybd_dev_addr = 0xff;

// Keep track of which keys are pressed down.
static bool pc_down_keys[256];
const int pc_down_keys_len = sizeof(pc_down_keys) / sizeof(pc_down_keys[0]);

// USB HID supports up to 6 regular keys pressed simultaneously, plus 7 modifier keys we track.
static uint8_t usb_reported[] = { PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY,
                                  PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY };
const int usb_reported_len = sizeof(usb_reported) / sizeof(usb_reported[0]);
