#include <iostream>
#include "cart.h"
#include "course.h"


int main() {
  
  Cart cart;

  Course course1("PRG210", "Programming 2", "M/W", 500.00);
  Course course2("NTF201", "Networking Fundamentals", "T/R", 450.00);
  std::cout << "Course Management System" << std::endl;
  cart.addCourse(course1);
  cart.addCourse(course2);

  cart.displayCourses();

  std::cout << "Course count: "
      << cart.getCourseCount()
      << std::endl;

  std::cout << std::boolalpha
      << "Cart is empty: "
      << cart.isEmpty()
      << std::endl;

  cart.clearCart();

  std::cout << "Course count after clearing: "
      << cart.getCourseCount()
      << std::endl;

  std::cout << std::boolalpha
      << "Cart is empty after clearing: "
      << cart.isEmpty()
      << std::endl;

  return 0;
}
