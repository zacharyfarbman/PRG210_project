#include <iostream>

#include "cart.h"
#include "course.h"
#include "department.h"
#include "admin_interface.h"
#include "student_interface.h"
#include "CSVUtils.h"

// Global department storage used by the CSV utility functions.
Department* StoreDepartments = nullptr;
int TotalDepartments = 0;
const char* csvFile = "courses.csv";

int main() {

  Cart cart;

  const Course course1("PRG210", "Programming 2", "M/W", 500.00);

  const Course course2("NTF201", "Networking Fundamentals", "T/R", 450.00);

  std::cout << "Course Management System\n";

  cart.addCourse(course1);
  cart.addCourse(course2);

  cart.displayCourses();

  std::cout << "Course count: " << cart.getCourseCount() << '\n';

  std::cout << std::boolalpha << "Cart is empty: " << cart.isEmpty() << '\n';

  cart.clearCart();

  std::cout << "Course count after clearing: " << cart.getCourseCount() << '\n';

  std::cout << std::boolalpha
            << "Cart is empty after clearing: " << cart.isEmpty() << '\n';
  loadFromCSV(csvFile);

  //AdminInterface admin;
  //admin.run();

  StudentInterface student;
  student.run();

  // Release the global dynamic department array.
  delete[] StoreDepartments;
  StoreDepartments = nullptr;
  TotalDepartments = 0;

  return 0;
}
