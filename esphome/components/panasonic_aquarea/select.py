import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import CONF_ID
from . import panasonic_aquarea_ns, PanasonicAquarea, CONF_PANASONIC_AQUAREA_ID

DEPENDENCIES = ["panasonic_aquarea"]

AquareaSelect = panasonic_aquarea_ns.class_("AquareaSelect", select.Select, cg.Component)

CONF_OPERATING_MODE_SELECT = "operating_mode"
CONF_QUIET_MODE_SELECT = "quiet_mode"
CONF_POWERFUL_MODE_SELECT = "powerful_mode"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_PANASONIC_AQUAREA_ID): cv.use_id(PanasonicAquarea),
        cv.Optional(CONF_OPERATING_MODE_SELECT): select.select_schema(
            AquareaSelect,
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_QUIET_MODE_SELECT): select.select_schema(
            AquareaSelect,
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_POWERFUL_MODE_SELECT): select.select_schema(
            AquareaSelect,
        ).extend(cv.COMPONENT_SCHEMA),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])

    if CONF_OPERATING_MODE_SELECT in config:
        conf = config[CONF_OPERATING_MODE_SELECT]
        var = await select.new_select(
            conf,
            options=["Heat only", "Cool only", "Auto", "DHW only", "Heat+DHW", "Cool+DHW", "Auto+DHW"],
        )
        await cg.register_component(var, conf)
        cg.add(var.set_parent(parent))
        cg.add(var.set_select_type("operating_mode"))

    if CONF_QUIET_MODE_SELECT in config:
        conf = config[CONF_QUIET_MODE_SELECT]
        var = await select.new_select(
            conf,
            options=["Off", "Level 1", "Level 2", "Level 3"],
        )
        await cg.register_component(var, conf)
        cg.add(var.set_parent(parent))
        cg.add(var.set_select_type("quiet_mode"))

    if CONF_POWERFUL_MODE_SELECT in config:
        conf = config[CONF_POWERFUL_MODE_SELECT]
        var = await select.new_select(
            conf,
            options=["Off", "30 min", "60 min", "90 min"],
        )
        await cg.register_component(var, conf)
        cg.add(var.set_parent(parent))
        cg.add(var.set_select_type("powerful_mode"))
