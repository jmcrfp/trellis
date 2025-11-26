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

  void register_button(TrellisKeypadBinarySensor *button) { buttons_.push_back(button); }

  void set_led_for_key(uint8_t keycode, bool on);

  std::string keys_;
  Adafruit_Trellis trellis_;
  std::vector<TrellisKeypadBinarySensor *> buttons_{};

 protected:
  uint16_t last_switches_{0};

  // debounce: time of last event per key (ms since boot)
  uint32_t last_event_ms_[16] = {0};
  static constexpr uint32_t DEBOUNCE_MS = 30;  // adjust if needed
};

}  // namespace trellis_keypad
}  // namespace esphome
