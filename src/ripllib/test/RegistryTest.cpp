#include "catch.hpp"
#include "ripllib/Registry.h"

using namespace std;

TEST_CASE("Registrar", "Registrar")
{
    SECTION("basics")
    {
        class TestRegistry : public Registry
        {
        public:
            void registerOp(const char* name, const char* description, ExecuteFunc executeFunc, HelpFunc helpFunc) override { }
        };

        TestRegistry registry;
    }
}
