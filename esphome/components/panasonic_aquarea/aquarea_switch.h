#pragma once

#include "esphome/components/switch/switch.h"
#include "panasonic_aquarea.h"

namespace esphome {
namespace panasonic_aquarea {

class AquareaSwitch : public switch_::Switch, public Component {
 public:
  void set_parent(PanasonicAquarea *parent) { parent_ = parent; }
  void set_switch_type(const std::string &type) { switch_type_ = type; }
  
  void setup() override {}
  
  void write_state(bool state) override {
    if (parent_ == nullptr) return;
    
    if (switch_type_ == "heatpump") {
      parent_->set_heatpump_state(state);
    } else if (switch_type_ == "force_dhw") {
      parent_->set_force_dhw(state);
    } else if (switch_type_ == "holiday_mode") {
      parent_->set_holiday_mode(state);
    } else if (switch_type_ == "force_defrost") {
      parent_->set_force_defrost(state);
    } else if (switch_type_ == "force_sterilization") {
      parent_->set_force_sterilization(state);
    }
    
    this->publish_state(state);
  }

 protected:
  PanasonicAquarea *parent_{nullptr};
  std::string switch_type_;
};

}  // namespace panasonic_aquarea
}  // namespace esphome
