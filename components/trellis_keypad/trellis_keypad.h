#pragma once

#include "esphome/components/key_provider/key_provider.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "Adafruit_Trellis.h"

#include <vector>
#include <string>

namespace esphome {
namespace trellis_keypad {

class TrellisKeypadBinarySensor;  // forward

class TrellisKeypad : public key_provider::KeyProvider, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_keys(std::string keys) { keys_ = std::move(keys); }

  void send_key(uint8_t keycode) { this->send_key_(keycode); }

  void register_button(TrellisKeypadBinarySensor *button) { this->buttons_.push_back(button); }

  // LED control for light output
  void set_led_for_key(uint8_t keycode, bool on);

  std::string keys_;  // 16 chars, mapping pad index -> key

  Adafruit_Trellis trellis_;  // underlying monochrome Trellis

  std::vector<TrellisKeypadBinarySensor *> buttons_{};

 protected:
  // remember previous switch state to detect edges
  uint16_t last_switches_ = 0;
};

}  // namespace trellis_keypad
}  // namespace esphome
