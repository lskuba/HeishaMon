#pragma once

#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/select/select.h"
#include "esphome/components/number/number.h"
#include "esphome/components/climate/climate.h"
#include <vector>
#include <string>

namespace esphome {
namespace panasonic_aquarea {

// Forward declarations
class AquareaClimate;
class AquareaDHWClimate;
class AquareaSwitch;
class AquareaSelect;
class AquareaNumber;

static const uint8_t DATASIZE = 203;
static const uint8_t PANASONIC_QUERY_SIZE = 110;
static const uint8_t INITIAL_QUERY_SIZE = 7;

// Operating modes
enum OperatingMode : uint8_t {
  MODE_HEAT_ONLY = 0,
  MODE_COOL_ONLY = 1,
  MODE_AUTO_HEAT = 2,
  MODE_DHW_ONLY = 3,
  MODE_HEAT_DHW = 4,
  MODE_COOL_DHW = 5,
  MODE_AUTO_HEAT_DHW = 6,
  MODE_AUTO_COOL = 7,
  MODE_AUTO_COOL_DHW = 8
};

class PanasonicAquarea : public PollingComponent, public uart::UARTDevice {
 public:
  PanasonicAquarea() = default;

  void setup() override;
  void loop() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  // Sensor setters
  void set_outside_temp_sensor(sensor::Sensor *sensor) { outside_temp_sensor_ = sensor; }
  void set_inlet_temp_sensor(sensor::Sensor *sensor) { inlet_temp_sensor_ = sensor; }
  void set_outlet_temp_sensor(sensor::Sensor *sensor) { outlet_temp_sensor_ = sensor; }
  void set_dhw_temp_sensor(sensor::Sensor *sensor) { dhw_temp_sensor_ = sensor; }
  void set_dhw_target_temp_sensor(sensor::Sensor *sensor) { dhw_target_temp_sensor_ = sensor; }
  void set_compressor_freq_sensor(sensor::Sensor *sensor) { compressor_freq_sensor_ = sensor; }
  void set_pump_flow_sensor(sensor::Sensor *sensor) { pump_flow_sensor_ = sensor; }
  void set_z1_water_temp_sensor(sensor::Sensor *sensor) { z1_water_temp_sensor_ = sensor; }
  void set_z2_water_temp_sensor(sensor::Sensor *sensor) { z2_water_temp_sensor_ = sensor; }
  void set_z1_target_temp_sensor(sensor::Sensor *sensor) { z1_target_temp_sensor_ = sensor; }
  void set_z2_target_temp_sensor(sensor::Sensor *sensor) { z2_target_temp_sensor_ = sensor; }
  void set_z1_room_temp_sensor(sensor::Sensor *sensor) { z1_room_temp_sensor_ = sensor; }
  void set_z2_room_temp_sensor(sensor::Sensor *sensor) { z2_room_temp_sensor_ = sensor; }
  void set_heat_power_production_sensor(sensor::Sensor *sensor) { heat_power_production_sensor_ = sensor; }
  void set_heat_power_consumption_sensor(sensor::Sensor *sensor) { heat_power_consumption_sensor_ = sensor; }
  void set_dhw_power_production_sensor(sensor::Sensor *sensor) { dhw_power_production_sensor_ = sensor; }
  void set_dhw_power_consumption_sensor(sensor::Sensor *sensor) { dhw_power_consumption_sensor_ = sensor; }
  void set_operations_hours_sensor(sensor::Sensor *sensor) { operations_hours_sensor_ = sensor; }
  void set_operations_counter_sensor(sensor::Sensor *sensor) { operations_counter_sensor_ = sensor; }
  void set_discharge_temp_sensor(sensor::Sensor *sensor) { discharge_temp_sensor_ = sensor; }
  void set_outside_pipe_temp_sensor(sensor::Sensor *sensor) { outside_pipe_temp_sensor_ = sensor; }
  void set_defrost_temp_sensor(sensor::Sensor *sensor) { defrost_temp_sensor_ = sensor; }
  void set_eva_outlet_temp_sensor(sensor::Sensor *sensor) { eva_outlet_temp_sensor_ = sensor; }
  void set_bypass_outlet_temp_sensor(sensor::Sensor *sensor) { bypass_outlet_temp_sensor_ = sensor; }
  void set_ipm_temp_sensor(sensor::Sensor *sensor) { ipm_temp_sensor_ = sensor; }
  void set_high_pressure_sensor(sensor::Sensor *sensor) { high_pressure_sensor_ = sensor; }
  void set_low_pressure_sensor(sensor::Sensor *sensor) { low_pressure_sensor_ = sensor; }
  void set_compressor_current_sensor(sensor::Sensor *sensor) { compressor_current_sensor_ = sensor; }
  void set_pump_speed_sensor(sensor::Sensor *sensor) { pump_speed_sensor_ = sensor; }
  void set_pump_duty_sensor(sensor::Sensor *sensor) { pump_duty_sensor_ = sensor; }
  void set_fan1_speed_sensor(sensor::Sensor *sensor) { fan1_speed_sensor_ = sensor; }
  void set_fan2_speed_sensor(sensor::Sensor *sensor) { fan2_speed_sensor_ = sensor; }
  
  // Binary sensor setters
  void set_heatpump_state_sensor(binary_sensor::BinarySensor *sensor) { heatpump_state_sensor_ = sensor; }
  void set_defrost_state_sensor(binary_sensor::BinarySensor *sensor) { defrost_state_sensor_ = sensor; }
  void set_dhw_heater_state_sensor(binary_sensor::BinarySensor *sensor) { dhw_heater_state_sensor_ = sensor; }
  void set_room_heater_state_sensor(binary_sensor::BinarySensor *sensor) { room_heater_state_sensor_ = sensor; }
  void set_internal_heater_state_sensor(binary_sensor::BinarySensor *sensor) { internal_heater_state_sensor_ = sensor; }
  void set_external_heater_state_sensor(binary_sensor::BinarySensor *sensor) { external_heater_state_sensor_ = sensor; }
  void set_force_dhw_state_sensor(binary_sensor::BinarySensor *sensor) { force_dhw_state_sensor_ = sensor; }
  void set_holiday_mode_sensor(binary_sensor::BinarySensor *sensor) { holiday_mode_sensor_ = sensor; }
  void set_threeway_valve_sensor(binary_sensor::BinarySensor *sensor) { threeway_valve_sensor_ = sensor; }
  
  // Text sensor setters
  void set_error_sensor(text_sensor::TextSensor *sensor) { error_sensor_ = sensor; }
  void set_operating_mode_sensor(text_sensor::TextSensor *sensor) { operating_mode_sensor_ = sensor; }
  void set_heatpump_model_sensor(text_sensor::TextSensor *sensor) { heatpump_model_sensor_ = sensor; }

  // Climate setters
  void set_zone1_climate(AquareaClimate *climate) { zone1_climate_ = climate; }
  void set_dhw_climate(AquareaDHWClimate *climate) { dhw_climate_ = climate; }
  void set_enable_pin(GPIOPin *pin) { enable_pin_ = pin; }

  // Commands
  void set_heatpump_state(bool state);
  void set_operating_mode(uint8_t mode);
  void set_dhw_target_temp(float temp);
  void set_z1_heat_target_temp(float temp);
  void set_z1_cool_target_temp(float temp);
  void set_z2_heat_target_temp(float temp);
  void set_z2_cool_target_temp(float temp);
  void set_quiet_mode(uint8_t level);
  void set_powerful_mode(uint8_t time);
  void set_force_dhw(bool state);
  void set_holiday_mode(bool state);
  void set_force_defrost(bool state);
  void set_force_sterilization(bool state);

  // State getters
  bool is_heatpump_on() const { return heatpump_state_; }
  uint8_t get_operating_mode() const { return operating_mode_; }
  float get_dhw_target_temp() const { return dhw_target_temp_; }
  float get_dhw_current_temp() const { return dhw_current_temp_; }
  float get_z1_target_temp() const { return z1_target_temp_; }
  float get_z1_current_temp() const { return z1_room_temp_; }
  float get_z1_water_temp() const { return z1_water_temp_; }
  float get_outside_temp() const { return outside_temp_; }

 protected:
  void send_command_(uint8_t *cmd, size_t len);
  void send_query_();
  void send_initial_query_();
  bool read_response_(uint8_t *data, size_t &len);
  void decode_data_(uint8_t *data, size_t len);
  uint8_t calculate_checksum_(uint8_t *data, size_t len);
  float decode_temperature_(uint8_t value);
  float decode_temperature_signed_(uint8_t value);
  std::string decode_operating_mode_(uint8_t value);
  std::string decode_error_(uint8_t type, uint8_t number);
  
  // Enable pin for level shifter
  GPIOPin *enable_pin_{nullptr};
  
  // Buffer for serial data
  std::vector<uint8_t> rx_buffer_;
  uint32_t last_rx_time_{0};
  uint32_t query_sent_time_{0};
  bool waiting_for_response_{false};
  bool initialized_{false};
  uint8_t init_retry_count_{0};
  uint8_t consecutive_timeouts_{0};
  uint32_t next_query_time_{0};

  // Decoded values
  bool heatpump_state_{false};
  uint8_t operating_mode_{0};
  float outside_temp_{0};
  float inlet_temp_{0};
  float outlet_temp_{0};
  float dhw_target_temp_{0};
  float dhw_current_temp_{0};
  float z1_target_temp_{0};
  float z1_water_temp_{0};
  float z1_room_temp_{0};
  float z2_target_temp_{0};
  float z2_water_temp_{0};
  float z2_room_temp_{0};
  uint8_t quiet_mode_{0};

  // Sensors
  sensor::Sensor *outside_temp_sensor_{nullptr};
  sensor::Sensor *inlet_temp_sensor_{nullptr};
  sensor::Sensor *outlet_temp_sensor_{nullptr};
  sensor::Sensor *dhw_temp_sensor_{nullptr};
  sensor::Sensor *dhw_target_temp_sensor_{nullptr};
  sensor::Sensor *compressor_freq_sensor_{nullptr};
  sensor::Sensor *pump_flow_sensor_{nullptr};
  sensor::Sensor *z1_water_temp_sensor_{nullptr};
  sensor::Sensor *z2_water_temp_sensor_{nullptr};
  sensor::Sensor *z1_target_temp_sensor_{nullptr};
  sensor::Sensor *z2_target_temp_sensor_{nullptr};
  sensor::Sensor *z1_room_temp_sensor_{nullptr};
  sensor::Sensor *z2_room_temp_sensor_{nullptr};
  sensor::Sensor *heat_power_production_sensor_{nullptr};
  sensor::Sensor *heat_power_consumption_sensor_{nullptr};
  sensor::Sensor *dhw_power_production_sensor_{nullptr};
  sensor::Sensor *dhw_power_consumption_sensor_{nullptr};
  sensor::Sensor *operations_hours_sensor_{nullptr};
  sensor::Sensor *operations_counter_sensor_{nullptr};
  sensor::Sensor *discharge_temp_sensor_{nullptr};
  sensor::Sensor *outside_pipe_temp_sensor_{nullptr};
  sensor::Sensor *defrost_temp_sensor_{nullptr};
  sensor::Sensor *eva_outlet_temp_sensor_{nullptr};
  sensor::Sensor *bypass_outlet_temp_sensor_{nullptr};
  sensor::Sensor *ipm_temp_sensor_{nullptr};
  sensor::Sensor *high_pressure_sensor_{nullptr};
  sensor::Sensor *low_pressure_sensor_{nullptr};
  sensor::Sensor *compressor_current_sensor_{nullptr};
  sensor::Sensor *pump_speed_sensor_{nullptr};
  sensor::Sensor *pump_duty_sensor_{nullptr};
  sensor::Sensor *fan1_speed_sensor_{nullptr};
  sensor::Sensor *fan2_speed_sensor_{nullptr};
  
  // Binary sensors
  binary_sensor::BinarySensor *heatpump_state_sensor_{nullptr};
  binary_sensor::BinarySensor *defrost_state_sensor_{nullptr};
  binary_sensor::BinarySensor *dhw_heater_state_sensor_{nullptr};
  binary_sensor::BinarySensor *room_heater_state_sensor_{nullptr};
  binary_sensor::BinarySensor *internal_heater_state_sensor_{nullptr};
  binary_sensor::BinarySensor *external_heater_state_sensor_{nullptr};
  binary_sensor::BinarySensor *force_dhw_state_sensor_{nullptr};
  binary_sensor::BinarySensor *holiday_mode_sensor_{nullptr};
  binary_sensor::BinarySensor *threeway_valve_sensor_{nullptr};
  
  // Text sensors
  text_sensor::TextSensor *error_sensor_{nullptr};
  text_sensor::TextSensor *operating_mode_sensor_{nullptr};
  text_sensor::TextSensor *heatpump_model_sensor_{nullptr};

  // Climate controllers
  AquareaClimate *zone1_climate_{nullptr};
  AquareaDHWClimate *dhw_climate_{nullptr};

  // Command buffer
  static const uint8_t initial_query_[INITIAL_QUERY_SIZE];
  static const uint8_t panasonic_query_[PANASONIC_QUERY_SIZE];
};

// Zone Climate (for house heating/cooling)
class AquareaClimate : public climate::Climate, public Component {
 public:
  void setup() override;
  void set_parent(PanasonicAquarea *parent) { parent_ = parent; }
  
  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;
  void update_state();

 protected:
  PanasonicAquarea *parent_{nullptr};
};

// DHW Climate (for hot water)
class AquareaDHWClimate : public climate::Climate, public Component {
 public:
  void setup() override;
  void set_parent(PanasonicAquarea *parent) { parent_ = parent; }
  
  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;
  void update_state();

 protected:
  PanasonicAquarea *parent_{nullptr};
};

}  // namespace panasonic_aquarea
}  // namespace esphome
