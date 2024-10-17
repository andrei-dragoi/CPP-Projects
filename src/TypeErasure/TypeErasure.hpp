#pragma once

#include "Animal.hpp"

class AnimalView
{ 
public:
    template<typename A>
    AnimalView(A&& animal)
    : _address{ static_cast<std::remove_cvref_t<A>*>(&animal) }
    , _type_invoker{ [] (void* animal_address) { return static_cast<std::remove_cvref_t<A>*>(animal_address)->type(); } }
    , _speak_invoker{ [] (void* animal_address) { return static_cast<std::remove_cvref_t<A>*>(animal_address)->speak(); } }
    , _next_year_population_invoker{ [] (void* animal_address, int current_population = 0) { return static_cast<std::remove_cvref_t<A>*>(animal_address)->next_year_population(current_population); } }
    {
    }
    
    AnimalView() 
    : _address{ nullptr }
    , _type_invoker{ [](void*) { return std::string{}; } }  // Default lambda for type
    , _speak_invoker{ [](void*) { return std::string{}; } } // Default lambda for speak
    , _next_year_population_invoker{ [](void*, int) { return 0; } } // Default lambda for population
    {
    }
    
    std::string type() const
    {
        return _type_invoker(_address);
    }
    
    std::string speak() const
    {
        return _speak_invoker(_address);
    }
    
    int next_year_population(int current_population) const
    {
        return _next_year_population_invoker(_address, current_population);
    }
    
private:
    using type_function = std::string (void*);
    using speak_function = std::string (void*);
    using next_year_population_function = int (void*, int);
    
    void* _address;
    type_function* _type_invoker;
    speak_function* _speak_invoker;
    next_year_population_function* _next_year_population_invoker;
};