import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import (
    CONF_ID,
    CONF_MIN_VALUE,
    CONF_MAX_VALUE,
    CONF_STEP,
    UNIT_CELSIUS,
)
from . import panasonic_aquarea_ns, PanasonicAquarea, CONF_PANASONIC_AQUAREA_ID

DEPENDENCIES = ["panasonic_aquarea"]

AquareaNumber = panasonic_aquarea_ns.class_("AquareaNumber", number.Number, cg.Component)

CONF_DHW_TARGET_TEMP_NUMBER = "dhw_target_temp"
CONF_Z1_HEAT_TARGET_TEMP_NUMBER = "z1_heat_target_temp"
CONF_Z1_COOL_TARGET_TEMP_NUMBER = "z1_cool_target_temp"
CONF_Z2_HEAT_TARGET_TEMP_NUMBER = "z2_heat_target_temp"
CONF_Z2_COOL_TARGET_TEMP_NUMBER = "z2_cool_target_temp"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_PANASONIC_AQUAREA_ID): cv.use_id(PanasonicAquarea),
        cv.Optional(CONF_DHW_TARGET_TEMP_NUMBER): number.number_schema(
            AquareaNumber,
            unit_of_measurement=UNIT_CELSIUS,
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_Z1_HEAT_TARGET_TEMP_NUMBER): number.number_schema(
            AquareaNumber,
            unit_of_measurement=UNIT_CELSIUS,
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_Z1_COOL_TARGET_TEMP_NUMBER): number.number_schema(
            AquareaNumber,
            unit_of_measurement=UNIT_CELSIUS,
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_Z2_HEAT_TARGET_TEMP_NUMBER): number.number_schema(
            AquareaNumber,
            unit_of_measurement=UNIT_CELSIUS,
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_Z2_COOL_TARGET_TEMP_NUMBER): number.number_schema(
            AquareaNumber,
            unit_of_measurement=UNIT_CELSIUS,
        ).extend(cv.COMPONENT_SCHEMA),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])

    if CONF_DHW_TARGET_TEMP_NUMBER in config:
        conf = config[CONF_DHW_TARGET_TEMP_NUMBER]
        var = await number.new_number(
            conf,
            min_value=40,
            max_value=75,
            step=1,
        )
        await cg.register_component(var, conf)
        cg.add(var.set_parent(parent))
        cg.add(var.set_number_type("dhw_target"))

    if CONF_Z1_HEAT_TARGET_TEMP_NUMBER in config:
        conf = config[CONF_Z1_HEAT_TARGET_TEMP_NUMBER]
        var = await number.new_number(
            conf,
            min_value=-5,
            max_value=55,
            step=1,
        )
        await cg.register_component(var, conf)
        cg.add(var.set_parent(parent))
        cg.add(var.set_number_type("z1_heat_target"))

    if CONF_Z1_COOL_TARGET_TEMP_NUMBER in config:
        conf = config[CONF_Z1_COOL_TARGET_TEMP_NUMBER]
        var = await number.new_number(
            conf,
            min_value=-5,
            max_value=25,
            step=1,
        )
        await cg.register_component(var, conf)
        cg.add(var.set_parent(parent))
        cg.add(var.set_number_type("z1_cool_target"))

    if CONF_Z2_HEAT_TARGET_TEMP_NUMBER in config:
        conf = config[CONF_Z2_HEAT_TARGET_TEMP_NUMBER]
        var = await number.new_number(
            conf,
            min_value=-5,
            max_value=55,
            step=1,
        )
        await cg.register_component(var, conf)
        cg.add(var.set_parent(parent))
        cg.add(var.set_number_type("z2_heat_target"))

    if CONF_Z2_COOL_TARGET_TEMP_NUMBER in config:
        conf = config[CONF_Z2_COOL_TARGET_TEMP_NUMBER]
        var = await number.new_number(
            conf,
            min_value=-5,
            max_value=25,
            step=1,
        )
        await cg.register_component(var, conf)
        cg.add(var.set_parent(parent))
        cg.add(var.set_number_type("z2_cool_target"))
