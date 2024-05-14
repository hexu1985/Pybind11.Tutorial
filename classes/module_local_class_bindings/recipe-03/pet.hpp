#pragma once

#include <string>

namespace pets {

class Pet {
public:
    Pet(const std::string &name) : name_(name) { }

    std::string name() { return name_; }

private:
    std::string name_;
};

}   // namespace pets
