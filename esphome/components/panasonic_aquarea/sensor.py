import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_PRESSURE,
    DEVICE_CLASS_FREQUENCY,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
    UNIT_CELSIUS,
    UNIT_WATT,
    UNIT_AMPERE,
    UNIT_HERTZ,
)
from . import PanasonicAquarea, CONF_PANASONIC_AQUAREA_ID

DEPENDENCIES = ["panasonic_aquarea"]

CONF_OUTSIDE_TEMP = "outside_temp"
CONF_INLET_TEMP = "inlet_temp"
CONF_OUTLET_TEMP = "outlet_temp"
CONF_DHW_TEMP = "dhw_temp"
CONF_DHW_TARGET_TEMP = "dhw_target_temp"
CONF_COMPRESSOR_FREQ = "compressor_freq"
CONF_PUMP_FLOW = "pump_flow"
CONF_Z1_WATER_TEMP = "z1_water_temp"
CONF_Z2_WATER_TEMP = "z2_water_temp"
CONF_Z1_TARGET_TEMP = "z1_target_temp"
CONF_Z2_TARGET_TEMP = "z2_target_temp"
CONF_Z1_ROOM_TEMP = "z1_room_temp"
CONF_Z2_ROOM_TEMP = "z2_room_temp"
CONF_HEAT_POWER_PRODUCTION = "heat_power_production"
CONF_HEAT_POWER_CONSUMPTION = "heat_power_consumption"
CONF_DHW_POWER_PRODUCTION = "dhw_power_production"
CONF_DHW_POWER_CONSUMPTION = "dhw_power_consumption"
CONF_OPERATIONS_HOURS = "operations_hours"
CONF_OPERATIONS_COUNTER = "operations_counter"
CONF_DISCHARGE_TEMP = "discharge_temp"
CONF_OUTSIDE_PIPE_TEMP = "outside_pipe_temp"
CONF_DEFROST_TEMP = "defrost_temp"
CONF_EVA_OUTLET_TEMP = "eva_outlet_temp"
CONF_BYPASS_OUTLET_TEMP = "bypass_outlet_temp"
CONF_IPM_TEMP = "ipm_temp"
CONF_HIGH_PRESSURE = "high_pressure"
CONF_LOW_PRESSURE = "low_pressure"
CONF_COMPRESSOR_CURRENT = "compressor_current"
CONF_PUMP_SPEED = "pump_speed"
CONF_PUMP_DUTY = "pump_duty"
CONF_FAN1_SPEED = "fan1_speed"
CONF_FAN2_SPEED = "fan2_speed"

UNIT_LITERS_PER_MIN = "L/min"
UNIT_HOURS = "h"
UNIT_KGF_CM2 = "kgf/cm²"
UNIT_RPM = "rpm"
UNIT_PERCENT = "%"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_PANASONIC_AQUAREA_ID): cv.use_id(PanasonicAquarea),
        cv.Optional(CONF_OUTSIDE_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_INLET_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_OUTLET_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_DHW_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_DHW_TARGET_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_COMPRESSOR_FREQ): sensor.sensor_schema(
            unit_of_measurement=UNIT_HERTZ,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_FREQUENCY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_PUMP_FLOW): sensor.sensor_schema(
            unit_of_measurement=UNIT_LITERS_PER_MIN,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_Z1_WATER_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_Z2_WATER_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_Z1_TARGET_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_Z2_TARGET_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_Z1_ROOM_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_Z2_ROOM_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_HEAT_POWER_PRODUCTION): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_HEAT_POWER_CONSUMPTION): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_DHW_POWER_PRODUCTION): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_DHW_POWER_CONSUMPTION): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_OPERATIONS_HOURS): sensor.sensor_schema(
            unit_of_measurement=UNIT_HOURS,
            accuracy_decimals=0,
            state_class=STATE_CLASS_TOTAL_INCREASING,
        ),
        cv.Optional(CONF_OPERATIONS_COUNTER): sensor.sensor_schema(
            accuracy_decimals=0,
            state_class=STATE_CLASS_TOTAL_INCREASING,
        ),
        cv.Optional(CONF_DISCHARGE_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_OUTSIDE_PIPE_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_DEFROST_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_EVA_OUTLET_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_BYPASS_OUTLET_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_IPM_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_HIGH_PRESSURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_KGF_CM2,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_PRESSURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_LOW_PRESSURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_KGF_CM2,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_PRESSURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_COMPRESSOR_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_PUMP_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_RPM,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_PUMP_DUTY): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_FAN1_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_RPM,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_FAN2_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_RPM,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])

    if CONF_OUTSIDE_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_OUTSIDE_TEMP])
        cg.add(parent.set_outside_temp_sensor(sens))

    if CONF_INLET_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_INLET_TEMP])
        cg.add(parent.set_inlet_temp_sensor(sens))

    if CONF_OUTLET_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_OUTLET_TEMP])
        cg.add(parent.set_outlet_temp_sensor(sens))

    if CONF_DHW_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_DHW_TEMP])
        cg.add(parent.set_dhw_temp_sensor(sens))

    if CONF_DHW_TARGET_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_DHW_TARGET_TEMP])
        cg.add(parent.set_dhw_target_temp_sensor(sens))

    if CONF_COMPRESSOR_FREQ in config:
        sens = await sensor.new_sensor(config[CONF_COMPRESSOR_FREQ])
        cg.add(parent.set_compressor_freq_sensor(sens))

    if CONF_PUMP_FLOW in config:
        sens = await sensor.new_sensor(config[CONF_PUMP_FLOW])
        cg.add(parent.set_pump_flow_sensor(sens))

    if CONF_Z1_WATER_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_Z1_WATER_TEMP])
        cg.add(parent.set_z1_water_temp_sensor(sens))

    if CONF_Z2_WATER_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_Z2_WATER_TEMP])
        cg.add(parent.set_z2_water_temp_sensor(sens))

    if CONF_Z1_TARGET_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_Z1_TARGET_TEMP])
        cg.add(parent.set_z1_target_temp_sensor(sens))

    if CONF_Z2_TARGET_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_Z2_TARGET_TEMP])
        cg.add(parent.set_z2_target_temp_sensor(sens))

    if CONF_Z1_ROOM_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_Z1_ROOM_TEMP])
        cg.add(parent.set_z1_room_temp_sensor(sens))

    if CONF_Z2_ROOM_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_Z2_ROOM_TEMP])
        cg.add(parent.set_z2_room_temp_sensor(sens))

    if CONF_HEAT_POWER_PRODUCTION in config:
        sens = await sensor.new_sensor(config[CONF_HEAT_POWER_PRODUCTION])
        cg.add(parent.set_heat_power_production_sensor(sens))

    if CONF_HEAT_POWER_CONSUMPTION in config:
        sens = await sensor.new_sensor(config[CONF_HEAT_POWER_CONSUMPTION])
        cg.add(parent.set_heat_power_consumption_sensor(sens))

    if CONF_DHW_POWER_PRODUCTION in config:
        sens = await sensor.new_sensor(config[CONF_DHW_POWER_PRODUCTION])
        cg.add(parent.set_dhw_power_production_sensor(sens))

    if CONF_DHW_POWER_CONSUMPTION in config:
        sens = await sensor.new_sensor(config[CONF_DHW_POWER_CONSUMPTION])
        cg.add(parent.set_dhw_power_consumption_sensor(sens))

    if CONF_OPERATIONS_HOURS in config:
        sens = await sensor.new_sensor(config[CONF_OPERATIONS_HOURS])
        cg.add(parent.set_operations_hours_sensor(sens))

    if CONF_OPERATIONS_COUNTER in config:
        sens = await sensor.new_sensor(config[CONF_OPERATIONS_COUNTER])
        cg.add(parent.set_operations_counter_sensor(sens))

    if CONF_DISCHARGE_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_DISCHARGE_TEMP])
        cg.add(parent.set_discharge_temp_sensor(sens))

    if CONF_OUTSIDE_PIPE_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_OUTSIDE_PIPE_TEMP])
        cg.add(parent.set_outside_pipe_temp_sensor(sens))

    if CONF_DEFROST_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_DEFROST_TEMP])
        cg.add(parent.set_defrost_temp_sensor(sens))

    if CONF_EVA_OUTLET_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_EVA_OUTLET_TEMP])
        cg.add(parent.set_eva_outlet_temp_sensor(sens))

    if CONF_BYPASS_OUTLET_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_BYPASS_OUTLET_TEMP])
        cg.add(parent.set_bypass_outlet_temp_sensor(sens))

    if CONF_IPM_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_IPM_TEMP])
        cg.add(parent.set_ipm_temp_sensor(sens))

    if CONF_HIGH_PRESSURE in config:
        sens = await sensor.new_sensor(config[CONF_HIGH_PRESSURE])
        cg.add(parent.set_high_pressure_sensor(sens))

    if CONF_LOW_PRESSURE in config:
        sens = await sensor.new_sensor(config[CONF_LOW_PRESSURE])
        cg.add(parent.set_low_pressure_sensor(sens))

    if CONF_COMPRESSOR_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_COMPRESSOR_CURRENT])
        cg.add(parent.set_compressor_current_sensor(sens))

    if CONF_PUMP_SPEED in config:
        sens = await sensor.new_sensor(config[CONF_PUMP_SPEED])
        cg.add(parent.set_pump_speed_sensor(sens))

    if CONF_PUMP_DUTY in config:
        sens = await sensor.new_sensor(config[CONF_PUMP_DUTY])
        cg.add(parent.set_pump_duty_sensor(sens))

    if CONF_FAN1_SPEED in config:
        sens = await sensor.new_sensor(config[CONF_FAN1_SPEED])
        cg.add(parent.set_fan1_speed_sensor(sens))

    if CONF_FAN2_SPEED in config:
        sens = await sensor.new_sensor(config[CONF_FAN2_SPEED])
        cg.add(parent.set_fan2_speed_sensor(sens))
