#include <iostream>
#include <sstream>
#include <string>

#include "CSVUtils.h"
#include "admin_interface.h"
#include "course.h"
#include "department.h"
#include "student_interface.h"

// Global department storage used by the CSV utility functions.
Department *StoreDepartments = nullptr;
int TotalDepartments = 0;
const char *csvFile = "courses.csv";

namespace {
/*
 * Displays the initial role menu and repeats the prompt until the use enters
 * one of the two valid role numbers. A value of zero indicates that the input
 * stream was closed before a selection was made.
 * */
int readRoleChoice() {
  std::cout << "===== Course Management System =====\n";
  std::cout << "1. Student\n";
  std::cout << "2. Admin\n";

  std::string line;

  while (true) {
    std::cout << "Enter your choice [1, 2]: ";

    if (!std::getline(std::cin, line)) {
      return 0;
    }

    std::stringstream stream(line);
    int choice = 0;
    char extraCharacter = '\0';

    if ((stream >> choice) && !(stream >> extraCharacter) && choice >= 1 &&
        choice <= 2) {
      return choice;
    }

    std::cout << "Please enter either 1 for Student or 2 for Admin.\n";
  }
}
} // namespace

int main() {
  // Load saved departments and courses if the CSV file exists. A missing
  // file is not fatal; the application will begin with an empty store.
  loadFromCSV(csvFile);

  const int roleChoice = readRoleChoice();
  Interface *selectedInterface = nullptr;

  if (roleChoice == 1) {
    selectedInterface = new StudentInterface();
  } else if (roleChoice == 2) {
    selectedInterface = new AdminInterface();
  }

  // Calling run() through the base pointer demonstrates polymorphism.
  if (selectedInterface != nullptr) {
    selectedInterface->run();
    delete selectedInterface;
    selectedInterface = nullptr;
  }

  // Release the global dynamic department array before the program exits.
  delete[] StoreDepartments;
  StoreDepartments = nullptr;
  TotalDepartments = 0;

  return 0;
}
