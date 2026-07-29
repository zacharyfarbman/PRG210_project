#include "department.h"
#include <cstddef>
#include <iostream>

Department::Department()
    : department_name(nullptr), courses(nullptr), course_count(0) {}

Department::Department(const std::string &departmentName)
    : department_name(nullptr), courses(nullptr), course_count(0) {
  const std::size_t nameLength = departmentName.length();

  department_name = new char[nameLength + 1];

  for (std::size_t i = 0; i < nameLength; ++i) {
    department_name[i] = departmentName[i];
  }

  department_name[nameLength] = '\0';
}

Department::Department(const Department &other)
    : department_name(nullptr), courses(nullptr),
      course_count(other.course_count) {
  const char *sourceName = other.getName();
  std::size_t nameLength = 0;

  while (sourceName[nameLength] != '\0') {
    ++nameLength;
  }

  department_name = new char[nameLength + 1];

  for (std::size_t i = 0; i < nameLength; ++i) {
    department_name[i] = sourceName[i];
  }

  department_name[nameLength] = '\0';

  if (course_count > 0) {
    courses = new Course[course_count];

    for (std::size_t i = 0; i < course_count; ++i) {
      courses[i] = other.courses[i];
    }
  }
}

Department &Department::operator=(const Department &other) {
  if (this != &other) {
    delete[] department_name;
    delete[] courses;

    department_name = nullptr;
    courses = nullptr;
    course_count = other.course_count;

    const char *sourceName = other.getName();
    std::size_t nameLength = 0;

    while (sourceName[nameLength] != '\0') {
      ++nameLength;
    }

    department_name = new char[nameLength + 1];

    for (std::size_t i = 0; i < nameLength; ++i) {
      department_name[i] = sourceName[i];
    }

    department_name[nameLength] = '\0';

    if (course_count > 0) {
      courses = new Course[course_count];

      for (std::size_t i = 0; i < course_count; ++i) {
        courses[i] = other.courses[i];
      }
    }
  }

  return *this;
}

Department::~Department() {
  delete[] department_name;
  delete[] courses;
}

const char *Department::getName() const {
  if (department_name == nullptr) {
    return "";
  }

  return department_name;
}

std::size_t Department::getCourseCount() const { return course_count; }

Course *Department::getCourse(std::size_t index) {
  if (index >= course_count) {
    return nullptr;
  }

  return &courses[index];
}

const Course *Department::getCourse(std::size_t index) const {
  if (index >= course_count) {
    return nullptr;
  }

  return &courses[index];
}

void Department::addCourse(const Course &course) {
  Course *expandedCourses = new Course[course_count + 1];

  for (std::size_t i = 0; i < course_count; ++i) {
    expandedCourses[i] = courses[i];
  }

  expandedCourses[course_count] = course;

  delete[] courses;
  courses = expandedCourses;
  ++course_count;
}

void Department::displayCourses() const {
  if (course_count == 0) {
    std::cout << "No courses available in this department." << std::endl;
    return;
  }

  for (std::size_t i = 0; i < course_count; ++i) {
    std::cout << i + 1 << ". ";
    courses[i].display();
  }
}
