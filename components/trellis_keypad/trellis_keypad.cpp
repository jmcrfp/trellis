void TrellisKeypad::loop() {
  // Read current switch states
  this->trellis_.readSwitches();

  uint16_t current_switches = 0;
  for (uint8_t i = 0; i < 16; i++) {
    if (this->trellis_.isKeyPressed(i)) {
      current_switches |= (1u << i);
    }
  }

  uint32_t now = millis();
  uint16_t changed = current_switches ^ this->last_switches_;

  if (changed != 0) {
    for (uint8_t i = 0; i < 16; i++) {
      if (changed & (1u << i)) {
        // Debounce: ignore if too soon since last change
        uint32_t dt = now - this->last_event_ms_[i];
        if (dt < DEBOUNCE_MS) {
          ESP_LOGVV(TAG, "Debouncing key index=%u (dt=%u ms)", i, (unsigned)dt);
          continue;
        }
        this->last_event_ms_[i] = now;

        bool now_pressed = (current_switches & (1u << i)) != 0;

        if (i < this->keys_.size()) {
          uint8_t keycode = this->keys_[i];

          ESP_LOGD(TAG, "Trellis event: index=%u, key='%c', edge=%s",
                   i, keycode, now_pressed ? "PRESSED" : "RELEASED");

          for (auto *button : this->buttons_) {
            if (button == nullptr)
              continue;
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
          ESP_LOGW(TAG, "Key index %u has no mapping in keys '%s'",
                   i, this->keys_.c_str());
        }
      }
    }
  }

  this->last_switches_ = current_switches;
}
