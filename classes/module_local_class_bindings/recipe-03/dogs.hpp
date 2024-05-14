#pragma once

#include "pet.hpp"

class Dog : public pets::Pet {
public:
    explicit Dog(std::string name) : pets::Pet(std::move(name)) {}
};

