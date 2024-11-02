#include "../src/Matrix/Matrix.hpp"

bool empty_test()
{
    Matrix<int> m{};
    
    return m.rows() == 0 && m.cols() == 0;
}

bool equality_test()
{
    std::vector<std::vector<int>> A
    {
    {
        {0, 1, 2}, 
        {3, 4, 5}
    }    
    };
    
    std::vector<std::vector<int>> B
    {
    {
        {0, 1, 2}, 
        {3, 4, 5}
    }    
    };
    
    
    Matrix<int> m1{A};
    Matrix<int> m2{B};
    
    return m1 == m2;
}

bool add_test()
{
    std::vector<std::vector<int>> A
    {
    {
        {0, 1, 2}, 
        {3, 4, 5}
    }    
    };
    
    std::vector<std::vector<int>> B
    {
    {
        {0, 2, 4}, 
        {6, 8, 10}
    }    
    };
    
    Matrix<int> m1{A};
    Matrix<int> m2{A};
    Matrix<int> m3{B};
    
    return m1 + m2 == m3;
}

bool sub_test()
{
    std::vector<std::vector<int>> A
    {
    {
        {0, 1, 2}, 
        {3, 4, 5}
    }    
    };
    
    std::vector<std::vector<int>> B
    {
    {
        {0, 0, 0}, 
        {0, 0, 0}
    }    
    };
    
    Matrix<int> m1{A};
    Matrix<int> m2{A};
    Matrix<int> m3{B};
    
    return m1 - m2 == m3;
}

bool mult_test()
{
    std::vector<std::vector<int>> A
    {
    {
        {0, 1, 2}, 
        {3, 4, 5}
    }    
    };
    
    std::vector<std::vector<int>> B
    {
    {
        {6, 7, 8, 9}, 
        {10, 11, 12, 13},
        {14, 15, 16, 17},
    }    
    };
    
    std::vector<std::vector<int>> C
    {
    {
        {38, 41, 44, 47}, 
        {128, 140, 152, 164},
    }    
    };
    
    Matrix m1{std::move(A)};
    Matrix m2{std::move(B)};
    Matrix m3{std::move(C)};
    
    return m1 * m2 == m3;
}

bool copy_constructor_test()
{
    std::vector<std::vector<int>> A
    {
    {
        {0, 1, 2}, 
        {3, 4, 5}
    }    
    };
    
    Matrix m1{A};
    Matrix m2{m1};
    
    return m1 == m2;
}

bool move_constructor_test()
{
    std::vector<std::vector<int>> A
    {
    {
        {0, 1, 2}, 
        {3, 4, 5}
    }    
    };
    
    Matrix m1{A};
    Matrix m2{A};
    Matrix m3{std::move(m2)};
    
    return m1 == m3;
}

bool copy_operator_test()
{
    std::vector<std::vector<int>> A
    {
    {
        {0, 1, 2}, 
        {3, 4, 5}
    }    
    };
    
    Matrix m1{A};
    Matrix m2 = m1;
    
    return m1 == m2;
}

bool move_operator_test()
{
    std::vector<std::vector<int>> A
    {
    {
        {0, 1, 2}, 
        {3, 4, 5}
    }    
    };
    
    Matrix m1{A};
    Matrix m2{A};
    Matrix m3 = std::move(m2);
    
    return m1 == m3;
}

bool matrix_tests()
{
    return empty_test()
        && equality_test()
        && add_test()
        && sub_test()
        && mult_test()
        && copy_constructor_test()
        && move_constructor_test()
        && copy_operator_test()
        && move_operator_test();
}