# HeishaMon ESPHome Configuration

This is an ESPHome-based implementation of HeishaMon for controlling Panasonic Aquarea heat pumps.

## Features

- **Two Climate Entities:**
  - **House Temperature (Zone 1)**: Controls heating/cooling for your home
  - **Hot Water (DHW)**: Controls domestic hot water temperature

- **Full Sensor Support:**
  - All temperatures (inlet, outlet, DHW, zones, outside, compressor, etc.)
  - Power consumption and production
  - Compressor frequency and current
  - Pump flow and speed
  - Pressure readings
  - Operation hours and counters

- **Control Options:**
  - Operating mode selection (Heat, Cool, Auto, DHW, combinations)
  - Quiet mode levels
  - Powerful mode timing
  - Force DHW
  - Holiday mode
  - Force defrost/sterilization

## Hardware Requirements

### ESP8266 D1 Mini

- **Board**: Wemos D1 Mini (ESP8266)
- **Level Shifter**: Required for 3.3V <-> 5V conversion

### Wiring

| D1 Mini Pin | Function | Heat Pump Connection |
|-------------|----------|---------------------|
| GPIO1 (TX)  | Serial TX | RX (via level shifter) |
| GPIO3 (RX)  | Serial RX | TX (via level shifter) |
| GPIO5 (D1)  | Enable   | Enable pin (active low) |
| 3.3V        | Power    | Level shifter LV |
| 5V          | Power    | Heat pump & level shifter HV |
| GND         | Ground   | Common ground |

### Serial Settings

- **Baud rate**: 9600
- **Data bits**: 8
- **Parity**: Even
- **Stop bits**: 1

## Installation

### 1. Prepare Secrets File

```bash
cd esphome
cp secrets.yaml.template secrets.yaml
# Edit secrets.yaml with your WiFi credentials and passwords
```

### 2. Generate API Encryption Key

```bash
esphome wizard heishamon.yaml
# Or use: openssl rand -base64 32
```

### 3. Compile and Upload

```bash
# First time - via USB
esphome run heishamon.yaml

# Subsequent updates - via OTA
esphome run heishamon.yaml --device heishamon.local
```

## Configuration Structure

```
esphome/
├── heishamon.yaml           # Main configuration
├── secrets.yaml             # Your secrets (not in git)
├── secrets.yaml.template    # Template for secrets
└── components/
    └── panasonic_aquarea/   # Custom component
        ├── __init__.py
        ├── panasonic_aquarea.h
        ├── panasonic_aquarea.cpp
        ├── sensor.py
        ├── binary_sensor.py
        ├── text_sensor.py
        ├── climate.py
        ├── switch.py
        ├── select.py
        ├── number.py
        └── aquarea_*.h      # Helper classes
```

## Home Assistant Integration

Once installed and connected to WiFi, the device will automatically appear in Home Assistant through ESPHome integration.

### Available Entities

#### Climate
- `climate.house_temperature` - Zone 1 heating/cooling control
- `climate.hot_water` - DHW temperature control

#### Switches
- `switch.heatpump_power` - Turn heat pump on/off
- `switch.force_dhw` - Force DHW heating
- `switch.holiday_mode` - Enable holiday mode
- `switch.force_defrost` - Trigger defrost cycle
- `switch.force_sterilization` - Trigger legionella prevention

#### Selects
- `select.operating_mode` - Change operating mode
- `select.quiet_mode` - Set quiet mode level
- `select.powerful_mode` - Set powerful mode duration

#### Numbers
- `number.dhw_target_temperature` - Set DHW target (40-75°C)
- `number.zone_1_heat_target_temperature` - Set heating target
- `number.zone_1_cool_target_temperature` - Set cooling target

## Troubleshooting

### No Communication with Heat Pump

1. Check wiring connections
2. Verify level shifter is working (5V on HV side, 3.3V on LV side)
3. Check enable pin (GPIO5) is connected
4. Verify serial settings (9600, 8, Even, 1)

### Wrong Temperature Readings

- Some heat pump models use different byte offsets
- Check logs for raw data values

### Climate Entity Not Updating

- The update interval is 5 seconds by default
- Check the heat pump is responding (see binary sensor for running state)

## Protocol Documentation

This implementation is based on the HeishaMon protocol. See:
- [MQTT-Topics.md](../MQTT-Topics.md) - All available topics
- [ProtocolByteDecrypt.md](../ProtocolByteDecrypt.md) - Protocol byte definitions
- [commands.cpp](../HeishaMon/commands.cpp) - Command implementations

## License

This project is based on HeishaMon and follows the same license terms.

## Credits

- Original HeishaMon project: https://github.com/Egyras/HeishaMon
- ESPHome: https://esphome.io
