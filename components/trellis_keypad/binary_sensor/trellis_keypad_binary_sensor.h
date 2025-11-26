#pragma once

#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace trellis_keypad {

class TrellisKeypadBinarySensor : public binary_sensor::BinarySensor {
 public:
  // called from Python glue to set the key
  void set_key(const std::string &key) {
    if (!key.empty()) {
      key_ = key[0];
    }
  }

  char key_{0};
};

}  // namespace trellis_keypad
}  // namespace esphome
