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

  if (!usb_dev_begun) {
    Serial.println("Starting USB");
    Keyboard.begin();
    USB.begin();
  }
  keybd_dev_addr = usbNum;
}

static void on_usb_keyboard_data(uint8_t usbNum, uint8_t byte_depth, uint8_t* data, uint8_t data_len) {
  // Forward data to microcontroller USB bus.
  // Keyboard.sendReport((KeyReport*)data);

  Serial.printf("in: ");
  for (int k = 0; k < data_len; k++) {
    Serial.printf("0x%02x ", data[k]);
  }
  Serial.printf("\n");

  // Populate fake PC key codes for modifier keys.
  usb_reported[0] = (data[0] & ((1 << 0) | (1 << 4))) != 0 ? PC_KEYCODE_FAKE_CTRL : PC_KEYCODE_NOT_A_KEY;
  usb_reported[1] = (data[0] & (1 << 1)) != 0 ? PC_KEYCODE_FAKE_LEFT_SHIFT : PC_KEYCODE_NOT_A_KEY;
  usb_reported[2] = (data[0] & (1 << 2)) != 0 ? PC_KEYCODE_FAKE_LEFT_ALT : PC_KEYCODE_NOT_A_KEY;
  usb_reported[3] = (data[0] & (1 << 3)) != 0 ? PC_KEYCODE_FAKE_LEFT_SUPER : PC_KEYCODE_NOT_A_KEY;
  usb_reported[4] = (data[0] & (1 << 5)) != 0 ? PC_KEYCODE_FAKE_RIGHT_SHIFT : PC_KEYCODE_NOT_A_KEY;
  usb_reported[5] = (data[0] & (1 << 6)) != 0 ? PC_KEYCODE_FAKE_RIGHT_ALT : PC_KEYCODE_NOT_A_KEY;
  usb_reported[6] = (data[0] & (1 << 7)) != 0 ? PC_KEYCODE_FAKE_RIGHT_SUPER : PC_KEYCODE_NOT_A_KEY;
  // Copy over non-modfier key codes.
  for (int i = 0; i < 6; i++) {
    uint8_t pc_code = data[2 + i];
    if (pc_code == 0x00) {
      pc_code = PC_KEYCODE_NOT_A_KEY;
    }
    usb_reported[7 + i] = pc_code;
  }

  // Check for CTRL-SUPER-SUPER.
  // http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0178.html
  bool reset_keys = usb_reported[0] != PC_KEYCODE_NOT_A_KEY && usb_reported[3] != PC_KEYCODE_NOT_A_KEY && usb_reported[6] != PC_KEYCODE_NOT_A_KEY;
  if (reset_keys | ctrl_amiga_amiga) {
    ctrl_amiga_amiga = reset_keys;
    send_amiga_hard_reset(ctrl_amiga_amiga);
  }

  // Don't send key data while reset keys are being held.
  if (ctrl_amiga_amiga) {
    return;
  }

  // Sort.
  bubble_sort(usb_reported, usb_reported_len);

  // Sort.
  bubble_sort(down_keys, down_keys_len);

  // Serial.printf("known down_keys: ");
  // for (int i = 0; i < down_keys_len; i++) {
  //   Serial.printf("0x%02x ", down_keys[i]);
  // }
  // Serial.printf("   usb_reported: ");
  // for (int i = 0; i < usb_reported_len; i++) {
  //   Serial.printf("0x%02x ", usb_reported[i]);
  // }
  // Serial.printf("\n");

  // Compare known down_keys with usb_reported down keys.
  for (int i = 0; i < down_keys_len; i++) {
    if (down_keys[i] > usb_reported[i]) {
      // Newly pressed key.
      send_pc_key_state_change(usb_reported[i], 0x00);
      for (int k = down_keys_len - 1; k > i; k--) {
        down_keys[k] = down_keys[k - 1];
      }
      down_keys[i] = usb_reported[i];
    } else if (down_keys[i] < usb_reported[i]) {
      // Newly released key.
      send_pc_key_state_change(down_keys[i], 0x80);
      for (int k = i; k < down_keys_len - 1; k++) {
        down_keys[k] = down_keys[k + 1];
      }
      down_keys[down_keys_len - 1] = PC_KEYCODE_NOT_A_KEY;
    }
  }

  // Serial.printf("known down_keys: ");
  // for (int i = 0; i < down_keys_len; i++) {
  //   Serial.printf("0x%02x ", down_keys[i]);
  // }
  // Serial.printf("\n");
  // Serial.printf("\n");
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

  // http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0175.html
  if (amiga_code == AMIGA_KEYCODE_CAPS_LOCK) {
    if (amiga_down_up_mask) {
      return;
    }
    capslock_on = !capslock_on;
    send_amiga_keycode(capslock_on ? amiga_code : (amiga_code | 0x80));
    return;
  }

  send_amiga_keycode(amiga_code | amiga_down_up_mask);
}

// http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0179.html
void send_amiga_hard_reset(bool reset_keys_held) {
  digitalWrite(KBCLK, reset_keys_held ? LOW : HIGH);
}