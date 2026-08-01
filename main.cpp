#include <iostream>

#include "course.h"
#include "department.h"
#include "student_interface.h"
#include "CSVUtils.h"

// Global department storage used by the CSV utility functions.
Department* StoreDepartments = nullptr;
int TotalDepartments = 0;
const char* csvFile = "courses.csv";

int main() {

  loadFromCSV(csvFile);

  StudentInterface student;
  student.run();

  // Release the global dynamic department array.
  delete[] StoreDepartments;
  StoreDepartments = nullptr;
  TotalDepartments = 0;

  return 0;
}
