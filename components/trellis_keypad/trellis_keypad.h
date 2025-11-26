#pragma once

#include "esphome/components/key_provider/key_provider.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "Adafruit_Trellis.h"

#include <vector>
#include <string>

namespace esphome {
namespace trellis_keypad {

class TrellisKeypadBinarySensor;  // forward declaration

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

  // 16-character key mapping
  std::string keys_;
  Adafruit_Trellis trellis_;

  // list of binary_sensor buttons connected to this keypad
  std::vector<TrellisKeypadBinarySensor *> buttons_{};

 protected:
  uint16_t last_switches_{0};

  // debounce: time of last event per key (ms since boot)
  uint32_t last_event_ms_[16] = {0};
  static constexpr uint32_t DEBOUNCE_MS = 30;
};

}  // namespace trellis_keypad
}  // namespace esphome
