#include <iostream>
#include <list>
#include <vector>

#include "tests.hpp"

int main(int argc, const char * argv[]) {
    if (argc == 2) {
        std::string temp(argv[1]);

        if ("compile" == temp) {
            std::cout << "Compiled Ok.\n";
            return 0;
        }
    }
    std::cout << "Running all tests\n";

    size_t points[5];

    std::cout << "\nPart 1" << std::endl;
    points[0] = basic_ice_cream_test();
    std::cout << "Part 1 Score: " << points[0] << "/5" << std::endl;

    std::cout << "\nPart 2" << std::endl;
    points[1] = add_toppings_test();
    std::cout << "Part 2 Score: " << points[1] << "/5" << std::endl;

    std::cout << "\nPart 3" << std::endl;
    points[2] = toppings_cost_test();
    std::cout << "Part 3 Score: " << points[2] << "/13" << std::endl;

    std::cout << "\nPart 4" << std::endl;
    points[3] = visitor_tests();
    std::cout << "Part 4 Score: " << points[3] << "/14" << std::endl;

    std::cout << "\nPart 5" << std::endl;
    points[4] = inventory_check_test();
    std::cout << "Part 5 Score: " << points[4] << "/18" << std::endl;

    std::cout << "\n\nTOTAL SCORE: " << points[0] + points[1] + points[2] + points[3] + points[4] << "/55\n";

    return 0;
}