#include "riplregistry/Registry.h"

RIPL_REGISTER_PLUGIN_OPS(registry)
{
    registry.registerOp("add", [](int x, int y) { return x + y; });
    registry.registerOp("multiply", [](int x, int y) { return x * y; });
}
