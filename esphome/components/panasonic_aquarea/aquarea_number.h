#pragma once

#include "esphome/components/number/number.h"
#include "panasonic_aquarea.h"

namespace esphome {
namespace panasonic_aquarea {

class AquareaNumber : public number::Number, public Component {
 public:
  void set_parent(PanasonicAquarea *parent) { parent_ = parent; }
  void set_number_type(const std::string &type) { number_type_ = type; }
  
  void setup() override {}
  
  void control(float value) override {
    if (parent_ == nullptr) return;
    
    if (number_type_ == "dhw_target") {
      parent_->set_dhw_target_temp(value);
    } else if (number_type_ == "z1_heat_target") {
      parent_->set_z1_heat_target_temp(value);
    } else if (number_type_ == "z1_cool_target") {
      parent_->set_z1_cool_target_temp(value);
    } else if (number_type_ == "z2_heat_target") {
      parent_->set_z2_heat_target_temp(value);
    } else if (number_type_ == "z2_cool_target") {
      parent_->set_z2_cool_target_temp(value);
    }
    
    this->publish_state(value);
  }

 protected:
  PanasonicAquarea *parent_{nullptr};
  std::string number_type_;
};

}  // namespace panasonic_aquarea
}  // namespace esphome
