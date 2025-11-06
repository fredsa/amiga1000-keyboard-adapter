// Amiga 1000 keyboard connector
// 1 GND         - BLACK
// 2 Vcc (+5V)   - RED
// 3 KBDATA      - GREEN
// 4 KBCLK       - YELLOW

#include <Adafruit_NeoPixel.h>

#define NEO_RED pixels.Color(75, 0, 0)
#define NEO_GREEN pixels.Color(0, 75, 0)
#define NEO_BLUE pixels.Color(0, 0, 75)
#define NEO_PURPLE pixels.Color(75, 0, 75)
#define NEO_YELLOW pixels.Color(75, 75, 0)
#define NEO_OFF pixels.Color(0, 0, 0)

static const uint8_t KBDATA = T11;
static const uint8_t KBCLK = T10;
static const uint8_t DEBUG = T9;


// http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node017A.html
static const uint8_t KEYCODE_BACKTICK_TILDE = 0x00;
static const uint8_t KEYCODE_1_EXCLAMATION = 0x01;
static const uint8_t KEYCODE_2_AT_SIGN = 0x02;
static const uint8_t KEYCODE_3_POUND_SIGN = 0x03;
static const uint8_t KEYCODE_4_DOLLAR_SIGN = 0x04;
static const uint8_t KEYCODE_5_PERCENT = 0x05;
static const uint8_t KEYCODE_6_HAT = 0x06;
static const uint8_t KEYCODE_7_AMPERSAND = 0x07;
static const uint8_t KEYCODE_8_ASTERISK = 0x08;
static const uint8_t KEYCODE_9_OPEN_PARENTHESIS = 0x09;
static const uint8_t KEYCODE_0_CLOSE_PARENTHESIS = 0x0A;
static const uint8_t KEYCODE_MINUS_UNDERSCORE = 0x0B;
static const uint8_t KEYCODE_EQUALS_PLUS = 0x0C;
static const uint8_t KEYCODE_BACKSLASH_PIPE = 0x0D;
static const uint8_t KEYCODE_NUMERIC_0 = 0x0F;
static const uint8_t KEYCODE_Q = 0x10;
static const uint8_t KEYCODE_W = 0x11;
static const uint8_t KEYCODE_E = 0x12;
static const uint8_t KEYCODE_R = 0x13;
static const uint8_t KEYCODE_T = 0x14;
static const uint8_t KEYCODE_Y = 0x15;
static const uint8_t KEYCODE_U = 0x16;
static const uint8_t KEYCODE_I = 0x17;
static const uint8_t KEYCODE_O = 0x18;
static const uint8_t KEYCODE_P = 0x19;
static const uint8_t KEYCODE_LEFT_SQUARE_SQUIGLEY_BRACKET = 0x1A;
static const uint8_t KEYCODE_RIGHT_SQUARE_SQUIGLEY_BRACKET = 0x1B;
// static const uint8_t KEYCODE_UNUSED_1C = 0x1C;
static const uint8_t KEYCODE_NUMERIC_1 = 0x1D;
static const uint8_t KEYCODE_NUMERIC_2 = 0x1E;
static const uint8_t KEYCODE_NUMERIC_3 = 0x1F;
static const uint8_t KEYCODE_A = 0x20;
static const uint8_t KEYCODE_S = 0x21;
static const uint8_t KEYCODE_D = 0x22;
static const uint8_t KEYCODE_F = 0x23;
static const uint8_t KEYCODE_G = 0x24;
static const uint8_t KEYCODE_H = 0x25;
static const uint8_t KEYCODE_J = 0x26;
static const uint8_t KEYCODE_K = 0x27;
static const uint8_t KEYCODE_L = 0x28;
static const uint8_t KEYCODE_SEMICOLON_COLON = 0x29;
static const uint8_t KEYCODE_SINGLE_DOUBLE_QUOTE = 0x2A;
static const uint8_t KEYCODE_I18N_KEY_NEXT_TO_RETURN = 0x2B;
// static const uint8_t KEYCODE_UNUSED_2C = 0x2C;
static const uint8_t KEYCODE_NUMERIC_4 = 0x2D;
static const uint8_t KEYCODE_NUMERIC_5 = 0x2E;
static const uint8_t KEYCODE_NUMERIC_6 = 0x2F;
static const uint8_t KEYCODE_I18N_KEY_NEXT_TO_LEFT_SHIFT = 0x30;
static const uint8_t KEYCODE_Z = 0x31;
static const uint8_t KEYCODE_X = 0x32;
static const uint8_t KEYCODE_C = 0x33;
static const uint8_t KEYCODE_V = 0x34;
static const uint8_t KEYCODE_B = 0x35;
static const uint8_t KEYCODE_N = 0x36;
static const uint8_t KEYCODE_M = 0x37;
static const uint8_t KEYCODE_COMMA_LESS_THAN = 0x38;
static const uint8_t KEYCODE_PERIOD_GREATER_THAN = 0x39;
static const uint8_t KEYCODE_SLASH_QUESTION_MARK = 0x3A;
// static const uint8_t KEYCODE_UNUSED_3B = 0x3B;
static const uint8_t KEYCODE_NUMERIC_PERIOD = 0x3C;
static const uint8_t KEYCODE_NUMERIC_7 = 0x3D;
static const uint8_t KEYCODE_NUMERIC_8 = 0x3E;
static const uint8_t KEYCODE_NUMERIC_9 = 0x3F;
static const uint8_t KEYCODE_SPACEBAR = 0x40;
static const uint8_t KEYCODE_BACKSPACE = 0x41;
static const uint8_t KEYCODE_TAB = 0x42;
static const uint8_t KEYCODE_NUMERIC_ENTER = 0x43;
static const uint8_t KEYCODE_RETURN = 0x44;
static const uint8_t KEYCODE_ESC = 0x45;
static const uint8_t KEYCODE_DEL = 0x46;
// static const uint8_t KEYCODE_UNUSED_47 = 0x47;
// static const uint8_t KEYCODE_UNUSED_48 = 0x48;
// static const uint8_t KEYCODE_UNUSED_49 = 0x49;
static const uint8_t KEYCODE_NUMERIC_MINUS = 0x4A;
static const uint8_t KEYCODE_CURSOR_UP = 0x4C;
static const uint8_t KEYCODE_CURSOR_DOWN = 0x4D;
static const uint8_t KEYCODE_CURSOR_RIGHT = 0x4E;
static const uint8_t KEYCODE_CURSOR_LEFT = 0x4F;
static const uint8_t KEYCODE_F1 = 0x50;
static const uint8_t KEYCODE_F2 = 0x51;
static const uint8_t KEYCODE_F3 = 0x52;
static const uint8_t KEYCODE_F4 = 0x53;
static const uint8_t KEYCODE_F5 = 0x54;
static const uint8_t KEYCODE_F6 = 0x55;
static const uint8_t KEYCODE_F7 = 0x56;
static const uint8_t KEYCODE_F8 = 0x57;
static const uint8_t KEYCODE_F9 = 0x58;
static const uint8_t KEYCODE_F10 = 0x59;
static const uint8_t KEYCODE_NUMERIC_OPEN_PARENTHESIS = 0x5A;
static const uint8_t KEYCODE_NUMERIC_CLOSE_PARENTHESIS = 0x5B;
static const uint8_t KEYCODE_NUMERIC_SLASH = 0x5C;
static const uint8_t KEYCODE_NUMERIC_ASTERISK = 0x5D;
static const uint8_t KEYCODE_NUMERIC_PLUS = 0x5E;
static const uint8_t KEYCODE_HELP = 0x5F;
static const uint8_t KEYCODE_LEFT_SHIFT = 0x60;
static const uint8_t KEYCODE_RIGHT_SHIFT = 0x61;
static const uint8_t KEYCODE_CAPS_LOCK = 0x62;
static const uint8_t KEYCODE_CTRL = 0x63;
static const uint8_t KEYCODE_LEFT_ALT = 0x64;
static const uint8_t KEYCODE_RIGHT_ALT = 0x65;
static const uint8_t KEYCODE_LEFT_AMIGA = 0x66;
static const uint8_t KEYCODE_RIGHT_AMIGA = 0x67;

static const uint8_t test_keys[] = {
  KEYCODE_ESC,
  KEYCODE_F1,
  KEYCODE_F2,
  KEYCODE_F3,
  KEYCODE_F4,
  KEYCODE_F5,
  KEYCODE_F6,
  KEYCODE_F7,
  KEYCODE_F8,
  KEYCODE_F9,
  KEYCODE_F10,
  KEYCODE_BACKTICK_TILDE,
  KEYCODE_1_EXCLAMATION,
  KEYCODE_2_AT_SIGN,
  KEYCODE_3_POUND_SIGN,
  KEYCODE_4_DOLLAR_SIGN,
  KEYCODE_5_PERCENT,
  KEYCODE_6_HAT,
  KEYCODE_7_AMPERSAND,
  KEYCODE_8_ASTERISK,
  KEYCODE_9_OPEN_PARENTHESIS,
  KEYCODE_0_CLOSE_PARENTHESIS,
  KEYCODE_MINUS_UNDERSCORE,
  KEYCODE_EQUALS_PLUS,
  KEYCODE_BACKSLASH_PIPE,
  KEYCODE_BACKSPACE,

  KEYCODE_TAB,
  KEYCODE_Q,
  KEYCODE_W,
  KEYCODE_E,
  KEYCODE_R,
  KEYCODE_T,
  KEYCODE_Y,
  KEYCODE_U,
  KEYCODE_I,
  KEYCODE_O,
  KEYCODE_P,
  KEYCODE_LEFT_SQUARE_SQUIGLEY_BRACKET,
  KEYCODE_RIGHT_SQUARE_SQUIGLEY_BRACKET,
  KEYCODE_RETURN,

  KEYCODE_CTRL,
  KEYCODE_CAPS_LOCK,
  KEYCODE_A,
  KEYCODE_S,
  KEYCODE_D,
  KEYCODE_F,
  KEYCODE_G,
  KEYCODE_H,
  KEYCODE_J,
  KEYCODE_K,
  KEYCODE_L,
  KEYCODE_SEMICOLON_COLON,
  KEYCODE_SINGLE_DOUBLE_QUOTE,
  KEYCODE_I18N_KEY_NEXT_TO_RETURN,

  KEYCODE_LEFT_SHIFT,
  KEYCODE_I18N_KEY_NEXT_TO_LEFT_SHIFT,
  KEYCODE_Z,
  KEYCODE_X,
  KEYCODE_C,
  KEYCODE_V,
  KEYCODE_B,
  KEYCODE_N,
  KEYCODE_M,
  KEYCODE_COMMA_LESS_THAN,
  KEYCODE_PERIOD_GREATER_THAN,
  KEYCODE_SLASH_QUESTION_MARK,
  KEYCODE_RIGHT_SHIFT,

  KEYCODE_LEFT_ALT,
  KEYCODE_LEFT_AMIGA,
  KEYCODE_SPACEBAR,
  KEYCODE_RIGHT_AMIGA,
  KEYCODE_RIGHT_ALT,

  KEYCODE_DEL,
  KEYCODE_HELP,

  KEYCODE_CURSOR_UP,
  KEYCODE_CURSOR_LEFT,
  KEYCODE_CURSOR_DOWN,
  KEYCODE_CURSOR_RIGHT,

  KEYCODE_NUMERIC_OPEN_PARENTHESIS,
  KEYCODE_NUMERIC_CLOSE_PARENTHESIS,
  KEYCODE_NUMERIC_SLASH,
  KEYCODE_NUMERIC_ASTERISK,

  KEYCODE_NUMERIC_7,
  KEYCODE_NUMERIC_8,
  KEYCODE_NUMERIC_9,
  KEYCODE_NUMERIC_MINUS,

  KEYCODE_NUMERIC_4,
  KEYCODE_NUMERIC_5,
  KEYCODE_NUMERIC_6,
  KEYCODE_NUMERIC_PLUS,

  KEYCODE_NUMERIC_1,
  KEYCODE_NUMERIC_2,
  KEYCODE_NUMERIC_3,
  KEYCODE_NUMERIC_ENTER,

  KEYCODE_NUMERIC_0,
  KEYCODE_NUMERIC_PERIOD,
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

  // Configure digital pins.
  pinMode(KBCLK, OUTPUT);
  pinMode(KBDATA, OUTPUT);
  pinMode(DEBUG, OUTPUT);
  digitalWrite(KBCLK, HIGH);
  digitalWrite(KBDATA, HIGH);
  digitalWrite(DEBUG, HIGH);

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
  } else {
    for (int i = 0; i < cmd.length(); i++) {
      sendChar(cmd[i]);
    }
    send_key_up_down(KEYCODE_RETURN);
  }
}

void sendChar(char c) {
  switch (c) {
    case 'A':
    case 'a':
      send_key_up_down(KEYCODE_A);
      break;
    case 'B':
    case 'b':
      send_key_up_down(KEYCODE_B);
      break;
    case 'C':
    case 'c':
      send_key_up_down(KEYCODE_C);
      break;
    case 'D':
    case 'd':
      send_key_up_down(KEYCODE_D);
      break;
    case 'E':
    case 'e':
      send_key_up_down(KEYCODE_E);
      break;
    case 'F':
    case 'f':
      send_key_up_down(KEYCODE_F);
      break;
    case 'G':
    case 'g':
      send_key_up_down(KEYCODE_G);
      break;
    case 'H':
    case 'h':
      send_key_up_down(KEYCODE_H);
      break;
    case 'I':
    case 'i':
      send_key_up_down(KEYCODE_I);
      break;
    case 'J':
    case 'j':
      send_key_up_down(KEYCODE_J);
      break;
    case 'K':
    case 'k':
      send_key_up_down(KEYCODE_K);
      break;
    case 'L':
    case 'l':
      send_key_up_down(KEYCODE_L);
      break;
    case 'M':
    case 'm':
      send_key_up_down(KEYCODE_M);
      break;
    case 'N':
    case 'n':
      send_key_up_down(KEYCODE_N);
      break;
    case 'O':
    case 'o':
      send_key_up_down(KEYCODE_O);
      break;
    case 'P':
    case 'p':
      send_key_up_down(KEYCODE_P);
      break;
    case 'Q':
    case 'q':
      send_key_up_down(KEYCODE_Q);
      break;
    case 'R':
    case 'r':
      send_key_up_down(KEYCODE_R);
      break;
    case 'S':
    case 's':
      send_key_up_down(KEYCODE_S);
      break;
    case 'T':
    case 't':
      send_key_up_down(KEYCODE_T);
      break;
    case 'U':
    case 'u':
      send_key_up_down(KEYCODE_U);
      break;
    case 'V':
    case 'v':
      send_key_up_down(KEYCODE_V);
      break;
    case 'W':
    case 'w':
      send_key_up_down(KEYCODE_W);
      break;
    case 'X':
    case 'x':
      send_key_up_down(KEYCODE_X);
      break;
    case 'Y':
    case 'y':
      send_key_up_down(KEYCODE_Y);
      break;
    case 'Z':
    case 'z':
      send_key_up_down(KEYCODE_Z);
      break;
    case ' ':
      send_key_up_down(KEYCODE_SPACEBAR);
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
  send_keycode(KEYCODE_CTRL);
  send_keycode(KEYCODE_LEFT_ALT);
  delay(500);

  // Enter Amiga Test Kit 1.21 keyboard test.
  send_key_up_down(KEYCODE_F2);
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
  send_keycode(KEYCODE_CTRL);
  send_keycode(KEYCODE_LEFT_ALT);
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
