#pragma once

#include <string>

class Course {
private:
  std::string course_number;
  std::string course_name;
  std::string schedule;
  double price;

public:
  Course();
  Course(const std::string &courseNumber, const std::string &courseName,
         const std::string &schedule, double price);
  ~Course() = default;

  const std::string &getCourseNumber() const;
  const std::string &getCourseName() const;
  const std::string &getSchedule() const;
  double getPrice() const;

  void display() const;
};
