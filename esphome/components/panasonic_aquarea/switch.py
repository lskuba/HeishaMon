import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID
from . import panasonic_aquarea_ns, PanasonicAquarea, CONF_PANASONIC_AQUAREA_ID

DEPENDENCIES = ["panasonic_aquarea"]

AquareaSwitch = panasonic_aquarea_ns.class_("AquareaSwitch", switch.Switch, cg.Component)

CONF_HEATPUMP_SWITCH = "heatpump"
CONF_FORCE_DHW_SWITCH = "force_dhw"
CONF_HOLIDAY_MODE_SWITCH = "holiday_mode"
CONF_FORCE_DEFROST_SWITCH = "force_defrost"
CONF_FORCE_STERILIZATION_SWITCH = "force_sterilization"

SWITCH_TYPES = {
    CONF_HEATPUMP_SWITCH: "heatpump",
    CONF_FORCE_DHW_SWITCH: "force_dhw",
    CONF_HOLIDAY_MODE_SWITCH: "holiday_mode",
    CONF_FORCE_DEFROST_SWITCH: "force_defrost",
    CONF_FORCE_STERILIZATION_SWITCH: "force_sterilization",
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_PANASONIC_AQUAREA_ID): cv.use_id(PanasonicAquarea),
        cv.Optional(CONF_HEATPUMP_SWITCH): switch.switch_schema(
            AquareaSwitch,
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_FORCE_DHW_SWITCH): switch.switch_schema(
            AquareaSwitch,
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_HOLIDAY_MODE_SWITCH): switch.switch_schema(
            AquareaSwitch,
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_FORCE_DEFROST_SWITCH): switch.switch_schema(
            AquareaSwitch,
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_FORCE_STERILIZATION_SWITCH): switch.switch_schema(
            AquareaSwitch,
        ).extend(cv.COMPONENT_SCHEMA),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])

    for key, switch_type in SWITCH_TYPES.items():
        if key in config:
            conf = config[key]
            var = await switch.new_switch(conf)
            await cg.register_component(var, conf)
            cg.add(var.set_parent(parent))
            cg.add(var.set_switch_type(switch_type))
