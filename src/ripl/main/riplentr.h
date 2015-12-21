#pragma once

#include <string>
#include <vector>

class RegistryImpl;

/* Processes command-line arguments and executes operators */
int runMain(const RegistryImpl& registry, const std::vector<std::string>& args);

/** Shows general help information */
void showHelp(const RegistryImpl& registry);
