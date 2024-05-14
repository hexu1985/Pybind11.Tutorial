#pragma once

#include <string>

struct Pet {
    Pet(const std::string &name) : name(name) { }
    std::string name;
};
