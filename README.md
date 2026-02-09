# HeishaMon ESPHome

ESPHome implementation for Panasonic Aquarea heat pumps, based on the HeishaMon protocol.

## Features

- **Two Climate Entities**: Zone 1 heating/cooling and DHW (Domestic Hot Water)
- **Comprehensive Sensors**: All heat pump parameters including temperatures, pressures, flow rates, power consumption
- **Binary Sensors**: Operating states (defrost, pumps, heaters, holiday mode, etc.)
- **Controls**: Switches for operating modes, quiet mode, powerful mode, force DHW
- **Selects**: Zone operating modes (heat/cool/auto)
- **Numbers**: Target temperature setpoints for heating, cooling, and DHW

## Hardware Requirements

### ESP8266 D1 Mini
- **GPIO13 (D7)**: RX - connected to heat pump TX via level shifter
- **GPIO15 (D8)**: TX - connected to heat pump RX via level shifter
- **GPIO5 (D1)**: Enable pin for level shifter (active HIGH)

### Level Shifter
**Required!** The heat pump uses 5V TTL logic while ESP8266 uses 3.3V. Use a bidirectional level shifter for safe communication.

### Connections
```
ESP8266 D1 Mini          Level Shifter       Panasonic Aquarea
                         (3.3V <-> 5V)
-----------------        -------------       -----------------
     3.3V    ---------->    LV          
     GND     ---------->    GND     <------>      GND
   GPIO13(RX)----------> LV1     HV1 <------      TX
   GPIO15(TX)----------> LV2     HV2 ------>      RX
     5V      ---------->             HV
   GPIO5(EN) -------> (Enable MOSFET for TX)
```

## Installation

### 1. Clone Repository
```bash
git clone https://github.com/lskuba/HeishaMon.git
cd HeishaMon/esphome
```

### 2. Configure Secrets
Edit `secrets.yaml`:
```yaml
wifi_ssid: "YourWiFiSSID"
wifi_password: "YourWiFiPassword"
ap_password: "FallbackAPPassword"
ota_password: "YourOTAPassword"
api_encryption_key: "base64_key_here"
```

Generate API encryption key:
```bash
python3 -c "import secrets, base64; print(base64.b64encode(secrets.token_bytes(32)).decode())"
```

### 3. Flash Firmware

First flash via USB:
```bash
esphome run heishamon.yaml
```

Subsequent updates via OTA:
```bash
esphome run heishamon.yaml --device heishamon.local
```

## Configuration Files

- **heishamon.yaml**: Full configuration with all entities
- **heishamon_minimal.yaml**: Minimal configuration with only essential sensors
- **secrets.yaml**: WiFi and password configuration

## Protocol Details

- **Baud Rate**: 9600
- **Data Bits**: 8
- **Parity**: Even
- **Stop Bits**: 1
- **Query Interval**: 5 seconds (configurable)

The component sends query packets to request data from the heat pump and decodes the 203-byte response packets containing all operating parameters.

## Home Assistant Integration

After flashing, the device will automatically appear in Home Assistant via the API integration. All entities will be available:

### Climate Entities
- `climate.heishamon_zone1` - Zone 1 heating/cooling control
- `climate.heishamon_dhw` - Domestic hot water control

### Main Sensors
- Outside temperature
- Inlet/Outlet water temperature
- DHW temperature
- Zone temperatures (water, room, target)
- Compressor frequency
- Pump flow rate
- Power production/consumption
- Operating hours

### Binary Sensors
- Heat pump state
- Defrost state
- Heater states (DHW, room, internal, external)
- Three-way valve position
- Holiday mode

### Controls
- Operating mode switches
- Quiet mode (0-3)
- Powerful mode (0-3)
- Force DHW
- Holiday mode
- Temperature setpoints

## Troubleshooting

### No Data Received
1. Check serial connections (GPIO13/GPIO15, not GPIO1/GPIO3)
2. Verify level shifter is working
3. Check enable pin (GPIO5) is HIGH during operation
4. Enable DEBUG logging in `heishamon.yaml`

### Communication Errors
1. Verify UART parameters (9600 8E1)
2. Check checksum calculation
3. Monitor logs for incomplete packets

### Enable Debug Logging
Change in `heishamon.yaml`:
```yaml
logger:
  level: DEBUG
```

## Credits

Based on the original [HeishaMon](https://github.com/Egyras/HeishaMon) project by Igor Ybema (Egyras).

Protocol documentation and decode functions derived from HeishaMon Arduino implementation.

## License

This project inherits the license from the original HeishaMon project.
