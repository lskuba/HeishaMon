import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID
from esphome import pins

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor", "binary_sensor", "text_sensor", "switch", "select", "number", "climate"]

MULTI_CONF = True

panasonic_aquarea_ns = cg.esphome_ns.namespace("panasonic_aquarea")
PanasonicAquarea = panasonic_aquarea_ns.class_(
    "PanasonicAquarea", cg.PollingComponent, uart.UARTDevice
)

CONF_PANASONIC_AQUAREA_ID = "panasonic_aquarea_id"
CONF_ENABLE_PIN = "enable_pin"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(PanasonicAquarea),
            cv.Optional(CONF_ENABLE_PIN): pins.gpio_output_pin_schema,
        }
    )
    .extend(cv.polling_component_schema("5s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    
    if CONF_ENABLE_PIN in config:
        enable_pin = await cg.gpio_pin_expression(config[CONF_ENABLE_PIN])
        cg.add(var.set_enable_pin(enable_pin))
