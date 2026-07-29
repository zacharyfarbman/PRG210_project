#include "course.h"
#include <iomanip>
#include <iostream>

Course::Course() : course_number(), course_name(), schedule(), price(0.0) {}

Course::Course(const std::string &courseNumber, const std::string &courseName,
               const std::string &courseSchedule, double coursePrice)
    : course_number(courseNumber), course_name(courseName),
      schedule(courseSchedule), price(coursePrice) {}

const std::string &Course::getCourseNumber() const { return course_number; }
const std::string &Course::getCourseName() const { return course_name; }
const std::string &Course::getSchedule() const { return schedule; }

double Course::getPrice() const { return price; }

void Course::display() const {
  std::cout << course_number << " - " << course_name
            << " | Schdeule: " << schedule << " | Price : $" << std::fixed
            << std::setprecision(2) << price << std::endl;
}
