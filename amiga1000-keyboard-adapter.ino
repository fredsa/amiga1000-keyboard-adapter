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


// http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node017A.html
static const uint8_t AMIGA_KEYCODE_BACKTICK_TILDE = 0x00;
static const uint8_t AMIGA_KEYCODE_1_EXCLAMATION = 0x01;
static const uint8_t AMIGA_KEYCODE_2_AT_SIGN = 0x02;
static const uint8_t AMIGA_KEYCODE_3_POUND_SIGN = 0x03;
static const uint8_t AMIGA_KEYCODE_4_DOLLAR_SIGN = 0x04;
static const uint8_t AMIGA_KEYCODE_5_PERCENT = 0x05;
static const uint8_t AMIGA_KEYCODE_6_HAT = 0x06;
static const uint8_t AMIGA_KEYCODE_7_AMPERSAND = 0x07;
static const uint8_t AMIGA_KEYCODE_8_ASTERISK = 0x08;
static const uint8_t AMIGA_KEYCODE_9_OPEN_PARENTHESIS = 0x09;
static const uint8_t AMIGA_KEYCODE_0_CLOSE_PARENTHESIS = 0x0A;
static const uint8_t AMIGA_KEYCODE_MINUS_UNDERSCORE = 0x0B;
static const uint8_t AMIGA_KEYCODE_EQUALS_PLUS = 0x0C;
static const uint8_t AMIGA_KEYCODE_BACKSLASH_PIPE = 0x0D;
static const uint8_t AMIGA_KEYCODE_NUMERIC_0 = 0x0F;
static const uint8_t AMIGA_KEYCODE_Q = 0x10;
static const uint8_t AMIGA_KEYCODE_W = 0x11;
static const uint8_t AMIGA_KEYCODE_E = 0x12;
static const uint8_t AMIGA_KEYCODE_R = 0x13;
static const uint8_t AMIGA_KEYCODE_T = 0x14;
static const uint8_t AMIGA_KEYCODE_Y = 0x15;
static const uint8_t AMIGA_KEYCODE_U = 0x16;
static const uint8_t AMIGA_KEYCODE_I = 0x17;
static const uint8_t AMIGA_KEYCODE_O = 0x18;
static const uint8_t AMIGA_KEYCODE_P = 0x19;
static const uint8_t AMIGA_KEYCODE_LEFT_SQUARE_SQUIGLEY_BRACKET = 0x1A;
static const uint8_t AMIGA_KEYCODE_RIGHT_SQUARE_SQUIGLEY_BRACKET = 0x1B;
// static const uint8_t AMIGA_KEYCODE_UNUSED_1C = 0x1C;
static const uint8_t AMIGA_KEYCODE_NUMERIC_1 = 0x1D;
static const uint8_t AMIGA_KEYCODE_NUMERIC_2 = 0x1E;
static const uint8_t AMIGA_KEYCODE_NUMERIC_3 = 0x1F;
static const uint8_t AMIGA_KEYCODE_A = 0x20;
static const uint8_t AMIGA_KEYCODE_S = 0x21;
static const uint8_t AMIGA_KEYCODE_D = 0x22;
static const uint8_t AMIGA_KEYCODE_F = 0x23;
static const uint8_t AMIGA_KEYCODE_G = 0x24;
static const uint8_t AMIGA_KEYCODE_H = 0x25;
static const uint8_t AMIGA_KEYCODE_J = 0x26;
static const uint8_t AMIGA_KEYCODE_K = 0x27;
static const uint8_t AMIGA_KEYCODE_L = 0x28;
static const uint8_t AMIGA_KEYCODE_SEMICOLON_COLON = 0x29;
static const uint8_t AMIGA_KEYCODE_SINGLE_DOUBLE_QUOTE = 0x2A;
static const uint8_t AMIGA_KEYCODE_I18N_KEY_NEXT_TO_RETURN = 0x2B;
// static const uint8_t AMIGA_KEYCODE_UNUSED_2C = 0x2C;
static const uint8_t AMIGA_KEYCODE_NUMERIC_4 = 0x2D;
static const uint8_t AMIGA_KEYCODE_NUMERIC_5 = 0x2E;
static const uint8_t AMIGA_KEYCODE_NUMERIC_6 = 0x2F;
static const uint8_t AMIGA_KEYCODE_I18N_KEY_NEXT_TO_LEFT_SHIFT = 0x30;
static const uint8_t AMIGA_KEYCODE_Z = 0x31;
static const uint8_t AMIGA_KEYCODE_X = 0x32;
static const uint8_t AMIGA_KEYCODE_C = 0x33;
static const uint8_t AMIGA_KEYCODE_V = 0x34;
static const uint8_t AMIGA_KEYCODE_B = 0x35;
static const uint8_t AMIGA_KEYCODE_N = 0x36;
static const uint8_t AMIGA_KEYCODE_M = 0x37;
static const uint8_t AMIGA_KEYCODE_COMMA_LESS_THAN = 0x38;
static const uint8_t AMIGA_KEYCODE_PERIOD_GREATER_THAN = 0x39;
static const uint8_t AMIGA_KEYCODE_SLASH_QUESTION_MARK = 0x3A;
// static const uint8_t AMIGA_KEYCODE_UNUSED_3B = 0x3B;
static const uint8_t AMIGA_KEYCODE_NUMERIC_PERIOD = 0x3C;
static const uint8_t AMIGA_KEYCODE_NUMERIC_7 = 0x3D;
static const uint8_t AMIGA_KEYCODE_NUMERIC_8 = 0x3E;
static const uint8_t AMIGA_KEYCODE_NUMERIC_9 = 0x3F;
static const uint8_t AMIGA_KEYCODE_SPACEBAR = 0x40;
static const uint8_t AMIGA_KEYCODE_BACKSPACE = 0x41;
static const uint8_t AMIGA_KEYCODE_TAB = 0x42;
static const uint8_t AMIGA_KEYCODE_NUMERIC_ENTER = 0x43;
static const uint8_t AMIGA_KEYCODE_RETURN = 0x44;
static const uint8_t AMIGA_KEYCODE_ESC = 0x45;
static const uint8_t AMIGA_KEYCODE_DEL = 0x46;
// static const uint8_t AMIGA_KEYCODE_UNUSED_47 = 0x47;
// static const uint8_t AMIGA_KEYCODE_UNUSED_48 = 0x48;
// static const uint8_t AMIGA_KEYCODE_UNUSED_49 = 0x49;
static const uint8_t AMIGA_KEYCODE_NUMERIC_MINUS = 0x4A;
static const uint8_t AMIGA_KEYCODE_CURSOR_UP = 0x4C;
static const uint8_t AMIGA_KEYCODE_CURSOR_DOWN = 0x4D;
static const uint8_t AMIGA_KEYCODE_CURSOR_RIGHT = 0x4E;
static const uint8_t AMIGA_KEYCODE_CURSOR_LEFT = 0x4F;
static const uint8_t AMIGA_KEYCODE_F1 = 0x50;
static const uint8_t AMIGA_KEYCODE_F2 = 0x51;
static const uint8_t AMIGA_KEYCODE_F3 = 0x52;
static const uint8_t AMIGA_KEYCODE_F4 = 0x53;
static const uint8_t AMIGA_KEYCODE_F5 = 0x54;
static const uint8_t AMIGA_KEYCODE_F6 = 0x55;
static const uint8_t AMIGA_KEYCODE_F7 = 0x56;
static const uint8_t AMIGA_KEYCODE_F8 = 0x57;
static const uint8_t AMIGA_KEYCODE_F9 = 0x58;
static const uint8_t AMIGA_KEYCODE_F10 = 0x59;
static const uint8_t AMIGA_KEYCODE_NUMERIC_OPEN_PARENTHESIS = 0x5A;
static const uint8_t AMIGA_KEYCODE_NUMERIC_CLOSE_PARENTHESIS = 0x5B;
static const uint8_t AMIGA_KEYCODE_NUMERIC_SLASH = 0x5C;
static const uint8_t AMIGA_KEYCODE_NUMERIC_ASTERISK = 0x5D;
static const uint8_t AMIGA_KEYCODE_NUMERIC_PLUS = 0x5E;
static const uint8_t AMIGA_KEYCODE_HELP = 0x5F;
static const uint8_t AMIGA_KEYCODE_LEFT_SHIFT = 0x60;
static const uint8_t AMIGA_KEYCODE_RIGHT_SHIFT = 0x61;
static const uint8_t AMIGA_KEYCODE_CAPS_LOCK = 0x62;
static const uint8_t AMIGA_KEYCODE_CTRL = 0x63;
static const uint8_t AMIGA_KEYCODE_LEFT_ALT = 0x64;
static const uint8_t AMIGA_KEYCODE_RIGHT_ALT = 0x65;
static const uint8_t AMIGA_KEYCODE_LEFT_AMIGA = 0x66;
static const uint8_t AMIGA_KEYCODE_RIGHT_AMIGA = 0x67;

static const uint8_t PC_KEYCODE_A = 0x04;
static const uint8_t PC_KEYCODE_B = 0x05;
static const uint8_t PC_KEYCODE_C = 0x06;
static const uint8_t PC_KEYCODE_D = 0x07;
static const uint8_t PC_KEYCODE_E = 0x08;
static const uint8_t PC_KEYCODE_F = 0x09;
static const uint8_t PC_KEYCODE_G = 0x0a;
static const uint8_t PC_KEYCODE_H = 0x0b;
static const uint8_t PC_KEYCODE_I = 0x0c;
static const uint8_t PC_KEYCODE_J = 0x0d;
static const uint8_t PC_KEYCODE_K = 0x0e;
static const uint8_t PC_KEYCODE_L = 0x0f;
static const uint8_t PC_KEYCODE_M = 0x10;
static const uint8_t PC_KEYCODE_N = 0x11;
static const uint8_t PC_KEYCODE_O = 0x12;
static const uint8_t PC_KEYCODE_P = 0x13;
static const uint8_t PC_KEYCODE_Q = 0x14;
static const uint8_t PC_KEYCODE_R = 0x15;
static const uint8_t PC_KEYCODE_S = 0x16;
static const uint8_t PC_KEYCODE_T = 0x17;
static const uint8_t PC_KEYCODE_U = 0x18;
static const uint8_t PC_KEYCODE_V = 0x19;
static const uint8_t PC_KEYCODE_W = 0x1a;
static const uint8_t PC_KEYCODE_X = 0x1b;
static const uint8_t PC_KEYCODE_Y = 0x1c;
static const uint8_t PC_KEYCODE_Z = 0x1d;


static const uint8_t PC_KEYCODE_1_EXCLAMATION = 0x1e;
static const uint8_t PC_KEYCODE_2_AT_SIGN = 0x1f;
static const uint8_t PC_KEYCODE_3_POUND_SIGN = 0x20;
static const uint8_t PC_KEYCODE_4_DOLLAR_SIGN = 0x21;
static const uint8_t PC_KEYCODE_5_PERCENT = 0x22;
static const uint8_t PC_KEYCODE_6_HAT = 0x23;
static const uint8_t PC_KEYCODE_7_AMPERSAND = 0x24;
static const uint8_t PC_KEYCODE_8_ASTERISK = 0x25;
static const uint8_t PC_KEYCODE_9_OPEN_PARENTHESIS = 0x26;
static const uint8_t PC_KEYCODE_0_CLOSE_PARENTHESIS = 0x27;
static const uint8_t PC_KEYCODE_RETURN = 0x28;
static const uint8_t PC_KEYCODE_ESC = 0x29;
static const uint8_t PC_KEYCODE_BACKSPACE = 0x2a;
static const uint8_t PC_KEYCODE_TAB = 0x2b;
static const uint8_t PC_KEYCODE_SPACEBAR = 0x2c;
static const uint8_t PC_KEYCODE_MINUS_UNDERSCORE = 0x2d;
static const uint8_t PC_KEYCODE_EQUALS_PLUS = 0x2e;
static const uint8_t PC_KEYCODE_LEFT_SQUARE_SQUIGLEY_BRACKET = 0x2f;
static const uint8_t PC_KEYCODE_RIGHT_SQUARE_SQUIGLEY_BRACKET = 0x30;
static const uint8_t PC_KEYCODE_BACKSLASH_PIPE = 0x31;

static const uint8_t PC_KEYCODE_SEMICOLON_COLON = 0x33;
static const uint8_t PC_KEYCODE_SINGLE_DOUBLE_QUOTE = 0x34;
static const uint8_t PC_KEYCODE_BACKTICK_TILDE = 0x35;
static const uint8_t PC_KEYCODE_COMMA_LESS_THAN = 0x36;
static const uint8_t PC_KEYCODE_PERIOD_GREATER_THAN = 0x37;
static const uint8_t PC_KEYCODE_SLASH_QUESTION_MARK = 0x38;
static const uint8_t PC_KEYCODE_CAPS_LOCK = 0x39;

static const uint8_t PC_KEYCODE_F1 = 0x3a;
static const uint8_t PC_KEYCODE_F2 = 0x3b;
static const uint8_t PC_KEYCODE_F3 = 0x3c;
static const uint8_t PC_KEYCODE_F4 = 0x3d;
static const uint8_t PC_KEYCODE_F5 = 0x3e;
static const uint8_t PC_KEYCODE_F6 = 0x3f;
static const uint8_t PC_KEYCODE_F7 = 0x40;
static const uint8_t PC_KEYCODE_F8 = 0x41;
static const uint8_t PC_KEYCODE_F9 = 0x42;
static const uint8_t PC_KEYCODE_F10 = 0x43;
static const uint8_t PC_KEYCODE_F11 = 0x44;
static const uint8_t PC_KEYCODE_F12 = 0x45;

static const uint8_t PC_KEYCODE_PRINT_SCREEN = 0x46;
static const uint8_t PC_KEYCODE_SCROLL_LOCK = 0x47;
static const uint8_t PC_KEYCODE_PAUSE = 0x48;
static const uint8_t PC_KEYCODE_INSERT = 0x49;
static const uint8_t PC_KEYCODE_HOME = 0x4a;
static const uint8_t PC_KEYCODE_PAGE_UP = 0x4b;
static const uint8_t PC_KEYCODE_DEL = 0x4c;
static const uint8_t PC_KEYCODE_END = 0x4d;
static const uint8_t PC_KEYCODE_PAGE_DOWN = 0x4e;
static const uint8_t PC_KEYCODE_CURSOR_RIGHT = 0x4f;
static const uint8_t PC_KEYCODE_CURSOR_LEFT = 0x50;
static const uint8_t PC_KEYCODE_CURSOR_DOWN = 0x51;
static const uint8_t PC_KEYCODE_CURSOR_UP = 0x52;
static const uint8_t PC_KEYCODE_NUMERIC_NUM_LOCK_CLEAR = 0x53;
static const uint8_t PC_KEYCODE_NUMERIC_SLASH = 0x54;
static const uint8_t PC_KEYCODE_NUMERIC_ASTERISK = 0x55;
static const uint8_t PC_KEYCODE_NUMERIC_MINUS = 0x56;
static const uint8_t PC_KEYCODE_NUMERIC_PLUS = 0x57;
static const uint8_t PC_KEYCODE_NUMERIC_ENTER = 0x58;
static const uint8_t PC_KEYCODE_NUMERIC_1_END = 0x59;
static const uint8_t PC_KEYCODE_NUMERIC_2_CUROR_DOWN = 0x5a;
static const uint8_t PC_KEYCODE_NUMERIC_3_PAGE_DOWN = 0x5b;
static const uint8_t PC_KEYCODE_NUMERIC_4_CURSOR_LEFT = 0x5c;
static const uint8_t PC_KEYCODE_NUMERIC_5 = 0x5d;
static const uint8_t PC_KEYCODE_NUMERIC_6_CURSOR_RIGHT = 0x5e;
static const uint8_t PC_KEYCODE_NUMERIC_7_HOME = 0x5f;
static const uint8_t PC_KEYCODE_NUMERIC_8_CURSOR_UP = 0x60;
static const uint8_t PC_KEYCODE_NUMERIC_9_PAGE_UP = 0x61;
static const uint8_t PC_KEYCODE_NUMERIC_0_INSERT = 0x62;
static const uint8_t PC_KEYCODE_NUMERIC_PERIOD = 0x63;

static const uint8_t test_keys[] = {
  AMIGA_KEYCODE_ESC,
  AMIGA_KEYCODE_F1,
  AMIGA_KEYCODE_F2,
  AMIGA_KEYCODE_F3,
  AMIGA_KEYCODE_F4,
  AMIGA_KEYCODE_F5,
  AMIGA_KEYCODE_F6,
  AMIGA_KEYCODE_F7,
  AMIGA_KEYCODE_F8,
  AMIGA_KEYCODE_F9,
  AMIGA_KEYCODE_F10,
  AMIGA_KEYCODE_BACKTICK_TILDE,
  AMIGA_KEYCODE_1_EXCLAMATION,
  AMIGA_KEYCODE_2_AT_SIGN,
  AMIGA_KEYCODE_3_POUND_SIGN,
  AMIGA_KEYCODE_4_DOLLAR_SIGN,
  AMIGA_KEYCODE_5_PERCENT,
  AMIGA_KEYCODE_6_HAT,
  AMIGA_KEYCODE_7_AMPERSAND,
  AMIGA_KEYCODE_8_ASTERISK,
  AMIGA_KEYCODE_9_OPEN_PARENTHESIS,
  AMIGA_KEYCODE_0_CLOSE_PARENTHESIS,
  AMIGA_KEYCODE_MINUS_UNDERSCORE,
  AMIGA_KEYCODE_EQUALS_PLUS,
  AMIGA_KEYCODE_BACKSLASH_PIPE,
  AMIGA_KEYCODE_BACKSPACE,

  AMIGA_KEYCODE_TAB,
  AMIGA_KEYCODE_Q,
  AMIGA_KEYCODE_W,
  AMIGA_KEYCODE_E,
  AMIGA_KEYCODE_R,
  AMIGA_KEYCODE_T,
  AMIGA_KEYCODE_Y,
  AMIGA_KEYCODE_U,
  AMIGA_KEYCODE_I,
  AMIGA_KEYCODE_O,
  AMIGA_KEYCODE_P,
  AMIGA_KEYCODE_LEFT_SQUARE_SQUIGLEY_BRACKET,
  AMIGA_KEYCODE_RIGHT_SQUARE_SQUIGLEY_BRACKET,
  AMIGA_KEYCODE_RETURN,

  AMIGA_KEYCODE_CTRL,
  AMIGA_KEYCODE_CAPS_LOCK,
  AMIGA_KEYCODE_A,
  AMIGA_KEYCODE_S,
  AMIGA_KEYCODE_D,
  AMIGA_KEYCODE_F,
  AMIGA_KEYCODE_G,
  AMIGA_KEYCODE_H,
  AMIGA_KEYCODE_J,
  AMIGA_KEYCODE_K,
  AMIGA_KEYCODE_L,
  AMIGA_KEYCODE_SEMICOLON_COLON,
  AMIGA_KEYCODE_SINGLE_DOUBLE_QUOTE,
  AMIGA_KEYCODE_I18N_KEY_NEXT_TO_RETURN,

  AMIGA_KEYCODE_LEFT_SHIFT,
  AMIGA_KEYCODE_I18N_KEY_NEXT_TO_LEFT_SHIFT,
  AMIGA_KEYCODE_Z,
  AMIGA_KEYCODE_X,
  AMIGA_KEYCODE_C,
  AMIGA_KEYCODE_V,
  AMIGA_KEYCODE_B,
  AMIGA_KEYCODE_N,
  AMIGA_KEYCODE_M,
  AMIGA_KEYCODE_COMMA_LESS_THAN,
  AMIGA_KEYCODE_PERIOD_GREATER_THAN,
  AMIGA_KEYCODE_SLASH_QUESTION_MARK,
  AMIGA_KEYCODE_RIGHT_SHIFT,

  AMIGA_KEYCODE_LEFT_ALT,
  AMIGA_KEYCODE_LEFT_AMIGA,
  AMIGA_KEYCODE_SPACEBAR,
  AMIGA_KEYCODE_RIGHT_AMIGA,
  AMIGA_KEYCODE_RIGHT_ALT,

  AMIGA_KEYCODE_DEL,
  AMIGA_KEYCODE_HELP,

  AMIGA_KEYCODE_CURSOR_UP,
  AMIGA_KEYCODE_CURSOR_LEFT,
  AMIGA_KEYCODE_CURSOR_DOWN,
  AMIGA_KEYCODE_CURSOR_RIGHT,

  AMIGA_KEYCODE_NUMERIC_OPEN_PARENTHESIS,
  AMIGA_KEYCODE_NUMERIC_CLOSE_PARENTHESIS,
  AMIGA_KEYCODE_NUMERIC_SLASH,
  AMIGA_KEYCODE_NUMERIC_ASTERISK,

  AMIGA_KEYCODE_NUMERIC_7,
  AMIGA_KEYCODE_NUMERIC_8,
  AMIGA_KEYCODE_NUMERIC_9,
  AMIGA_KEYCODE_NUMERIC_MINUS,

  AMIGA_KEYCODE_NUMERIC_4,
  AMIGA_KEYCODE_NUMERIC_5,
  AMIGA_KEYCODE_NUMERIC_6,
  AMIGA_KEYCODE_NUMERIC_PLUS,

  AMIGA_KEYCODE_NUMERIC_1,
  AMIGA_KEYCODE_NUMERIC_2,
  AMIGA_KEYCODE_NUMERIC_3,
  AMIGA_KEYCODE_NUMERIC_ENTER,

  AMIGA_KEYCODE_NUMERIC_0,
  AMIGA_KEYCODE_NUMERIC_PERIOD,
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
  // USH.setOnConfigDescCB(Default_USB_ConfigDescCB);
  // USH.setOnIfaceDescCb(Default_USB_IfaceDescCb);
  // USH.setOnHIDDevDescCb(Default_USB_HIDDevDescCb);
  // USH.setOnEPDescCb(Default_USB_EPDescCb);

  // USH.init(USB_Pins_Config, onKeyboardDetect, onKeyboardData);

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
    USH.init(USB_Pins_Config, onKeyboardDetect, onKeyboardData);
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

static void onKeyboardDetect(uint8_t usbNum, void* dev) {
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

static void onKeyboardData(uint8_t usbNum, uint8_t byte_depth, uint8_t* data, uint8_t data_len) {
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

  handleKey(data[2]);
  // Keyboard.sendReport((KeyReport*)data);
}

void handleKey(uint8_t code) {
  if (!code) { return; }
  if (code >= 0x04 /* A */ && code <= 0x1d /* Z */) {
  }
}

static uint8_t keycode_map[256];

void init_keycode_map() {
  keycode_map[PC_KEYCODE_ESC] = AMIGA_KEYCODE_ESC;
  keycode_map[PC_KEYCODE_F1] = AMIGA_KEYCODE_F1;
  keycode_map[PC_KEYCODE_F2] = AMIGA_KEYCODE_F2;
  keycode_map[PC_KEYCODE_F3] = AMIGA_KEYCODE_F3;
  keycode_map[PC_KEYCODE_F4] = AMIGA_KEYCODE_F4;
  keycode_map[PC_KEYCODE_F5] = AMIGA_KEYCODE_F5;
  keycode_map[PC_KEYCODE_F6] = AMIGA_KEYCODE_F6;
  keycode_map[PC_KEYCODE_F7] = AMIGA_KEYCODE_F7;
  keycode_map[PC_KEYCODE_F8] = AMIGA_KEYCODE_F8;
  keycode_map[PC_KEYCODE_F9] = AMIGA_KEYCODE_F9;
  keycode_map[PC_KEYCODE_F10] = AMIGA_KEYCODE_F10;
  keycode_map[PC_KEYCODE_BACKTICK_TILDE] = AMIGA_KEYCODE_BACKTICK_TILDE;
  keycode_map[PC_KEYCODE_1_EXCLAMATION] = AMIGA_KEYCODE_1_EXCLAMATION;
  keycode_map[PC_KEYCODE_2_AT_SIGN] = AMIGA_KEYCODE_2_AT_SIGN;
  keycode_map[PC_KEYCODE_3_POUND_SIGN] = AMIGA_KEYCODE_3_POUND_SIGN;
  keycode_map[PC_KEYCODE_4_DOLLAR_SIGN] = AMIGA_KEYCODE_4_DOLLAR_SIGN;
  keycode_map[PC_KEYCODE_5_PERCENT] = AMIGA_KEYCODE_5_PERCENT;
  keycode_map[PC_KEYCODE_6_HAT] = AMIGA_KEYCODE_6_HAT;
  keycode_map[PC_KEYCODE_7_AMPERSAND] = AMIGA_KEYCODE_7_AMPERSAND;
  keycode_map[PC_KEYCODE_8_ASTERISK] = AMIGA_KEYCODE_8_ASTERISK;
  keycode_map[PC_KEYCODE_9_OPEN_PARENTHESIS] = AMIGA_KEYCODE_9_OPEN_PARENTHESIS;
  keycode_map[PC_KEYCODE_0_CLOSE_PARENTHESIS] = AMIGA_KEYCODE_0_CLOSE_PARENTHESIS;
  keycode_map[PC_KEYCODE_MINUS_UNDERSCORE] = AMIGA_KEYCODE_MINUS_UNDERSCORE;
  keycode_map[PC_KEYCODE_EQUALS_PLUS] = AMIGA_KEYCODE_EQUALS_PLUS;
  keycode_map[PC_KEYCODE_BACKSLASH_PIPE] = AMIGA_KEYCODE_BACKSLASH_PIPE;
  keycode_map[PC_KEYCODE_BACKSPACE] = AMIGA_KEYCODE_BACKSPACE;

  keycode_map[PC_KEYCODE_TAB] = AMIGA_KEYCODE_TAB;
  keycode_map[PC_KEYCODE_Q] = AMIGA_KEYCODE_Q;
  keycode_map[PC_KEYCODE_W] = AMIGA_KEYCODE_W;
  keycode_map[PC_KEYCODE_E] = AMIGA_KEYCODE_E;
  keycode_map[PC_KEYCODE_R] = AMIGA_KEYCODE_R;
  keycode_map[PC_KEYCODE_T] = AMIGA_KEYCODE_T;
  keycode_map[PC_KEYCODE_Y] = AMIGA_KEYCODE_Y;
  keycode_map[PC_KEYCODE_U] = AMIGA_KEYCODE_U;
  keycode_map[PC_KEYCODE_I] = AMIGA_KEYCODE_I;
  keycode_map[PC_KEYCODE_O] = AMIGA_KEYCODE_O;
  keycode_map[PC_KEYCODE_P] = AMIGA_KEYCODE_P;
  keycode_map[PC_KEYCODE_LEFT_SQUARE_SQUIGLEY_BRACKET] = AMIGA_KEYCODE_LEFT_SQUARE_SQUIGLEY_BRACKET;
  keycode_map[PC_KEYCODE_RIGHT_SQUARE_SQUIGLEY_BRACKET] = AMIGA_KEYCODE_RIGHT_SQUARE_SQUIGLEY_BRACKET;
  keycode_map[PC_KEYCODE_RETURN] = AMIGA_KEYCODE_RETURN;

  // keycode_map[PC_KEYCODE_LEFT_CTRL] = AMIGA_KEYCODE_CTRL;
  // keycode_map[PC_KEYCODE_RIGHT_CTRL] = AMIGA_KEYCODE_CTRL;
  keycode_map[PC_KEYCODE_CAPS_LOCK] = AMIGA_KEYCODE_CAPS_LOCK;
  keycode_map[PC_KEYCODE_A] = AMIGA_KEYCODE_A;
  keycode_map[PC_KEYCODE_S] = AMIGA_KEYCODE_S;
  keycode_map[PC_KEYCODE_D] = AMIGA_KEYCODE_D;
  keycode_map[PC_KEYCODE_F] = AMIGA_KEYCODE_F;
  keycode_map[PC_KEYCODE_G] = AMIGA_KEYCODE_G;
  keycode_map[PC_KEYCODE_H] = AMIGA_KEYCODE_H;
  keycode_map[PC_KEYCODE_J] = AMIGA_KEYCODE_J;
  keycode_map[PC_KEYCODE_K] = AMIGA_KEYCODE_K;
  keycode_map[PC_KEYCODE_L] = AMIGA_KEYCODE_L;
  keycode_map[PC_KEYCODE_SEMICOLON_COLON] = AMIGA_KEYCODE_SEMICOLON_COLON;
  keycode_map[PC_KEYCODE_SINGLE_DOUBLE_QUOTE] = AMIGA_KEYCODE_SINGLE_DOUBLE_QUOTE;
  // keycode_map[PC_KEYCODE_I18N_KEY_NEXT_TO_RETURN] = AMIGA_KEYCODE_I18N_KEY_NEXT_TO_RETURN;

  // keycode_map[PC_KEYCODE_LEFT_SHIFT] = AMIGA_KEYCODE_LEFT_SHIFT;
  // keycode_map[PC_KEYCODE_I18N_KEY_NEXT_TO_LEFT_SHIFT] = AMIGA_KEYCODE_I18N_KEY_NEXT_TO_LEFT_SHIFT;
  keycode_map[PC_KEYCODE_Z] = AMIGA_KEYCODE_Z;
  keycode_map[PC_KEYCODE_X] = AMIGA_KEYCODE_X;
  keycode_map[PC_KEYCODE_C] = AMIGA_KEYCODE_C;
  keycode_map[PC_KEYCODE_V] = AMIGA_KEYCODE_V;
  keycode_map[PC_KEYCODE_B] = AMIGA_KEYCODE_B;
  keycode_map[PC_KEYCODE_N] = AMIGA_KEYCODE_N;
  keycode_map[PC_KEYCODE_M] = AMIGA_KEYCODE_M;
  keycode_map[PC_KEYCODE_COMMA_LESS_THAN] = AMIGA_KEYCODE_COMMA_LESS_THAN;
  keycode_map[PC_KEYCODE_PERIOD_GREATER_THAN] = AMIGA_KEYCODE_PERIOD_GREATER_THAN;
  keycode_map[PC_KEYCODE_SLASH_QUESTION_MARK] = AMIGA_KEYCODE_SLASH_QUESTION_MARK;
  // keycode_map[PC_KEYCODE_RIGHT_SHIFT] = AMIGA_KEYCODE_RIGHT_SHIFT;

  // keycode_map[/*PC_KEYCODE_LEFT_ALT] = AMIGA_KEYCODE_LEFT_ALT;
  // keycode_map[/*PC_KEYCODE_LEFT_AMIGA] = AMIGA_KEYCODE_LEFT_AMIGA;
  keycode_map[PC_KEYCODE_SPACEBAR] = AMIGA_KEYCODE_SPACEBAR;
  // keycode_map[PC_KEYCODE_RIGHT_AMIGA] = AMIGA_KEYCODE_RIGHT_AMIGA;
  // keycode_map[PC_KEYCODE_RIGHT_ALT] = AMIGA_KEYCODE_RIGHT_ALT;

  keycode_map[PC_KEYCODE_DEL] = AMIGA_KEYCODE_DEL;
  keycode_map[PC_KEYCODE_F12] = AMIGA_KEYCODE_HELP;  // F12 => Help

  keycode_map[PC_KEYCODE_CURSOR_UP] = AMIGA_KEYCODE_CURSOR_UP;
  keycode_map[PC_KEYCODE_CURSOR_LEFT] = AMIGA_KEYCODE_CURSOR_LEFT;
  keycode_map[PC_KEYCODE_CURSOR_DOWN] = AMIGA_KEYCODE_CURSOR_DOWN;
  keycode_map[PC_KEYCODE_CURSOR_RIGHT] = AMIGA_KEYCODE_CURSOR_RIGHT;

  // keycode_map[PC_KEYCODE_NUMERIC_OPEN_PARENTHESIS] = AMIGA_KEYCODE_NUMERIC_OPEN_PARENTHESIS;
  // keycode_map[PC_KEYCODE_NUMERIC_CLOSE_PARENTHESIS] = AMIGA_KEYCODE_NUMERIC_CLOSE_PARENTHESIS;
  keycode_map[PC_KEYCODE_NUMERIC_SLASH] = AMIGA_KEYCODE_NUMERIC_SLASH;
  keycode_map[PC_KEYCODE_NUMERIC_ASTERISK] = AMIGA_KEYCODE_NUMERIC_ASTERISK;

  keycode_map[PC_KEYCODE_NUMERIC_7_HOME] = AMIGA_KEYCODE_NUMERIC_7;
  keycode_map[PC_KEYCODE_NUMERIC_8_CURSOR_UP] = AMIGA_KEYCODE_NUMERIC_8;
  keycode_map[PC_KEYCODE_NUMERIC_9_PAGE_UP] = AMIGA_KEYCODE_NUMERIC_9;
  keycode_map[PC_KEYCODE_NUMERIC_MINUS] = AMIGA_KEYCODE_NUMERIC_MINUS;
  keycode_map[PC_KEYCODE_NUMERIC_4_CURSOR_LEFT] = AMIGA_KEYCODE_NUMERIC_4;

  keycode_map[PC_KEYCODE_NUMERIC_5] = AMIGA_KEYCODE_NUMERIC_5;
  keycode_map[PC_KEYCODE_NUMERIC_6_CURSOR_RIGHT] = AMIGA_KEYCODE_NUMERIC_6;
  keycode_map[PC_KEYCODE_NUMERIC_PLUS] = AMIGA_KEYCODE_NUMERIC_PLUS;

  keycode_map[PC_KEYCODE_NUMERIC_1_END] = AMIGA_KEYCODE_NUMERIC_1;
  keycode_map[PC_KEYCODE_NUMERIC_2_CUROR_DOWN] = AMIGA_KEYCODE_NUMERIC_2;
  keycode_map[PC_KEYCODE_NUMERIC_3_PAGE_DOWN] = AMIGA_KEYCODE_NUMERIC_3;
  keycode_map[PC_KEYCODE_NUMERIC_ENTER] = AMIGA_KEYCODE_NUMERIC_ENTER;

  keycode_map[PC_KEYCODE_NUMERIC_0_INSERT] = AMIGA_KEYCODE_NUMERIC_0;
  keycode_map[PC_KEYCODE_NUMERIC_PERIOD] = AMIGA_KEYCODE_NUMERIC_PERIOD;
};