#include <cassert>
#include <iostream>

//forward declaration for aggregated, class-wide test functions
bool memory_pool_tests();
bool sqrt_func_tests();
bool optional_tests();
bool moving_average_tests();
bool moving_median_tests();
bool vector_tests();
bool run_all_tests();
bool spsc_all_tests();
bool animal_view_tests();
bool matrix_tests();
bool unique_ptr_tests();
bool shared_ptr_tests();
bool spsc_all_tests();
bool mutex_tests();

//call all forwarded declared functions in main
int main()
{
    assert(memory_pool_tests());
    assert(sqrt_func_tests());
    assert(optional_tests());
    assert(moving_average_tests());
    assert(moving_median_tests());
    assert(vector_tests());
    assert(run_all_tests());
    assert(spsc_all_tests());
    assert(animal_view_tests());
    assert(matrix_tests());
    assert(unique_ptr_tests());
    assert(shared_ptr_tests());
    assert(spsc_all_tests());
    assert(mutex_tests());

    return 0;
}