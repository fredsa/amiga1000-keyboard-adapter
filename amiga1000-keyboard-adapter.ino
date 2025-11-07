// https://github.com/tobozo/ESP32-USB-Soft-Host compatibility:
// - Compiles with `esp32` Espressif SYstems library version 2.0.17
// - Will not compile version 3.3.3

#if !defined CONFIG_IDF_TARGET_ESP32S3 && !defined CONFIG_IDF_TARGET_ESP32S2
#error "This sketch will only run on ESP32-S2 or S3"
#endif

#if ARDUINO_USB_MODE
#error "This sketch should be used when USB is in OTG mode"
#endif

// Amiga 1000 keyboard connector
// 1 GND         - BLACK
// 2 Vcc (+5V)   - RED
// 3 KBDATA      - GREEN
// 4 KBCLK       - YELLOW

#include <Adafruit_NeoPixel.h>
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

static const uint8_t KBDATA = T11;
static const uint8_t KBCLK = T10;
static const uint8_t DEBUG = T9;

// USB Host Part (handles detection and input from the physical keyboard)
#define DP_P0 A5  // USB Host Data+ Pin (must be an analog pin)
#define DM_P0 A4  // USB Host Data- Pin (must be an analog pin)
#define FORCE_TEMPLATED_NOPS
#include <ESP32-USB-Soft-Host.h>

// Device Part (handles HID device emulation)
#include "USB.h"
#include "USBHIDKeyboard.h"  // Keybo
USBHIDKeyboard Keyboard;

const usb_pins_config_t USB_Pins_Config = {
  DP_P0, DM_P0,
  -1, -1,
  -1, -1,
  -1, -1
};


bool syncd = false;

Adafruit_NeoPixel pixels(NEOPIXEL_NUM, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

void setup() {
  // Blink main LED.
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }

  // Configure neopixel.
  pixels.begin();
  neo_color(NEO_PURPLE);

  // Configure serial.
  Serial.begin();
  Serial.println("Settting up...");
  Serial.println();

  // Configure keyboard.
  init_keycode_map();

  // Configure digital pins.
  pinMode(KBCLK, OUTPUT);
  pinMode(KBDATA, OUTPUT);
  pinMode(DEBUG, OUTPUT);
  digitalWrite(KBCLK, HIGH);
  digitalWrite(KBDATA, HIGH);
  digitalWrite(DEBUG, HIGH);

  // Configure USB host.
  USH.setOnConfigDescCB(Default_USB_ConfigDescCB);
  USH.setOnIfaceDescCb(Default_USB_IfaceDescCb);
  USH.setOnHIDDevDescCb(Default_USB_HIDDevDescCb);
  USH.setOnEPDescCb(Default_USB_EPDescCb);

  USH.init(USB_Pins_Config, on_usb_keyboard_detect, on_usb_keyboard_data);

  // Setup complete.
  Serial.println("Setup complete.");
  Serial.flush();

  neo_color(NEO_YELLOW);
}

void loop() {
  Serial.println("Cmd:");
  Serial.setTimeout(100000);
  String cmd = Serial.readStringUntil('\n');
  if (cmd == "test") {
    do_amiga_test_kit_keyboard_test();
  } else if (cmd == "usb") {
    USH.setOnConfigDescCB(Default_USB_ConfigDescCB);
    USH.setOnIfaceDescCb(Default_USB_IfaceDescCb);
    USH.setOnHIDDevDescCb(Default_USB_HIDDevDescCb);
    USH.setOnEPDescCb(Default_USB_EPDescCb);
    USH.init(USB_Pins_Config, on_usb_keyboard_detect, on_usb_keyboard_data);
  } else {
    for (int i = 0; i < cmd.length(); i++) {
      sendChar(cmd[i]);
    }
    send_key_up_down(AMIGA_KEYCODE_RETURN);
  }
}

void sendChar(char c) {
  switch (c) {
    case 'A':
    case 'a':
      send_key_up_down(AMIGA_KEYCODE_A);
      break;
    case 'B':
    case 'b':
      send_key_up_down(AMIGA_KEYCODE_B);
      break;
    case 'C':
    case 'c':
      send_key_up_down(AMIGA_KEYCODE_C);
      break;
    case 'D':
    case 'd':
      send_key_up_down(AMIGA_KEYCODE_D);
      break;
    case 'E':
    case 'e':
      send_key_up_down(AMIGA_KEYCODE_E);
      break;
    case 'F':
    case 'f':
      send_key_up_down(AMIGA_KEYCODE_F);
      break;
    case 'G':
    case 'g':
      send_key_up_down(AMIGA_KEYCODE_G);
      break;
    case 'H':
    case 'h':
      send_key_up_down(AMIGA_KEYCODE_H);
      break;
    case 'I':
    case 'i':
      send_key_up_down(AMIGA_KEYCODE_I);
      break;
    case 'J':
    case 'j':
      send_key_up_down(AMIGA_KEYCODE_J);
      break;
    case 'K':
    case 'k':
      send_key_up_down(AMIGA_KEYCODE_K);
      break;
    case 'L':
    case 'l':
      send_key_up_down(AMIGA_KEYCODE_L);
      break;
    case 'M':
    case 'm':
      send_key_up_down(AMIGA_KEYCODE_M);
      break;
    case 'N':
    case 'n':
      send_key_up_down(AMIGA_KEYCODE_N);
      break;
    case 'O':
    case 'o':
      send_key_up_down(AMIGA_KEYCODE_O);
      break;
    case 'P':
    case 'p':
      send_key_up_down(AMIGA_KEYCODE_P);
      break;
    case 'Q':
    case 'q':
      send_key_up_down(AMIGA_KEYCODE_Q);
      break;
    case 'R':
    case 'r':
      send_key_up_down(AMIGA_KEYCODE_R);
      break;
    case 'S':
    case 's':
      send_key_up_down(AMIGA_KEYCODE_S);
      break;
    case 'T':
    case 't':
      send_key_up_down(AMIGA_KEYCODE_T);
      break;
    case 'U':
    case 'u':
      send_key_up_down(AMIGA_KEYCODE_U);
      break;
    case 'V':
    case 'v':
      send_key_up_down(AMIGA_KEYCODE_V);
      break;
    case 'W':
    case 'w':
      send_key_up_down(AMIGA_KEYCODE_W);
      break;
    case 'X':
    case 'x':
      send_key_up_down(AMIGA_KEYCODE_X);
      break;
    case 'Y':
    case 'y':
      send_key_up_down(AMIGA_KEYCODE_Y);
      break;
    case 'Z':
    case 'z':
      send_key_up_down(AMIGA_KEYCODE_Z);
      break;
    case ' ':
      send_key_up_down(AMIGA_KEYCODE_SPACEBAR);
      break;
    default:
      Serial.printf("Unknown char: %c\n", c);
      break;
  }
}

void neo_color(uint32_t color) {
  pixels.setPixelColor(0, color);
  pixels.show();
}

void sync_up() {
  if (syncd) {
    return;
  }

  print_uptime();
  Serial.println("\nSyncing...");
  Serial.flush();

  unsigned long count = 0;
  while (true) {
    count++;
    pinMode(KBDATA, OUTPUT);
    send_bit(1);

    wait_for_ack();
    if (syncd) {
      Serial.printf("ACK received after %lu attempts.\n", count);
      return;
    }
    if (count % 1000 == 0) {
      Serial.printf("Still NO ack after %lu attempts.\n", count);
    }

    // Slight throttle.
    delay(10);
  }
}

void wait_for_ack() {
  digitalWrite(DEBUG, LOW);
  digitalWrite(DEBUG, HIGH);

  pinMode(KBDATA, INPUT);
  int lowafter = wait_for_state(KBDATA, LOW, 500);
  if (lowafter != -1) {
    int highafter = wait_for_state(KBDATA, HIGH, 500);
    if (highafter != -1) {
      // Serial.printf("Got sync LOW after %i us, HIGH after %i us.\n", lowafter, highafter);
      set_syncd(true);
      return;
    }
  }
  set_syncd(false);
  return;
}

int wait_for_state(uint8_t pin, uint8_t desired, int timeout_us) {
  unsigned long start = micros();
  unsigned long until = micros() + timeout_us;
  while (true) {
    unsigned long now = micros();
    if (now > until) {
      return -1;
    }
    uint8_t state = digitalRead(pin);
    if (state == desired) {
      return now - start;
    }
  }
}

void print_uptime() {
  unsigned long currentMillis = millis();

  unsigned long seconds = currentMillis / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;

  Serial.printf("Uptime: %02lu:%02lu:%02lu", hours, minutes, seconds);
}

// Amiga Test Kit 1.21
// https://github.com/keirf/amiga-stuff
void do_amiga_test_kit_keyboard_test() {
  // Exit back to main menu.
  send_keycode(AMIGA_KEYCODE_CTRL);
  send_keycode(AMIGA_KEYCODE_LEFT_ALT);
  delay(500);

  // Enter Amiga Test Kit 1.21 keyboard test.
  send_key_up_down(AMIGA_KEYCODE_F2);
  delay(1000);

  // Send test keys.
  size_t array_size = sizeof(test_keys) / sizeof(test_keys[0]);
  for (int i = 0; i < array_size; i++) {
    if (!syncd) {
      // Abort.
      return;
    }
    send_key_up_down(test_keys[i]);
  }

  delay(3000);

  // Exit back to main menu.
  send_keycode(AMIGA_KEYCODE_CTRL);
  send_keycode(AMIGA_KEYCODE_LEFT_ALT);
  delay(500);
}

// http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0173.html
void send_bit(uint8_t bit) {
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(KBDATA, OUTPUT);
  digitalWrite(KBDATA, !bit);  // Active low.
  delayMicroseconds(20);
  digitalWrite(KBCLK, LOW);
  delayMicroseconds(20);
  digitalWrite(KBCLK, HIGH);
  delayMicroseconds(20);
  digitalWrite(LED_BUILTIN, LOW);
}

void send_key_up_down(uint8_t keycode) {
  send_keycode(keycode);
  delay(300);
  send_keycode(keycode | 0x80);
}

void send_keycode(uint8_t keycode) {
  sync_up();

  neo_color(NEO_BLUE);
  for (int i = 6; i >= 0; i--) {
    uint8_t bit = (keycode >> i) & 0x01;
    send_bit(bit);
  }

  uint8_t bit = keycode >> 7;
  send_bit(bit);

  digitalWrite(KBDATA, HIGH);

  wait_for_ack();
  if (syncd) {
    Serial.printf("Sent 0x%x, received ACK.\n", keycode);
  } else {
    Serial.printf("Sent 0x%x, NO ACK!\n", keycode);
  }
}

void set_syncd(bool state) {
  syncd = state;
  if (syncd) {
    neo_color(NEO_OFF);
  } else {
    neo_color(NEO_RED);
  }
}

static void on_usb_keyboard_detect(uint8_t usbNum, void* dev) {
  sDevDesc* device = (sDevDesc*)dev;
  Serial.printf("New device detected on USB#%d\n", usbNum);
  Serial.printf("desc.bcdUSB             = 0x%04x\n", device->bcdUSB);
  Serial.printf("desc.bDeviceClass       = 0x%02x\n", device->bDeviceClass);
  Serial.printf("desc.bDeviceSubClass    = 0x%02x\n", device->bDeviceSubClass);
  Serial.printf("desc.bDeviceProtocol    = 0x%02x\n", device->bDeviceProtocol);
  Serial.printf("desc.bMaxPacketSize0    = 0x%02x\n", device->bMaxPacketSize0);
  Serial.printf("desc.idVendor           = 0x%04x\n", device->idVendor);
  Serial.printf("desc.idProduct          = 0x%04x\n", device->idProduct);
  Serial.printf("desc.bcdDevice          = 0x%04x\n", device->bcdDevice);
  Serial.printf("desc.iManufacturer      = 0x%02x\n", device->iManufacturer);
  Serial.printf("desc.iProduct           = 0x%02x\n", device->iProduct);
  Serial.printf("desc.iSerialNumber      = 0x%02x\n", device->iSerialNumber);
  Serial.printf("desc.bNumConfigurations = 0x%02x\n", device->bNumConfigurations);
  // if( device->iProduct == mySupportedIdProduct && device->iManufacturer == mySupportedManufacturer ) {
  //   myListenUSBPort = usbNum;
  // }

  static bool usb_dev_begun = false;

  if (!usb_dev_begun) {
    Serial.printf("Starting USB");
    Keyboard.begin();
    USB.begin();
  }
}

static uint8_t downkeys[] = { 0, 0, 0, 0, 0 };

static void on_usb_keyboard_data(uint8_t usbNum, uint8_t byte_depth, uint8_t* data, uint8_t data_len) {
  // if( myListenUSBPort != usbNum ) return;
  Serial.printf("in: ");
  for (int k = 0; k < data_len; k++) {
    Serial.printf("0x%02x ", data[k]);
  }
  Serial.printf("\n");
  bool left_ctrl = data[0] && 1 << 0;
  bool left_shift = data[0] && 1 << 1;
  bool left_alt = data[0] && 1 << 2;
  bool left_super = data[0] && 1 << 3;
  bool right_ctrl = data[0] && 1 << 4;
  bool right_shift = data[0] && 1 << 5;
  bool right_alt = data[0] && 1 << 6;
  bool right_super = data[0] && 1 << 7;

  handle_key(data[2]);
  // Keyboard.sendReport((KeyReport*)data);
}

void handle_key(uint8_t pccode) {
  if (!pccode) {
    return;
  }
  uint8_t amigacode = keycode_map[pccode];
  if (amigacode == AMIGA_KEYCODE_NOT_A_KEY) {
    return;
  }
  send_key_up_down(amigacode);
}
