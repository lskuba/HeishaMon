#pragma once

#include "esphome/components/select/select.h"
#include "panasonic_aquarea.h"

namespace esphome {
namespace panasonic_aquarea {

class AquareaSelect : public select::Select, public Component {
 public:
  void set_parent(PanasonicAquarea *parent) { parent_ = parent; }
  void set_select_type(const std::string &type) { select_type_ = type; }
  
  void setup() override {}
  
  void control(const std::string &value) override {
    if (parent_ == nullptr) return;
    
    if (select_type_ == "operating_mode") {
      uint8_t mode = 0;
      if (value == "Heat only") mode = 0;
      else if (value == "Cool only") mode = 1;
      else if (value == "Auto") mode = 2;
      else if (value == "DHW only") mode = 3;
      else if (value == "Heat+DHW") mode = 4;
      else if (value == "Cool+DHW") mode = 5;
      else if (value == "Auto+DHW") mode = 6;
      parent_->set_operating_mode(mode);
    } else if (select_type_ == "quiet_mode") {
      uint8_t level = 0;
      if (value == "Off") level = 0;
      else if (value == "Level 1") level = 1;
      else if (value == "Level 2") level = 2;
      else if (value == "Level 3") level = 3;
      parent_->set_quiet_mode(level);
    } else if (select_type_ == "powerful_mode") {
      uint8_t time = 0;
      if (value == "Off") time = 0;
      else if (value == "30 min") time = 1;
      else if (value == "60 min") time = 2;
      else if (value == "90 min") time = 3;
      parent_->set_powerful_mode(time);
    }
    
    this->publish_state(value);
  }

 protected:
  PanasonicAquarea *parent_{nullptr};
  std::string select_type_;
};

}  // namespace panasonic_aquarea
}  // namespace esphome
