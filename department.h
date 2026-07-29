#pragma once

#include "course.h"

#include <cstddef>
#include <string>

class Department {
private:
  char *department_name;
  Course *courses;
  std::size_t course_count;

public:
  Department();
  Department(const std::string &);
  Department(const Department &);
  Department &operator=(const Department &);
  ~Department();

  const char *getName() const;

  std::size_t getCourseCount() const;

  Course *getCourse(std::size_t);
  const Course *getCourse(std::size_t) const;

  void addCourse(const Course &);
  void displayCourses() const;
};
