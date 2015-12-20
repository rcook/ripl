#include "catch.hpp"
#include "riplregistry/Registry.h"

using namespace std;

TEST_CASE("Registrar", "Registrar")
{
    SECTION("basics")
    {
        class TestRegistry : public Registry
        {
        public:
            void registerOp(const string& name, OpFunc func) override { }
        };

        TestRegistry registry;
    }
}
