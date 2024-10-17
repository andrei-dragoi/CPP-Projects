#pragma once

#include <string>

//super basic Animal class, created inefficiently by design to make existence of AnimalView necessary 
class Animal
{
public:
    virtual std::string type() const = 0;
    
    virtual std::string speak() const = 0; 
    
    virtual int next_year_population(int current_population) const = 0;
    
    virtual ~Animal() = default;
};

class Cow final : public Animal
{
public:
    std::string type() const final
    {
        return "Cow";
    }
    
    std::string speak() const final
    {
        return "Mooo";
    }
    
    int next_year_population(int current_population) const final
    {
        return 2 * current_population;
    }
    
    ~Cow() final = default;
};

class Wolf final : public Animal
{
public:

    std::string type() const final
    {
        return "Wolf";
    }
    
    std::string speak() const final
    {
        return "Awoo";
    }
    
    int next_year_population(int current_population) const final
    {
        return 4 * current_population;
    }
    
    ~Wolf() final = default;
};