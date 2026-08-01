#include "student_interface.h"

#include <cstddef>

extern Department* StoreDepartments;
extern int TotalDepartments;

StudentInterface::StudentInterface()
    : Interface(),
    cart()
{
}

StudentInterface::StudentInterface(std::istream& inputStream,
    std::ostream& outputStream)
    : Interface(inputStream, outputStream),
    cart()
{
}

void StudentInterface::showMainMenu() const
{
    getOutput() << "\n===== Student Menu =====\n";
    getOutput() << "1. View cart\n";
    getOutput() << "2. Browse departments to add courses\n";
    getOutput() << "3. Exit\n";
}

void StudentInterface::showCartMenu()
{
    bool viewingCart = true;

    while (viewingCart)
    {
        getOutput() << "\n===== Cart =====\n";

        if (cart.isEmpty())
        {
            getOutput() << "Your cart is empty.\n";
        }
        else
        {
            cart.displayCourses();
        }

        getOutput() << "\n1. Checkout\n";
        getOutput() << "2. Clear cart\n";
        getOutput() << "3. Return to student menu\n";

        int choice = 0;

        if (!readChoice("Enter your choice: ", 1, 3, choice))
        {
            return;
        }

        switch (choice)
        {
        case 1:
            checkout();
            break;

        case 2:
            if (cart.isEmpty())
            {
                getOutput() << "Your cart is already empty.\n";
            }
            else
            {
                cart.clearCart();
                getOutput() << "Cart cleared successfully.\n";
            }
            break;

        case 3:
            viewingCart = false;
            break;
        }
    }
}

void StudentInterface::browseDepartments()
{
    if (StoreDepartments == nullptr || TotalDepartments <= 0)
    {
        getOutput() << "No departments are available.\n";
        return;
    }

    std::size_t departmentIndex = 0;

    if (!selectDepartment(departmentIndex))
    {
        return;
    }

    Department& selectedDepartment =
        StoreDepartments[departmentIndex];

    if (selectedDepartment.getCourseCount() == 0)
    {
        getOutput()
            << "No courses are available in this department.\n";

        return;
    }

    bool browsingCourses = true;

    while (browsingCourses)
    {
        getOutput() << "\n===== Courses =====\n";
        displayCourses(selectedDepartment);

        int courseChoice = 0;

        if (!readChoice(
            "Enter a course number to add to the cart "
            "[0 to go back]: ",
            0,
            static_cast<int>(
                selectedDepartment.getCourseCount()),
            courseChoice))
        {
            return;
        }

        if (courseChoice == 0)
        {
            browsingCourses = false;
            continue;
        }

        const Course* selectedCourse =
            selectedDepartment.getCourse(
                static_cast<std::size_t>(courseChoice - 1));

        if (selectedCourse == nullptr)
        {
            getOutput() << "The selected course was not found.\n";
            continue;
        }

        cart.addCourse(*selectedCourse);

        getOutput() << "Course added to the cart.\n";
    }
}

void StudentInterface::checkout()
{
    if (cart.isEmpty())
    {
        getOutput() << "Your cart is empty.\n";
        return;
    }

    getOutput() << "\n===== Checkout =====\n";
    cart.displayCourses();

    getOutput() << "Checkout completed successfully.\n";

    cart.clearCart();
}

void StudentInterface::run()
{
    bool running = true;

    while (running)
    {
        showMainMenu();

        int choice = 0;

        if (!readChoice("Enter your choice: ", 1, 3, choice))
        {
            return;
        }

        switch (choice)
        {
        case 1:
            showCartMenu();
            break;

        case 2:
            browseDepartments();
            break;

        case 3:
            running = false;
            break;
        }
    }
}