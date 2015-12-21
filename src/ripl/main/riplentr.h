#pragma once

#include "riplgrey.h"
#include "RegistryImpl.h"

int riplMain1(
    const RegistryImpl& registry,
    unsigned argc,
    char **argv);

int riplMain2(
    const RegistryImpl& registry,
    unsigned argc,
    char** argv,
    riplGreyMap* pinputGreyMap,
    riplGreyMap* poutputGreyMap);

/** Shows general help information */
void showHelp(const RegistryImpl& registry);
