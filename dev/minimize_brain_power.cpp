#include "../bengine/bengine_helpers.hpp"
#include <iostream>

int main() {
    for (unsigned char i = 1; i < 256 && i > 0; i++) {
        std::cout << bengine::string_helper::to_string_with_added_zeros(static_cast<int>(i), 3, 0, false) << ": "
        /**/      << static_cast<int>(bengine::bitwise_manipulator::get_subvalue(i, 6, 2)) << "\n    "
        /**/      << static_cast<int>(bengine::bitwise_manipulator::get_subvalue(i, 4, 2)) << " "
        /**/      << static_cast<int>(bengine::bitwise_manipulator::get_subvalue(i, 2, 2)) << "\n     "
        /**/      << static_cast<int>(bengine::bitwise_manipulator::get_subvalue(i, 0, 2)) << "\n----------\n";
    }
}

/**
 * 001:  01
 *     01  01
 *       01
 * 002:  01
 * 
 */