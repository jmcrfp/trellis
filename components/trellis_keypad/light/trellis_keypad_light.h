#pragma once

#include "esphome/components/light/light_output.h"
#include "esphome/core/log.h"
#include "../trellis_keypad.h"

namespace esphome {
namespace trellis_keypad {

class TrellisKeypadLight : public light::LightOutput {
 public:
  light::LightTraits get_traits() override {
    light::LightTraits traits;
    traits.set_supported_color_modes({light::ColorMode::ON_OFF});
    return traits;
  }

  void write_state(light::LightState *state) override {
    if (index_ == -1 || pad_ == nullptr)
      return;

    bool on = state->current_values.is_on();
    ESP_LOGD("trellis_keypad", "Setting LED for index %d (%c) to %s",
             index_, key_, on ? "ON" : "OFF");
    pad_->set_led_for_key((uint8_t)key_, on);
  }

  void set_pad(TrellisKeypad *pad, const std::string &key) {
    pad_ = pad;
    if (key.empty()) {
      index_ = -1;
      key_ = 0;
      return;
    }
    key_ = key[0];
    index_ = pad_->keys_.find_first_of(key_);
  }

 protected:
  TrellisKeypad *pad_{nullptr};
  int index_{-1};
  char key_{0};
};

}  // namespace trellis_keypad
}  // namespace esphome
