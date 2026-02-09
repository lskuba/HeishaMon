import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_RUNNING,
    DEVICE_CLASS_HEAT,
    DEVICE_CLASS_PROBLEM,
)
from . import PanasonicAquarea, CONF_PANASONIC_AQUAREA_ID

DEPENDENCIES = ["panasonic_aquarea"]

CONF_HEATPUMP_STATE = "heatpump_state"
CONF_DEFROST_STATE = "defrost_state"
CONF_DHW_HEATER_STATE = "dhw_heater_state"
CONF_ROOM_HEATER_STATE = "room_heater_state"
CONF_INTERNAL_HEATER_STATE = "internal_heater_state"
CONF_EXTERNAL_HEATER_STATE = "external_heater_state"
CONF_FORCE_DHW_STATE = "force_dhw_state"
CONF_HOLIDAY_MODE = "holiday_mode"
CONF_THREEWAY_VALVE = "threeway_valve"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_PANASONIC_AQUAREA_ID): cv.use_id(PanasonicAquarea),
        cv.Optional(CONF_HEATPUMP_STATE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_RUNNING,
        ),
        cv.Optional(CONF_DEFROST_STATE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_RUNNING,
        ),
        cv.Optional(CONF_DHW_HEATER_STATE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_HEAT,
        ),
        cv.Optional(CONF_ROOM_HEATER_STATE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_HEAT,
        ),
        cv.Optional(CONF_INTERNAL_HEATER_STATE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_HEAT,
        ),
        cv.Optional(CONF_EXTERNAL_HEATER_STATE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_HEAT,
        ),
        cv.Optional(CONF_FORCE_DHW_STATE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_RUNNING,
        ),
        cv.Optional(CONF_HOLIDAY_MODE): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_THREEWAY_VALVE): binary_sensor.binary_sensor_schema(),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])

    if CONF_HEATPUMP_STATE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_HEATPUMP_STATE])
        cg.add(parent.set_heatpump_state_sensor(sens))

    if CONF_DEFROST_STATE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_DEFROST_STATE])
        cg.add(parent.set_defrost_state_sensor(sens))

    if CONF_DHW_HEATER_STATE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_DHW_HEATER_STATE])
        cg.add(parent.set_dhw_heater_state_sensor(sens))

    if CONF_ROOM_HEATER_STATE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_ROOM_HEATER_STATE])
        cg.add(parent.set_room_heater_state_sensor(sens))

    if CONF_INTERNAL_HEATER_STATE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_INTERNAL_HEATER_STATE])
        cg.add(parent.set_internal_heater_state_sensor(sens))

    if CONF_EXTERNAL_HEATER_STATE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_EXTERNAL_HEATER_STATE])
        cg.add(parent.set_external_heater_state_sensor(sens))

    if CONF_FORCE_DHW_STATE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_FORCE_DHW_STATE])
        cg.add(parent.set_force_dhw_state_sensor(sens))

    if CONF_HOLIDAY_MODE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_HOLIDAY_MODE])
        cg.add(parent.set_holiday_mode_sensor(sens))

    if CONF_THREEWAY_VALVE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_THREEWAY_VALVE])
        cg.add(parent.set_threeway_valve_sensor(sens))
