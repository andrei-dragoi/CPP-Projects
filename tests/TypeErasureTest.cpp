#include "../src/TypeErasure/TypeErasure.hpp"
#include "../src/TypeErasure/Animal.hpp"

bool basic_test()
{
    Cow cow{};
    AnimalView av{cow};
    
    return av.type() == cow.type() && av.speak() == cow.speak() && av.next_year_population(10) == cow.next_year_population(10);
}

bool switch_animal_test()
{
    Wolf wolf{};
    AnimalView av{wolf};
    
    Cow cow{};
    av = cow;
    
    return av.type() == cow.type() && av.speak() == cow.speak() && av.next_year_population(10) == cow.next_year_population(10);
}

bool animal_view_tests()
{
    return basic_test()
        && switch_animal_test();
}