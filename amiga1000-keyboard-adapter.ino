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
#define NEO_OFF pixels.Color(0, 0, 0)

static const uint8_t KBDATA = T11;
static const uint8_t KBCLK = T10;
static const uint8_t DEBUG = T9;

static const uint8_t KEYCODE_F = 0x23;
static const uint8_t KEYCODE_R = 0x13;
static const uint8_t KEYCODE_E = 0x12;
static const uint8_t KEYCODE_D = 0x22;
static const uint8_t KEYCODE_LEFT_ALT = 0x64;
static const uint8_t KEYCODE_CTRL = 0x63;
static const uint8_t KEYCODE_F2 = 0x51;

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

  // Set up serial.
  Serial.begin();
  Serial.println("Settting up...");

  // Configure neopixel.
  pixels.begin();
  neo_color(NEO_PURPLE);

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
}

void loop() {
  delay(5000);
  send_key_up_down(KEYCODE_F2);
  delay(1000);

  send_key_up_down(KEYCODE_F);
  delay(300);
  send_key_up_down(KEYCODE_R);
  delay(300);
  send_key_up_down(KEYCODE_E);
  delay(300);
  send_key_up_down(KEYCODE_D);
  delay(300);

  delay(3000);
  send_keycode(KEYCODE_CTRL);
  send_keycode(KEYCODE_LEFT_ALT);
}

void neo_color(uint32_t color) {
  pixels.setPixelColor(0, color);
  pixels.show();
}

void sync_up() {
  print_uptime();
  Serial.println("\nSyncing...");
  Serial.flush();

  int count = 0;
  while (true) {
    count++;
    pinMode(KBDATA, OUTPUT);

    send_bit(1);
    // digitalWrite(KBDATA, LOW);
    // delayMicroseconds(9);
    // digitalWrite(KBCLK, LOW);
    // delayMicroseconds(4);
    // digitalWrite(KBCLK, HIGH);

    wait_for_ack();
    if (syncd) {
      Serial.printf("...after %i sync bits.\n", count);
      return;
    }
    Serial.printf("NO sync %i.\n", count);
    delay(1);
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
  if (!syncd) {
    sync_up();
  }

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
