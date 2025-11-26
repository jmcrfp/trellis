import esphome.codegen as cg
from esphome.components import key_provider
import esphome.config_validation as cv
from esphome.const import CONF_ID

cg.add_library("Wire", None)

cg.add_library(
    name="Adafruit_Trellis",
    repository="https://github.com/adafruit/Adafruit_Trellis_Library",
    version=None,
)

CODEOWNERS = ["@jmcrfp"]

AUTO_LOAD = ["key_provider"]

MULTI_CONF = True

trellis_keypad_ns = cg.esphome_ns.namespace("trellis_keypad")
TrellisKeypad = trellis_keypad_ns.class_(
    "TrellisKeypad", key_provider.KeyProvider, cg.Component
)

CONF_KEYPAD_ID = "keypad_id"
CONF_KEYS = "keys"


def check_keys(obj):
    if len(obj[CONF_KEYS]) != 16:
        raise cv.Invalid("keys must be exactly 16 characters for 4x4 Trellis")
    return obj


CONFIG_SCHEMA = cv.All(
    cv.COMPONENT_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(TrellisKeypad),
            cv.Required(CONF_KEYS): cv.string,
        }
    ),
    check_keys,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    cg.add(var.set_keys(config[CONF_KEYS]))
