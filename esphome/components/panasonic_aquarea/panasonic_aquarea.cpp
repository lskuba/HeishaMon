#include "panasonic_aquarea.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace panasonic_aquarea {

static const char *const TAG = "panasonic_aquarea";

// Initial query to start communication
const uint8_t PanasonicAquarea::initial_query_[INITIAL_QUERY_SIZE] = {
  0x31, 0x05, 0x10, 0x01, 0x00, 0x00, 0x00
};

// Query to request all data
const uint8_t PanasonicAquarea::panasonic_query_[PANASONIC_QUERY_SIZE] = {
  0x71, 0x6c, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Command template for sending settings
static const uint8_t PANASONIC_SEND_QUERY[PANASONIC_QUERY_SIZE] = {
  0xf1, 0x6c, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void PanasonicAquarea::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Panasonic Aquarea...");
  this->rx_buffer_.reserve(256);
  
  // Setup enable pin - HIGH enables the level shifter for TX
  if (this->enable_pin_ != nullptr) {
    this->enable_pin_->setup();
    this->enable_pin_->digital_write(true);  // Enable TX to heat pump
  }
}

void PanasonicAquarea::dump_config() {
  ESP_LOGCONFIG(TAG, "Panasonic Aquarea Heat Pump:");
  LOG_UPDATE_INTERVAL(this);
}

void PanasonicAquarea::loop() {
  // Read available data from UART
  while (this->available()) {
    uint8_t byte;
    this->read_byte(&byte);
    this->rx_buffer_.push_back(byte);
    this->last_rx_time_ = millis();
  }

  // Check if we have a complete message (timeout based)
  if (!this->rx_buffer_.empty() && (millis() - this->last_rx_time_ > 100)) {
    // Process received data
    if (this->rx_buffer_.size() >= DATASIZE) {
      this->decode_data_(this->rx_buffer_.data(), this->rx_buffer_.size());
      this->waiting_for_response_ = false;
    }
    this->rx_buffer_.clear();
  }
}

void PanasonicAquarea::update() {
  if (!this->initialized_) {
    ESP_LOGI(TAG, "Sending initial query to heat pump...");
    this->send_initial_query_();
    this->init_retry_count_++;
    if (this->init_retry_count_ > 5) {
      ESP_LOGW(TAG, "Heat pump not responding to initial query, trying regular query...");
      this->initialized_ = true;
    }
  } else {
    this->send_query_();
  }
}

void PanasonicAquarea::send_initial_query_() {
  uint8_t cmd[INITIAL_QUERY_SIZE + 1];
  memcpy(cmd, initial_query_, INITIAL_QUERY_SIZE);
  cmd[INITIAL_QUERY_SIZE] = this->calculate_checksum_(cmd, INITIAL_QUERY_SIZE);
  
  this->write_array(cmd, INITIAL_QUERY_SIZE + 1);
  this->waiting_for_response_ = true;
}

void PanasonicAquarea::send_query_() {
  uint8_t cmd[PANASONIC_QUERY_SIZE + 1];
  memcpy(cmd, panasonic_query_, PANASONIC_QUERY_SIZE);
  cmd[PANASONIC_QUERY_SIZE] = this->calculate_checksum_(cmd, PANASONIC_QUERY_SIZE);
  
  this->write_array(cmd, PANASONIC_QUERY_SIZE + 1);
  this->waiting_for_response_ = true;
}

void PanasonicAquarea::send_command_(uint8_t *cmd, size_t len) {
  cmd[len] = this->calculate_checksum_(cmd, len);
  this->write_array(cmd, len + 1);
}

uint8_t PanasonicAquarea::calculate_checksum_(uint8_t *data, size_t len) {
  uint8_t sum = 0;
  for (size_t i = 0; i < len; i++) {
    sum += data[i];
  }
  return (0x100 - sum) & 0xFF;
}

float PanasonicAquarea::decode_temperature_(uint8_t value) {
  return (float)(value) - 128.0f;
}

float PanasonicAquarea::decode_temperature_signed_(uint8_t value) {
  return (float)((int8_t)value - 128);
}

std::string PanasonicAquarea::decode_operating_mode_(uint8_t value) {
  uint8_t mode = value & 0x3F;
  switch (mode) {
    case 18: return "Heat only";
    case 19: return "Cool only";
    case 25: return "Auto (Heat)";
    case 33: return "DHW only";
    case 34: return "Heat + DHW";
    case 35: return "Cool + DHW";
    case 41: return "Auto (Heat) + DHW";
    case 26: return "Auto (Cool)";
    case 42: return "Auto (Cool) + DHW";
    default: return "Unknown";
  }
}

std::string PanasonicAquarea::decode_error_(uint8_t type, uint8_t number) {
  char error_str[16];
  int error_num = number - 17;
  switch (type) {
    case 177:  // 0xB1 = F type error
      sprintf(error_str, "F%02X", error_num);
      break;
    case 161:  // 0xA1 = H type error
      sprintf(error_str, "H%02X", error_num);
      break;
    default:
      strcpy(error_str, "No error");
      break;
  }
  return std::string(error_str);
}

void PanasonicAquarea::decode_data_(uint8_t *data, size_t len) {
  if (len < DATASIZE) {
    ESP_LOGW(TAG, "Data too short: %d bytes", len);
    return;
  }

  // Verify checksum
  uint8_t checksum = this->calculate_checksum_(data, DATASIZE);
  if (data[DATASIZE] != 0 && checksum != 0) {
    ESP_LOGW(TAG, "Checksum mismatch");
    // Continue anyway as HeishaMon does
  }

  // Mark as initialized on first successful decode
  if (!this->initialized_) {
    this->initialized_ = true;
    ESP_LOGI(TAG, "Heat pump communication established");
  }

  // Decode heatpump state (TOP0) - byte 4, bits 7-8
  this->heatpump_state_ = ((data[4] & 0x03) - 1) == 1;
  if (this->heatpump_state_sensor_ != nullptr)
    this->heatpump_state_sensor_->publish_state(this->heatpump_state_);

  // Decode operating mode (TOP4) - byte 6
  this->operating_mode_ = data[6] & 0x3F;
  if (this->operating_mode_sensor_ != nullptr) {
    this->operating_mode_sensor_->publish_state(this->decode_operating_mode_(data[6]));
  }

  // Decode force DHW state (TOP2) - byte 4, bits 1-2
  if (this->force_dhw_state_sensor_ != nullptr) {
    bool force_dhw = ((data[4] >> 6) - 1) == 1;
    this->force_dhw_state_sensor_->publish_state(force_dhw);
  }

  // Decode temperatures
  // Main Inlet Temp (TOP5) - byte 143
  this->inlet_temp_ = this->decode_temperature_(data[143]);
  if (this->inlet_temp_sensor_ != nullptr)
    this->inlet_temp_sensor_->publish_state(this->inlet_temp_);

  // Main Outlet Temp (TOP6) - byte 144
  this->outlet_temp_ = this->decode_temperature_(data[144]);
  if (this->outlet_temp_sensor_ != nullptr)
    this->outlet_temp_sensor_->publish_state(this->outlet_temp_);

  // Main Target Temp (TOP7) - byte 153
  float target_temp = this->decode_temperature_(data[153]);
  
  // Compressor Freq (TOP8) - byte 166
  if (this->compressor_freq_sensor_ != nullptr)
    this->compressor_freq_sensor_->publish_state((float)(data[166] - 1));

  // DHW Target Temp (TOP9) - byte 42
  this->dhw_target_temp_ = this->decode_temperature_(data[42]);
  if (this->dhw_target_temp_sensor_ != nullptr)
    this->dhw_target_temp_sensor_->publish_state(this->dhw_target_temp_);

  // DHW Temp (TOP10) - byte 141
  this->dhw_current_temp_ = this->decode_temperature_(data[141]);
  if (this->dhw_temp_sensor_ != nullptr)
    this->dhw_temp_sensor_->publish_state(this->dhw_current_temp_);

  // Operations Hours (TOP11) - bytes 182-183
  if (this->operations_hours_sensor_ != nullptr) {
    uint16_t hours = ((uint16_t)data[183] << 8) | data[182];
    this->operations_hours_sensor_->publish_state((float)(hours - 1));
  }

  // Operations Counter (TOP12) - bytes 179-180
  if (this->operations_counter_sensor_ != nullptr) {
    uint16_t counter = ((uint16_t)data[180] << 8) | data[179];
    this->operations_counter_sensor_->publish_state((float)(counter - 1));
  }

  // Outside Temp (TOP14) - byte 142
  this->outside_temp_ = this->decode_temperature_(data[142]);
  if (this->outside_temp_sensor_ != nullptr)
    this->outside_temp_sensor_->publish_state(this->outside_temp_);

  // Heat Power Production (TOP15) - byte 194
  if (this->heat_power_production_sensor_ != nullptr)
    this->heat_power_production_sensor_->publish_state((float)((data[194] - 1) * 200));

  // Heat Power Consumption (TOP16) - byte 193
  if (this->heat_power_consumption_sensor_ != nullptr)
    this->heat_power_consumption_sensor_->publish_state((float)((data[193] - 1) * 200));

  // Quiet Mode Level (TOP18) - byte 7, bits 3-5
  this->quiet_mode_ = ((data[7] >> 3) & 0x07) - 1;

  // 3-Way Valve State (TOP20) - byte 111
  if (this->threeway_valve_sensor_ != nullptr)
    this->threeway_valve_sensor_->publish_state((data[111] & 0x03) == 1);

  // Outside Pipe Temp (TOP21) - byte 158
  if (this->outside_pipe_temp_sensor_ != nullptr)
    this->outside_pipe_temp_sensor_->publish_state(this->decode_temperature_(data[158]));

  // Defrosting State (TOP26) - byte 111
  if (this->defrost_state_sensor_ != nullptr)
    this->defrost_state_sensor_->publish_state(((data[111] >> 2) & 0x03) == 1);

  // Z1 Heat Request Temp (TOP27) - byte 38
  this->z1_target_temp_ = this->decode_temperature_(data[38]);
  if (this->z1_target_temp_sensor_ != nullptr)
    this->z1_target_temp_sensor_->publish_state(this->z1_target_temp_);

  // Z1 Water Temp (TOP36) - byte 145
  this->z1_water_temp_ = this->decode_temperature_(data[145]);
  if (this->z1_water_temp_sensor_ != nullptr)
    this->z1_water_temp_sensor_->publish_state(this->z1_water_temp_);

  // Z2 Water Temp (TOP37) - byte 146
  this->z2_water_temp_ = this->decode_temperature_(data[146]);
  if (this->z2_water_temp_sensor_ != nullptr)
    this->z2_water_temp_sensor_->publish_state(this->z2_water_temp_);

  // DHW Power Production (TOP40) - byte 198
  if (this->dhw_power_production_sensor_ != nullptr)
    this->dhw_power_production_sensor_->publish_state((float)((data[198] - 1) * 200));

  // DHW Power Consumption (TOP41) - byte 197
  if (this->dhw_power_consumption_sensor_ != nullptr)
    this->dhw_power_consumption_sensor_->publish_state((float)((data[197] - 1) * 200));

  // Z1 Water Target Temp (TOP42) - byte 147
  if (this->z1_target_temp_sensor_ != nullptr)
    this->z1_target_temp_sensor_->publish_state(this->decode_temperature_(data[147]));

  // Z2 Water Target Temp (TOP43) - byte 148
  if (this->z2_target_temp_sensor_ != nullptr)
    this->z2_target_temp_sensor_->publish_state(this->decode_temperature_(data[148]));

  // Error (TOP44) - bytes 113-114
  if (this->error_sensor_ != nullptr) {
    this->error_sensor_->publish_state(this->decode_error_(data[113], data[114]));
  }

  // Discharge Temp (TOP50) - byte 155
  if (this->discharge_temp_sensor_ != nullptr)
    this->discharge_temp_sensor_->publish_state(this->decode_temperature_(data[155]));

  // Defrost Temp (TOP52) - byte 159
  if (this->defrost_temp_sensor_ != nullptr)
    this->defrost_temp_sensor_->publish_state(this->decode_temperature_(data[159]));

  // Eva Outlet Temp (TOP53) - byte 160
  if (this->eva_outlet_temp_sensor_ != nullptr)
    this->eva_outlet_temp_sensor_->publish_state(this->decode_temperature_(data[160]));

  // Bypass Outlet Temp (TOP54) - byte 161
  if (this->bypass_outlet_temp_sensor_ != nullptr)
    this->bypass_outlet_temp_sensor_->publish_state(this->decode_temperature_(data[161]));

  // IPM Temp (TOP55) - byte 162
  if (this->ipm_temp_sensor_ != nullptr)
    this->ipm_temp_sensor_->publish_state(this->decode_temperature_(data[162]));

  // Z1 Room Temp (TOP56) - byte 139
  this->z1_room_temp_ = this->decode_temperature_(data[139]);
  if (this->z1_room_temp_sensor_ != nullptr)
    this->z1_room_temp_sensor_->publish_state(this->z1_room_temp_);

  // Z2 Room Temp (TOP57) - byte 140
  this->z2_room_temp_ = this->decode_temperature_(data[140]);
  if (this->z2_room_temp_sensor_ != nullptr)
    this->z2_room_temp_sensor_->publish_state(this->z2_room_temp_);

  // DHW Heater State (TOP58) - byte 9
  if (this->dhw_heater_state_sensor_ != nullptr)
    this->dhw_heater_state_sensor_->publish_state(((data[9] >> 6) - 1) == 1);

  // Room Heater State (TOP59) - byte 9
  if (this->room_heater_state_sensor_ != nullptr)
    this->room_heater_state_sensor_->publish_state((((data[9] >> 4) & 0x03) - 1) == 1);

  // Internal Heater State (TOP60) - byte 112
  if (this->internal_heater_state_sensor_ != nullptr)
    this->internal_heater_state_sensor_->publish_state(((data[112] >> 6) - 1) == 1);

  // External Heater State (TOP61) - byte 112
  if (this->external_heater_state_sensor_ != nullptr)
    this->external_heater_state_sensor_->publish_state((((data[112] >> 4) & 0x03) - 1) == 1);

  // Fan1 Motor Speed (TOP62) - byte 173
  if (this->fan1_speed_sensor_ != nullptr)
    this->fan1_speed_sensor_->publish_state((float)((data[173] - 1) * 10));

  // Fan2 Motor Speed (TOP63) - byte 174
  if (this->fan2_speed_sensor_ != nullptr)
    this->fan2_speed_sensor_->publish_state((float)((data[174] - 1) * 10));

  // High Pressure (TOP64) - byte 163
  if (this->high_pressure_sensor_ != nullptr)
    this->high_pressure_sensor_->publish_state((float)(data[163] - 1) / 5.0f);

  // Pump Speed (TOP65) - byte 171
  if (this->pump_speed_sensor_ != nullptr)
    this->pump_speed_sensor_->publish_state((float)((data[171] - 1) * 50));

  // Low Pressure (TOP66) - byte 164
  if (this->low_pressure_sensor_ != nullptr)
    this->low_pressure_sensor_->publish_state((float)(data[164] - 1) / 5.0f);

  // Compressor Current (TOP67) - byte 165
  if (this->compressor_current_sensor_ != nullptr)
    this->compressor_current_sensor_->publish_state((float)(data[165] - 1) / 5.0f);

  // Pump Duty (TOP93) - byte 172
  if (this->pump_duty_sensor_ != nullptr)
    this->pump_duty_sensor_->publish_state((float)(data[172] - 1));

  // Pump Flow (TOP1) - bytes 169-170
  if (this->pump_flow_sensor_ != nullptr) {
    float flow = (float)data[170] + ((float)(data[169] - 1) / 256.0f);
    this->pump_flow_sensor_->publish_state(flow);
  }

  // Holiday Mode (TOP19) - byte 5
  if (this->holiday_mode_sensor_ != nullptr) {
    uint8_t holiday = ((data[5] >> 4) & 0x03) - 1;
    this->holiday_mode_sensor_->publish_state(holiday > 0);
  }

  // Heat Pump Model (TOP92) - bytes 129-138
  if (this->heatpump_model_sensor_ != nullptr) {
    char model[32];
    for (int i = 0; i < 10; i++) {
      sprintf(&model[i*3], "%02X ", data[129 + i]);
    }
    model[29] = '\0';
    this->heatpump_model_sensor_->publish_state(std::string(model));
  }

  // Update climate controllers
  if (this->zone1_climate_ != nullptr)
    this->zone1_climate_->update_state();
  if (this->dhw_climate_ != nullptr)
    this->dhw_climate_->update_state();
}

void PanasonicAquarea::set_heatpump_state(bool state) {
  uint8_t cmd[PANASONIC_QUERY_SIZE + 1];
  memcpy(cmd, PANASONIC_SEND_QUERY, PANASONIC_QUERY_SIZE);
  cmd[4] = state ? 2 : 1;  // 2 = on, 1 = off
  ESP_LOGI(TAG, "Setting heatpump state to %s", state ? "ON" : "OFF");
  this->send_command_(cmd, PANASONIC_QUERY_SIZE);
}

void PanasonicAquarea::set_operating_mode(uint8_t mode) {
  uint8_t cmd[PANASONIC_QUERY_SIZE + 1];
  memcpy(cmd, PANASONIC_SEND_QUERY, PANASONIC_QUERY_SIZE);
  
  uint8_t mode_value;
  switch (mode) {
    case 0: mode_value = 18; break;  // Heat only
    case 1: mode_value = 19; break;  // Cool only
    case 2: mode_value = 24; break;  // Auto
    case 3: mode_value = 33; break;  // DHW only
    case 4: mode_value = 34; break;  // Heat + DHW
    case 5: mode_value = 35; break;  // Cool + DHW
    case 6: mode_value = 40; break;  // Auto + DHW
    default: mode_value = 34; break; // Default to Heat + DHW
  }
  
  cmd[6] = mode_value;
  ESP_LOGI(TAG, "Setting operating mode to %d (raw: %d)", mode, mode_value);
  this->send_command_(cmd, PANASONIC_QUERY_SIZE);
}

void PanasonicAquarea::set_dhw_target_temp(float temp) {
  uint8_t cmd[PANASONIC_QUERY_SIZE + 1];
  memcpy(cmd, PANASONIC_SEND_QUERY, PANASONIC_QUERY_SIZE);
  cmd[42] = (uint8_t)(temp + 128);
  ESP_LOGI(TAG, "Setting DHW target temperature to %.1f°C", temp);
  this->send_command_(cmd, PANASONIC_QUERY_SIZE);
}

void PanasonicAquarea::set_z1_heat_target_temp(float temp) {
  uint8_t cmd[PANASONIC_QUERY_SIZE + 1];
  memcpy(cmd, PANASONIC_SEND_QUERY, PANASONIC_QUERY_SIZE);
  cmd[38] = (uint8_t)(temp + 128);
  ESP_LOGI(TAG, "Setting Z1 heat target temperature to %.1f°C", temp);
  this->send_command_(cmd, PANASONIC_QUERY_SIZE);
}

void PanasonicAquarea::set_z1_cool_target_temp(float temp) {
  uint8_t cmd[PANASONIC_QUERY_SIZE + 1];
  memcpy(cmd, PANASONIC_SEND_QUERY, PANASONIC_QUERY_SIZE);
  cmd[39] = (uint8_t)(temp + 128);
  ESP_LOGI(TAG, "Setting Z1 cool target temperature to %.1f°C", temp);
  this->send_command_(cmd, PANASONIC_QUERY_SIZE);
}

void PanasonicAquarea::set_z2_heat_target_temp(float temp) {
  uint8_t cmd[PANASONIC_QUERY_SIZE + 1];
  memcpy(cmd, PANASONIC_SEND_QUERY, PANASONIC_QUERY_SIZE);
  cmd[40] = (uint8_t)(temp + 128);
  ESP_LOGI(TAG, "Setting Z2 heat target temperature to %.1f°C", temp);
  this->send_command_(cmd, PANASONIC_QUERY_SIZE);
}

void PanasonicAquarea::set_z2_cool_target_temp(float temp) {
  uint8_t cmd[PANASONIC_QUERY_SIZE + 1];
  memcpy(cmd, PANASONIC_SEND_QUERY, PANASONIC_QUERY_SIZE);
  cmd[41] = (uint8_t)(temp + 128);
  ESP_LOGI(TAG, "Setting Z2 cool target temperature to %.1f°C", temp);
  this->send_command_(cmd, PANASONIC_QUERY_SIZE);
}

void PanasonicAquarea::set_quiet_mode(uint8_t level) {
  uint8_t cmd[PANASONIC_QUERY_SIZE + 1];
  memcpy(cmd, PANASONIC_SEND_QUERY, PANASONIC_QUERY_SIZE);
  cmd[7] = (level + 1) * 8;
  ESP_LOGI(TAG, "Setting quiet mode level to %d", level);
  this->send_command_(cmd, PANASONIC_QUERY_SIZE);
}

void PanasonicAquarea::set_powerful_mode(uint8_t time) {
  uint8_t cmd[PANASONIC_QUERY_SIZE + 1];
  memcpy(cmd, PANASONIC_SEND_QUERY, PANASONIC_QUERY_SIZE);
  cmd[7] = (time + 1) & 0x07;
  ESP_LOGI(TAG, "Setting powerful mode time to %d (x30min)", time);
  this->send_command_(cmd, PANASONIC_QUERY_SIZE);
}

void PanasonicAquarea::set_force_dhw(bool state) {
  uint8_t cmd[PANASONIC_QUERY_SIZE + 1];
  memcpy(cmd, PANASONIC_SEND_QUERY, PANASONIC_QUERY_SIZE);
  cmd[4] = state ? 0x80 : 0x40;  // 0x80 = force on, 0x40 = force off
  ESP_LOGI(TAG, "Setting force DHW to %s", state ? "ON" : "OFF");
  this->send_command_(cmd, PANASONIC_QUERY_SIZE);
}

void PanasonicAquarea::set_holiday_mode(bool state) {
  uint8_t cmd[PANASONIC_QUERY_SIZE + 1];
  memcpy(cmd, PANASONIC_SEND_QUERY, PANASONIC_QUERY_SIZE);
  cmd[5] = state ? 0x20 : 0x10;  // 0x20 = on, 0x10 = off
  ESP_LOGI(TAG, "Setting holiday mode to %s", state ? "ON" : "OFF");
  this->send_command_(cmd, PANASONIC_QUERY_SIZE);
}

void PanasonicAquarea::set_force_defrost(bool state) {
  uint8_t cmd[PANASONIC_QUERY_SIZE + 1];
  memcpy(cmd, PANASONIC_SEND_QUERY, PANASONIC_QUERY_SIZE);
  cmd[8] = state ? 0x02 : 0x00;
  ESP_LOGI(TAG, "Setting force defrost to %s", state ? "ON" : "OFF");
  this->send_command_(cmd, PANASONIC_QUERY_SIZE);
}

void PanasonicAquarea::set_force_sterilization(bool state) {
  uint8_t cmd[PANASONIC_QUERY_SIZE + 1];
  memcpy(cmd, PANASONIC_SEND_QUERY, PANASONIC_QUERY_SIZE);
  cmd[8] = state ? 0x04 : 0x00;
  ESP_LOGI(TAG, "Setting force sterilization to %s", state ? "ON" : "OFF");
  this->send_command_(cmd, PANASONIC_QUERY_SIZE);
}

// ============ Zone Climate Implementation ============

void AquareaClimate::setup() {
  // Initial state
  this->mode = climate::CLIMATE_MODE_OFF;
  this->action = climate::CLIMATE_ACTION_IDLE;
}

climate::ClimateTraits AquareaClimate::traits() {
  auto traits = climate::ClimateTraits();
  traits.set_supports_current_temperature(true);
  traits.set_supports_two_point_target_temperature(false);
  traits.set_visual_min_temperature(15);
  traits.set_visual_max_temperature(55);
  traits.set_visual_temperature_step(0.5);
  
  traits.set_supported_modes({
    climate::CLIMATE_MODE_OFF,
    climate::CLIMATE_MODE_HEAT,
    climate::CLIMATE_MODE_COOL,
    climate::CLIMATE_MODE_HEAT_COOL
  });
  
  traits.set_supported_presets({
    climate::CLIMATE_PRESET_NONE,
    climate::CLIMATE_PRESET_ECO,
    climate::CLIMATE_PRESET_BOOST
  });
  
  return traits;
}

void AquareaClimate::control(const climate::ClimateCall &call) {
  if (this->parent_ == nullptr) return;

  if (call.get_mode().has_value()) {
    climate::ClimateMode mode = *call.get_mode();
    switch (mode) {
      case climate::CLIMATE_MODE_OFF:
        this->parent_->set_heatpump_state(false);
        break;
      case climate::CLIMATE_MODE_HEAT:
        this->parent_->set_heatpump_state(true);
        this->parent_->set_operating_mode(4);  // Heat + DHW
        break;
      case climate::CLIMATE_MODE_COOL:
        this->parent_->set_heatpump_state(true);
        this->parent_->set_operating_mode(5);  // Cool + DHW
        break;
      case climate::CLIMATE_MODE_HEAT_COOL:
        this->parent_->set_heatpump_state(true);
        this->parent_->set_operating_mode(6);  // Auto + DHW
        break;
      default:
        break;
    }
    this->mode = mode;
  }

  if (call.get_target_temperature().has_value()) {
    float temp = *call.get_target_temperature();
    if (this->mode == climate::CLIMATE_MODE_COOL) {
      this->parent_->set_z1_cool_target_temp(temp);
    } else {
      this->parent_->set_z1_heat_target_temp(temp);
    }
    this->target_temperature = temp;
  }

  if (call.get_preset().has_value()) {
    climate::ClimatePreset preset = *call.get_preset();
    switch (preset) {
      case climate::CLIMATE_PRESET_ECO:
        this->parent_->set_quiet_mode(3);  // Maximum quiet
        break;
      case climate::CLIMATE_PRESET_BOOST:
        this->parent_->set_powerful_mode(3);  // 90 min powerful
        break;
      default:
        this->parent_->set_quiet_mode(0);  // Off
        break;
    }
    this->preset = preset;
  }

  this->publish_state();
}

void AquareaClimate::update_state() {
  if (this->parent_ == nullptr) return;

  // Update current temperature from zone
  this->current_temperature = this->parent_->get_z1_water_temp();
  this->target_temperature = this->parent_->get_z1_target_temp();

  // Update mode based on heatpump state and operating mode
  if (!this->parent_->is_heatpump_on()) {
    this->mode = climate::CLIMATE_MODE_OFF;
    this->action = climate::CLIMATE_ACTION_OFF;
  } else {
    uint8_t op_mode = this->parent_->get_operating_mode();
    switch (op_mode) {
      case 18:  // Heat only
      case 34:  // Heat + DHW
        this->mode = climate::CLIMATE_MODE_HEAT;
        break;
      case 19:  // Cool only
      case 35:  // Cool + DHW
        this->mode = climate::CLIMATE_MODE_COOL;
        break;
      case 25:  // Auto (Heat)
      case 26:  // Auto (Cool)
      case 41:  // Auto (Heat) + DHW
      case 42:  // Auto (Cool) + DHW
        this->mode = climate::CLIMATE_MODE_HEAT_COOL;
        break;
      default:
        this->mode = climate::CLIMATE_MODE_HEAT;
        break;
    }

    // Determine action
    if (this->current_temperature < this->target_temperature - 0.5f) {
      this->action = (this->mode == climate::CLIMATE_MODE_COOL) 
                     ? climate::CLIMATE_ACTION_IDLE 
                     : climate::CLIMATE_ACTION_HEATING;
    } else if (this->current_temperature > this->target_temperature + 0.5f) {
      this->action = (this->mode == climate::CLIMATE_MODE_HEAT) 
                     ? climate::CLIMATE_ACTION_IDLE 
                     : climate::CLIMATE_ACTION_COOLING;
    } else {
      this->action = climate::CLIMATE_ACTION_IDLE;
    }
  }

  this->publish_state();
}

// ============ DHW Climate Implementation ============

void AquareaDHWClimate::setup() {
  this->mode = climate::CLIMATE_MODE_OFF;
  this->action = climate::CLIMATE_ACTION_IDLE;
}

climate::ClimateTraits AquareaDHWClimate::traits() {
  auto traits = climate::ClimateTraits();
  traits.set_supports_current_temperature(true);
  traits.set_supports_two_point_target_temperature(false);
  traits.set_visual_min_temperature(40);
  traits.set_visual_max_temperature(75);
  traits.set_visual_temperature_step(1);
  
  traits.set_supported_modes({
    climate::CLIMATE_MODE_OFF,
    climate::CLIMATE_MODE_HEAT
  });
  
  traits.set_supported_presets({
    climate::CLIMATE_PRESET_NONE,
    climate::CLIMATE_PRESET_BOOST
  });
  
  return traits;
}

void AquareaDHWClimate::control(const climate::ClimateCall &call) {
  if (this->parent_ == nullptr) return;

  if (call.get_mode().has_value()) {
    climate::ClimateMode mode = *call.get_mode();
    if (mode == climate::CLIMATE_MODE_OFF) {
      // Set to heat only mode (no DHW)
      this->parent_->set_operating_mode(0);  // Heat only
    } else {
      // Include DHW in operation
      this->parent_->set_operating_mode(4);  // Heat + DHW
      this->parent_->set_heatpump_state(true);
    }
    this->mode = mode;
  }

  if (call.get_target_temperature().has_value()) {
    float temp = *call.get_target_temperature();
    this->parent_->set_dhw_target_temp(temp);
    this->target_temperature = temp;
  }

  if (call.get_preset().has_value()) {
    climate::ClimatePreset preset = *call.get_preset();
    if (preset == climate::CLIMATE_PRESET_BOOST) {
      this->parent_->set_force_dhw(true);
    } else {
      this->parent_->set_force_dhw(false);
    }
    this->preset = preset;
  }

  this->publish_state();
}

void AquareaDHWClimate::update_state() {
  if (this->parent_ == nullptr) return;

  this->current_temperature = this->parent_->get_dhw_current_temp();
  this->target_temperature = this->parent_->get_dhw_target_temp();

  // Check if DHW is included in operating mode
  uint8_t op_mode = this->parent_->get_operating_mode();
  bool dhw_active = (op_mode == 33 || op_mode == 34 || op_mode == 35 || 
                     op_mode == 41 || op_mode == 42);

  if (!this->parent_->is_heatpump_on() || !dhw_active) {
    this->mode = climate::CLIMATE_MODE_OFF;
    this->action = climate::CLIMATE_ACTION_OFF;
  } else {
    this->mode = climate::CLIMATE_MODE_HEAT;
    
    if (this->current_temperature < this->target_temperature - 2.0f) {
      this->action = climate::CLIMATE_ACTION_HEATING;
    } else {
      this->action = climate::CLIMATE_ACTION_IDLE;
    }
  }

  this->publish_state();
}

}  // namespace panasonic_aquarea
}  // namespace esphome
