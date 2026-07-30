#include "CSVUtils.h"

#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

extern Department* StoreDepartments;
extern int TotalDepartments;

std::string trimCSVField(const std::string& value) {
  std::size_t first = 0;

  while (first < value.length() &&
         (value[first] == ' ' || value[first] == '\t' || value[first] == '\r' ||
          value[first] == '\n')) {
    ++first;
  }

  std::size_t last = value.length();

  while (last > first && (value[last - 1] == ' ' || value[last - 1] == '\t' ||
                          value[last - 1] == '\r' || value[last - 1] == '\n')) {
    --last;
  }

  return value.substr(first, last - first);
}

bool containsComma(const std::string& value) {
  for (std::size_t i = 0; i < value.length(); ++i) {
    if (value[i] == ',') {
      return true;
    }
  }

  return false;
}

bool parseNonNegativeInteger(const std::string& text, int& result) {
  std::stringstream stream(trimCSVField(text));
  int parsedValue = 0;
  char extraCharacter = '\0';

  if (!(stream >> parsedValue) || parsedValue < 0) {
    return false;
  }

  if (stream >> extraCharacter) {
    return false;
  }

  result = parsedValue;
  return true;
}

bool parsePositivePrice(const std::string& text, double& result) {
  std::stringstream stream(trimCSVField(text));
  double parsedValue = 0.0;
  char extraCharacter = '\0';

  if (!(stream >> parsedValue) || parsedValue <= 0.0) {
    return false;
  }

  if (stream >> extraCharacter) {
    return false;
  }

  result = parsedValue;
  return true;
}

bool isValidSchedule(const std::string& schedule) {
  return schedule == "M/W" || schedule == "T/R" || schedule == "W/F";
}

bool loadFailure(Department* loadedDepartments, const std::string& message) {
  delete[] loadedDepartments;
  std::cout << message << '\n';
  return false;
}

bool loadFromCSV(const char* filename) {
  if (filename == nullptr || filename[0] == '\0') {
    std::cout << "CSV filename is invalid.\n";
    return false;
  }

  std::ifstream file(filename);

  if (!file.is_open()) {
    std::cout << "CSV file not found. No data was loaded.\n";
    return false;
  }

  std::string line;

  if (!std::getline(file, line)) {
    std::cout << "CSV file is empty or invalid.\n";
    return false;
  }

  int loadedDepartmentCount = 0;

  if (!parseNonNegativeInteger(line, loadedDepartmentCount)) {
    std::cout << "Invalid department count in CSV file.\n";
    return false;
  }

  Department* loadedDepartments = nullptr;

  if (loadedDepartmentCount > 0) {
    loadedDepartments = new Department[loadedDepartmentCount];
  }

  for (int i = 0; i < loadedDepartmentCount; ++i) {
    if (!std::getline(file, line)) {
      return loadFailure(loadedDepartments,
                         "Missing department record in CSV file.");
    }

    std::stringstream departmentStream(line);
    std::string departmentName;
    std::string courseCountText;

    if (!std::getline(departmentStream, departmentName, ',') ||
        !std::getline(departmentStream, courseCountText)) {
      return loadFailure(loadedDepartments,
                         "Invalid department record in CSV file.");
    }

    departmentName = trimCSVField(departmentName);

    int loadedCourseCount = 0;

    if (departmentName.empty() ||
        !parseNonNegativeInteger(courseCountText, loadedCourseCount)) {
      return loadFailure(
          loadedDepartments,
          "Invalid department name or course count in CSV file.");
    }

    loadedDepartments[i] = Department(departmentName);

    for (int j = 0; j < loadedCourseCount; ++j) {
      if (!std::getline(file, line)) {
        return loadFailure(loadedDepartments,
                           "Missing course record in CSV file.");
      }

      std::stringstream courseStream(line);
      std::string courseNumber;
      std::string courseName;
      std::string schedule;
      std::string priceText;

      if (!std::getline(courseStream, courseNumber, ',') ||
          !std::getline(courseStream, courseName, ',') ||
          !std::getline(courseStream, schedule, ',') ||
          !std::getline(courseStream, priceText)) {
        return loadFailure(loadedDepartments,
                           "Invalid course record in CSV file.");
      }

      courseNumber = trimCSVField(courseNumber);
      courseName = trimCSVField(courseName);
      schedule = trimCSVField(schedule);

      double price = 0.0;

      if (courseNumber.empty() || courseName.empty() ||
          !isValidSchedule(schedule) || !parsePositivePrice(priceText, price)) {
        return loadFailure(loadedDepartments,
                           "Invalid course data in CSV file.");
      }

      loadedDepartments[i].addCourse(
          Course(courseNumber, courseName, schedule, price));
    }
  }

  delete[] StoreDepartments;
  StoreDepartments = loadedDepartments;
  TotalDepartments = loadedDepartmentCount;

  std::cout << "Data loaded from CSV successfully.\n";
  return true;
}

bool saveToCSV(const char* filename) {
  if (filename == nullptr || filename[0] == '\0') {
    std::cout << "CSV filename is invalid.\n";
    return false;
  }

  if (TotalDepartments < 0 ||
      (TotalDepartments > 0 && StoreDepartments == nullptr)) {
    std::cout << "Department data is invalid and cannot be saved.\n";
    return false;
  }

  for (int i = 0; i < TotalDepartments; ++i) {
    const Department& department = StoreDepartments[i];
    const std::string departmentName = department.getName();

    if (departmentName.empty() || containsComma(departmentName)) {
      std::cout << "Department data contains an invalid name.\n";
      return false;
    }

    for (std::size_t j = 0; j < department.getCourseCount(); ++j) {
      const Course* course = department.getCourse(j);

      if (course == nullptr || course->getCourseNumber().empty() ||
          course->getCourseName().empty() ||
          containsComma(course->getCourseNumber()) ||
          containsComma(course->getCourseName()) ||
          !isValidSchedule(course->getSchedule()) ||
          course->getPrice() <= 0.0) {
        std::cout << "Course data is invalid and cannot be saved.\n";
        return false;
      }
    }
  }

  std::ofstream file(filename);

  if (!file.is_open()) {
    std::cout << "Failed to open CSV file for writing.\n";
    return false;
  }

  file << TotalDepartments << '\n';
  file << std::fixed << std::setprecision(2);

  for (int i = 0; i < TotalDepartments; ++i) {
    const Department& department = StoreDepartments[i];

    file << department.getName() << ',' << department.getCourseCount() << '\n';

    for (std::size_t j = 0; j < department.getCourseCount(); ++j) {
      const Course* course = department.getCourse(j);

      file << course->getCourseNumber() << ',' << course->getCourseName() << ','
           << course->getSchedule() << ',' << course->getPrice() << '\n';
    }
  }

  file.close();

  if (file.fail()) {
    std::cout << "Failed while writing changes to the CSV file.\n";
    return false;
  }

  std::cout << "Changes saved to CSV successfully.\n";
  return true;
}
