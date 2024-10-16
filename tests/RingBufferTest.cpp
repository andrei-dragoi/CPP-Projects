#include "../src/RingBuffer/RingBuffer.hpp"

bool simple_read_write_test()
{
    RingBuffer<size_t, 3> b{};
    
    for (size_t i = 0; i < 4; ++i)
    {
        b.write(i);
    }
    
    size_t result{};
    for (size_t i = 0; i < 4; ++i)
    {
        b.read(result);
        
        if (result != i)
        {
            return false;
        }
    }
    
    return true;
}

bool buffer_tests()
{
    return simple_read_write_test();
}