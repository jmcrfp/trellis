#include "trellis_keypad.h"
#include "esphome/core/log.h"

namespace esphome {
namespace trellis_keypad {

static const char *const TAG = "trellis_keypad";

void TrellisKeypad::setup() {
  ESP_LOGI(TAG, "Setting up Adafruit Trellis (monochrome) keypad...");

  // Default I2C address for a single Trellis board is usually 0x70
  if (!this->trellis_.begin(0x70)) {
    ESP_LOGE(TAG, "Failed to initialize Adafruit_Trellis at address 0x70");
    mark_failed();
    return;
  }

  ESP_LOGD(TAG, "Adafruit_Trellis begin() success");
  ESP_LOGD(TAG, "Configured keys string: '%s' (length=%u)",
           this->keys_.c_str(), (unsigned)this->keys_.size());

  if (this->keys_.size() != 16) {
    ESP_LOGW(TAG, "Expected 16 keys, but got %u; behavior may be undefined",
             (unsigned)this->keys_.size());
  }

  // Clear LEDs initially
  for (uint8_t i = 0; i < 16; i++) {
    this->trellis_.clrLED(i);
  }
  this->trellis_.writeDisplay();

  this->last_switches_ = 0;
  ESP_LOGI(TAG, "Trellis keypad setup complete");
}

void TrellisKeypad::loop() {
  // Read current state of switches
  if (!this->trellis_.readSwitches()) {
    // Typically readSwitches() returns bool; if not needed, you can ignore return
    ESP_LOGVV(TAG, "readSwitches() called");
  }

  uint16_t current_switches = 0;
  for (uint8_t i = 0; i < 16; i++) {
    if (this->trellis_.isKeyPressed(i)) {
      current_switches |= (1 << i);
    }
  }

  // Detect edges
  uint16_t changed = current_switches ^ this->last_switches_;
  if (changed != 0) {
    for (uint8_t i = 0; i < 16; i++) {
      if (changed & (1 << i)) {
        bool now_pressed = (current_switches & (1 << i)) != 0;

        if (i < this->keys_.size()) {
          uint8_t keycode = this->keys_[i];

          ESP_LOGD(TAG, "Trellis event: index=%u, key='%c', edge=%s",
                   i, keycode, now_pressed ? "PRESSED" : "RELEASED");

          // update binary_sensors
          for (auto *button : this->buttons_) {
            if (button->key_ == keycode) {
              ESP_LOGD(TAG, "Publishing state for key '%c' -> %s",
                       keycode, now_pressed ? "ON" : "OFF");
              button->publish_state(now_pressed);
            }
          }

          if (now_pressed) {
            ESP_LOGV(TAG, "Sending key '%c' to key_provider", keycode);
            this->send_key(keycode);
          }
        } else {
          ESP_LOGW(TAG, "Key index %u has no mapping in keys string '%s'",
                   i, this->keys_.c_str());
        }
      }
    }
  }

  this->last_switches_ = current_switches;
}

void TrellisKeypad::set_led_for_key(uint8_t keycode, bool on) {
  // Find index from key
  int index = this->keys_.find_first_of((char)keycode);
  if (index < 0 || index >= 16) {
    ESP_LOGW(TAG, "set_led_for_key: key '%c' not found in keys '%s'",
             keycode, this->keys_.c_str());
    return;
  }

  if (on) {
    this->trellis_.setLED((uint8_t)index);
  } else {
    this->trellis_.clrLED((uint8_t)index);
  }
  this->trellis_.writeDisplay();
}

void TrellisKeypad::dump_config() {
  ESP_LOGCONFIG(TAG, "Adafruit Trellis (monochrome) Keypad:");
  ESP_LOGCONFIG(TAG, "  Keys: %s", this->keys_.c_str());
  ESP_LOGCONFIG(TAG, "  Registered binary_sensor buttons: %u",
                (unsigned)this->buttons_.size());
}

}  // namespace trellis_keypad
}  // namespace esphome
