#pragma once

#include "pet.hpp"

class Cat : public pets::Pet {
public:
    explicit Cat(std::string name) : pets::Pet(std::move(name)) {}
};

