#include "../src/TypeErasure/TypeErasure.hpp"
#include "../src/TypeErasure/Animal.hpp"

bool empty_test()
{
    AnimalView av{};

    return av.speak().empty() && av.type().empty() && av.next_year_population(0) == 0;
}

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
    return empty_test()
        && basic_test()
        && switch_animal_test();
}