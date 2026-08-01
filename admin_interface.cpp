#include "admin_interface.h"
#include "CSVUtils.h"
#include <string>

extern Department* StoreDepartments;
extern int TotalDepartments;
extern const char* csvFile;

AdminInterface::AdminInterface()
    : Interface()
{
}

AdminInterface::AdminInterface(std::istream& inputStream,
    std::ostream& outputStream)
    : Interface(inputStream, outputStream)
{
}

void AdminInterface::showMainMenu() const
{
    getOutput() << "\n===== Admin Menu =====\n";
    getOutput() << "1. List departments\n";
    getOutput() << "2. Add department\n";
    getOutput() << "3. Add course to department\n";
    getOutput() << "4. Save changes\n";
    getOutput() << "5. Exit\n";
}

void AdminInterface::listDepartments() const
{
    getOutput() << "\n===== Departments =====\n";
    displayDepartments();
}

void AdminInterface::addDepartment()
{
    std::string departmentName;

    if (!readRequiredText("Enter the new department name: ",
        departmentName))
    {
        return;
    }

    Department* updatedDepartments =
        new Department[TotalDepartments + 1];

    for (int i = 0; i < TotalDepartments; ++i)
    {
        updatedDepartments[i] = StoreDepartments[i];
    }

    updatedDepartments[TotalDepartments] =
        Department(departmentName);

    delete[] StoreDepartments;

    StoreDepartments = updatedDepartments;
    ++TotalDepartments;

    getOutput() << "Department added successfully.\n";
}

void AdminInterface::addCourseToDepartment()
{
    std::size_t departmentIndex = 0;

    if (!selectDepartment(departmentIndex))
    {
        return;
    }

    getOutput() << "\n===== Existing Courses =====\n";
    displayCourses(StoreDepartments[departmentIndex]);

    std::string courseNumber;
    std::string courseName;
    std::string schedule;
    double price = 0.0;

    if (!readRequiredText("Enter course number: ", courseNumber))
    {
        return;
    }

    if (!readRequiredText("Enter course name: ", courseName))
    {
        return;
    }

    if (!readSchedule("Enter course schedule: ", schedule))
    {
        return;
    }

    if (!readPositivePrice("Enter course price: ", price))
    {
        return;
    }

    Course newCourse(courseNumber, courseName, schedule, price);

    StoreDepartments[departmentIndex].addCourse(newCourse);

    getOutput() << "Course added successfully.\n";
}

void AdminInterface::saveChanges()
{
    saveToCSV(csvFile);
}

void AdminInterface::run()
{
    int choice = 0;
    bool running = true;

    while (running)
    {
        showMainMenu();

        if (!readChoice("Enter your choice: ", 1, 5, choice))
        {
            return;
        }

        switch (choice)
        {
        case 1:
            listDepartments();
            break;

        case 2:
            addDepartment();
            break;

        case 3:
            addCourseToDepartment();
            break;

        case 4:
            saveChanges();
            break;

        case 5:
            running = false;
            break;
        }
    }
}