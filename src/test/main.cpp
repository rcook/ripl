#ifdef USE_CATCH_MAIN
#define CATCH_CONFIG_MAIN
#else
#define CATCH_CONFIG_RUNNER
#endif
#include "catch.hpp"

#ifndef USE_CATCH_MAIN

#include <iostream>

using namespace std;

int main(int argc, char* const argv[])
{
    cout << "ripltest begin" << endl;
    auto result = Catch::Session().run(argc, argv);
    cout << "ripltest end" << endl;
    return result;
}

#endif
