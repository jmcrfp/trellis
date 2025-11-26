import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_KEY
from .. import TrellisKeypad, trellis_keypad_ns, CONF_KEYPAD_ID

DEPENDENCIES = ["trellis_keypad"]

TrellisKeypadBinarySensor = trellis_keypad_ns.class_(
    "TrellisKeypadBinarySensor", binary_sensor.BinarySensor
)

def check_button(obj):
    if len(obj[CONF_KEY]) != 1:
        raise cv.Invalid("Key must be one character")
    return obj

CONFIG_SCHEMA = cv.All(
    binary_sensor.binary_sensor_schema(TrellisKeypadBinarySensor).extend(
        {
            cv.GenerateID(CONF_KEYPAD_ID): cv.use_id(TrellisKeypad),
            cv.Required(CONF_KEY): cv.string,
        }
    ),
    check_button,
)

async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)
    pad = await cg.get_variable(config[CONF_KEYPAD_ID])

    # store key in C++
    cg.add(var.set_key(config[CONF_KEY]))

    # register the button with the keypad
    cg.add(pad.register_button(var))
