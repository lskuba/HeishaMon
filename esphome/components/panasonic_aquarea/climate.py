import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.const import CONF_ID
from . import panasonic_aquarea_ns, PanasonicAquarea, CONF_PANASONIC_AQUAREA_ID

DEPENDENCIES = ["panasonic_aquarea"]

AquareaClimate = panasonic_aquarea_ns.class_("AquareaClimate", climate.Climate, cg.Component)
AquareaDHWClimate = panasonic_aquarea_ns.class_("AquareaDHWClimate", climate.Climate, cg.Component)

CONF_ZONE1_CLIMATE = "zone1"
CONF_DHW_CLIMATE = "dhw"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_PANASONIC_AQUAREA_ID): cv.use_id(PanasonicAquarea),
        cv.Optional(CONF_ZONE1_CLIMATE): climate.climate_schema(AquareaClimate),
        cv.Optional(CONF_DHW_CLIMATE): climate.climate_schema(AquareaDHWClimate),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])

    if CONF_ZONE1_CLIMATE in config:
        conf = config[CONF_ZONE1_CLIMATE]
        var = await climate.new_climate(conf)
        await cg.register_component(var, conf)
        cg.add(var.set_parent(parent))
        cg.add(parent.set_zone1_climate(var))

    if CONF_DHW_CLIMATE in config:
        conf = config[CONF_DHW_CLIMATE]
        var = await climate.new_climate(conf)
        await cg.register_component(var, conf)
        cg.add(var.set_parent(parent))
        cg.add(parent.set_dhw_climate(var))
