#include "interface.h"
#include <iostream>
#include <limits>

int Interface::getValidatedChoice(int min_val, int max_val)
{
    int choice;

    while (true)
    {
        std::cout << "Please enter your choice: ";
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout << "Please enter a valid number between "
                << min_val << " and " << max_val << "."
                << std::endl;
        }
        else if (choice < min_val || choice > max_val)
        {
            std::cout << "Please enter a valid number between "
                << min_val << " and " << max_val << "."
                << std::endl;
        }
        else
        {
            return choice;
        }
    }
}

Interface::~Interface() = default;

void AdminInterface::run() {
    std::cout << "Admin Interface" << std::endl;
}

