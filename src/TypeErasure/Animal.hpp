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

private:
    std::string _animal_name{};
    std::string _animal_noise{};
    int _population_multiplier{};
};

class Cow : public Animal
{
public:
    std::string type() const
    {
        return "Cow";
    }
    
    std::string speak() const
    {
        return "Mooo";
    }
    
    int next_year_population(int current_population) const
    {
        return 2 * current_population;
    }
    
    ~Cow() = default;
};

class Wolf : public Animal
{
public:

    std::string type() const
    {
        return "Wolf";
    }
    
    std::string speak() const
    {
        return "Awoo";
    }
    
    int next_year_population(int current_population) const
    {
        return 4 * current_population;
    }
    
    ~Wolf() = default;
};