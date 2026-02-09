import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import CONF_ID
from . import PanasonicAquarea, CONF_PANASONIC_AQUAREA_ID

DEPENDENCIES = ["panasonic_aquarea"]

CONF_ERROR = "error"
CONF_OPERATING_MODE = "operating_mode"
CONF_HEATPUMP_MODEL = "heatpump_model"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_PANASONIC_AQUAREA_ID): cv.use_id(PanasonicAquarea),
        cv.Optional(CONF_ERROR): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_OPERATING_MODE): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_HEATPUMP_MODEL): text_sensor.text_sensor_schema(),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])

    if CONF_ERROR in config:
        sens = await text_sensor.new_text_sensor(config[CONF_ERROR])
        cg.add(parent.set_error_sensor(sens))

    if CONF_OPERATING_MODE in config:
        sens = await text_sensor.new_text_sensor(config[CONF_OPERATING_MODE])
        cg.add(parent.set_operating_mode_sensor(sens))

    if CONF_HEATPUMP_MODEL in config:
        sens = await text_sensor.new_text_sensor(config[CONF_HEATPUMP_MODEL])
        cg.add(parent.set_heatpump_model_sensor(sens))
