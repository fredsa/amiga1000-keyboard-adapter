#include <ESP32-USB-Soft-Host.h>

#include "USB.h"
#include "USBHIDKeyboard.h"

#include "amiga.h"
#include "pc.h"
#include "test_keys.h"
#include "key_map.h"


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

bool syncd = false;

// USB HID supports up to 6 regular keys pressed simultaneously.
static uint8_t down_keys[] = { PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY };
int down_keys_len = sizeof(down_keys) / sizeof(down_keys[0]);

// USB HID supports up to 6 regular keys pressed simultaneously, plus 7 modifier keys we track.
static uint8_t usb_reported[] = { PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY,
                                  PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY, PC_KEYCODE_NOT_A_KEY };
int usb_reported_len = sizeof(usb_reported) / sizeof(usb_reported[0]);
