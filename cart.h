#pragma once

#include "course.h"
#include <cstddef>
#include <string>

class Cart
{
private:
    Course* courses;
    std::string* schedules;
    std::size_t course_count;
    double total_cost;

    void calculateTotalCost();

public:
    Cart();
    ~Cart();

    void addCourse(const Course& course);
    void displayCourses() const;
    void clearCart();

    bool isEmpty() const;
    std::size_t getCourseCount() const;
    double getTotalCost() const;
};
