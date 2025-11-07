#include <Adafruit_NeoPixel.h>

#include "amiga1000-keyboard-adapter.h"

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
      send_typed_char(cmd[i]);
    }
    send_amiga_keycode_up_down(AMIGA_KEYCODE_RETURN);
  }
}

void send_typed_char(char c) {
  switch (c) {
    case 'A':
    case 'a':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_A);
      break;
    case 'B':
    case 'b':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_B);
      break;
    case 'C':
    case 'c':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_C);
      break;
    case 'D':
    case 'd':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_D);
      break;
    case 'E':
    case 'e':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_E);
      break;
    case 'F':
    case 'f':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_F);
      break;
    case 'G':
    case 'g':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_G);
      break;
    case 'H':
    case 'h':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_H);
      break;
    case 'I':
    case 'i':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_I);
      break;
    case 'J':
    case 'j':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_J);
      break;
    case 'K':
    case 'k':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_K);
      break;
    case 'L':
    case 'l':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_L);
      break;
    case 'M':
    case 'm':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_M);
      break;
    case 'N':
    case 'n':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_N);
      break;
    case 'O':
    case 'o':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_O);
      break;
    case 'P':
    case 'p':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_P);
      break;
    case 'Q':
    case 'q':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_Q);
      break;
    case 'R':
    case 'r':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_R);
      break;
    case 'S':
    case 's':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_S);
      break;
    case 'T':
    case 't':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_T);
      break;
    case 'U':
    case 'u':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_U);
      break;
    case 'V':
    case 'v':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_V);
      break;
    case 'W':
    case 'w':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_W);
      break;
    case 'X':
    case 'x':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_X);
      break;
    case 'Y':
    case 'y':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_Y);
      break;
    case 'Z':
    case 'z':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_Z);
      break;
    case ' ':
      send_amiga_keycode_up_down(AMIGA_KEYCODE_SPACEBAR);
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

void amiga_sync_up() {
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

    wait_for_amiga_ack();
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

void wait_for_amiga_ack() {
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
  send_amiga_keycode(AMIGA_KEYCODE_CTRL);
  send_amiga_keycode(AMIGA_KEYCODE_LEFT_ALT);
  delay(500);

  // Enter Amiga Test Kit 1.21 keyboard test.
  send_amiga_keycode_up_down(AMIGA_KEYCODE_F2);
  delay(1000);

  // Send test keys.
  size_t array_size = sizeof(test_keys) / sizeof(test_keys[0]);
  for (int i = 0; i < array_size; i++) {
    if (!syncd) {
      // Abort.
      return;
    }
    send_amiga_keycode_up_down(test_keys[i]);
  }

  delay(3000);

  // Exit back to main menu.
  send_amiga_keycode(AMIGA_KEYCODE_CTRL);
  send_amiga_keycode(AMIGA_KEYCODE_LEFT_ALT);
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

void send_amiga_keycode_up_down(uint8_t keycode) {
  send_amiga_keycode(keycode);
  delay(300);
  send_amiga_keycode(keycode | 0x80);
}

void send_amiga_keycode(uint8_t keycode) {
  amiga_sync_up();

  neo_color(NEO_BLUE);
  for (int i = 6; i >= 0; i--) {
    uint8_t bit = (keycode >> i) & 0x01;
    send_bit(bit);
  }

  uint8_t bit = keycode >> 7;
  send_bit(bit);

  digitalWrite(KBDATA, HIGH);

  wait_for_amiga_ack();
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

  if (!usb_dev_begun) {
    Serial.println("Starting USB");
    Keyboard.begin();
    USB.begin();

    Serial.printf("down_keys: ");
    for (int i = 0; i < down_keys_len; i++) {
      Serial.printf("0x%02x ", down_keys[i]);
    }
    Serial.printf("\n");
  }
}

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

  // Filter out all modifier keys.
  data[0] = PC_KEYCODE_NOT_A_KEY;
  // Filter out vendor code.
  data[1] = PC_KEYCODE_NOT_A_KEY;
  // Prepare for sorting.
  for (int i = 0; i < data_len; i++) {
    if (data[i] == 0) {
      data[i] = PC_KEYCODE_NOT_A_KEY;
    }
  }
  // Sort.
  bubble_sort(data, data_len);

  // Sort.
  bubble_sort(down_keys, down_keys_len);

  Serial.printf("down_keys: ");
  for (int i = 0; i < down_keys_len; i++) {
    Serial.printf("0x%02x ", down_keys[i]);
  }
  Serial.printf("   HID: ");
  for (int i = 0; i < data_len; i++) {
    Serial.printf("0x%02x ", data[i]);
  }
  Serial.printf("\n");

  // down_keys 0x56   data 0x34    =>  0x34 down
  // down_keys 0xff   data 0x34    =>  0x34 down
  // down_keys 0x34   data 0x12    =>  0x12 down

  // down_keys 0x12   data 0x34    =>  0x12 up
  // down_keys 0x34   data 0x56    =>  0x34 up
  // down_keys 0x34   data 0xff    =>  0x34 up

  // Compare.
  for (int i = 0; i < down_keys_len; i++) {
    if (down_keys[i] > data[i]) {
      // Newly pressed key.
      send_pc_key_state_change(data[i], 0x00);
      for (int k = down_keys_len - 1; k > i + 1; k--) {
        down_keys[k] = down_keys[k - 1];
      }
      down_keys[i] = data[i];
    } else if (down_keys[i] < data[i]) {
      // Newly released key.
      send_pc_key_state_change(down_keys[i], 0x80);
      for (int k = i; k < down_keys_len - 1; k++) {
        down_keys[k] = down_keys[k + 1];
      }
      down_keys[down_keys_len - 1] = PC_KEYCODE_NOT_A_KEY;
    }
  }

  Serial.printf("down_keys: ");
  for (int i = 0; i < down_keys_len; i++) {
    Serial.printf("0x%02x ", down_keys[i]);
  }
  Serial.printf("\n");
  Serial.printf("\n");

  // Keyboard.sendReport((KeyReport*)data);
}

void bubble_sort(uint8_t arr[], int size) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        // Swap elements
        uint8_t temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}

void send_pc_key_state_change(uint8_t pc_code, uint8_t amiga_down_up_mask) {
  if (pc_code == AMIGA_KEYCODE_NOT_A_KEY) {
    return;
  }
  uint8_t amiga_code = keycode_map[pc_code];
  if (amiga_code == AMIGA_KEYCODE_NOT_A_KEY) {
    return;
  }
  send_amiga_keycode(amiga_code | amiga_down_up_mask);
}
