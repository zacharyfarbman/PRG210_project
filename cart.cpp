#include "cart.h"
#include <iostream>

Cart::Cart()
    : courses(nullptr),
    schedules(nullptr),
    course_count(0),
    total_cost(0.0)
{
}

Cart::~Cart()
{
    delete[] courses;
    delete[] schedules;
}

void Cart::calculateTotalCost()
{
    double subtotal = 0.0;

    for (std::size_t i = 0; i < course_count; ++i)
    {
        subtotal += courses[i].getPrice();
    }

    total_cost = subtotal * 1.13;
}

void Cart::addCourse(const Course& course)
{
    Course* expandedCourses = new Course[course_count + 1];
    std::string* expandedSchedules = new std::string[course_count + 1];

    for (std::size_t i = 0; i < course_count; ++i)
    {
        expandedCourses[i] = courses[i];
        expandedSchedules[i] = schedules[i];
    }

    expandedCourses[course_count] = course;
    expandedSchedules[course_count] = course.getSchedule();

    delete[] courses;
    delete[] schedules;

    courses = expandedCourses;
    schedules = expandedSchedules;

    ++course_count;

    calculateTotalCost();
}

void Cart::displayCourses() const
{
    if (course_count == 0)
    {
        std::cout << "No courses in the cart." << std::endl;
        return;
    }

    for (std::size_t i = 0; i < course_count; i++)
    {
        courses[i].display();
    }

    std::cout << "----------------------" << std::endl;
    std::cout << "Total with tax: $" << total_cost << std::endl;
}

void Cart::clearCart()
{
    delete[] courses;
    courses = nullptr;

    delete[] schedules;
    schedules = nullptr;
        
    course_count = 0;
    total_cost = 0.0;

    std::cout << "Your Cart has been cleared" << std::endl;
}

bool Cart::isEmpty() const
{
    return course_count == 0;
}

std::size_t Cart::getCourseCount() const
{
    return course_count;
}

double Cart::getTotalCost() const
{
    return total_cost;
}

