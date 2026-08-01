#include "interface.h"

#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

extern Department* StoreDepartments;
extern int TotalDepartments;

namespace
{
    std::string trimText(const std::string& text)
    {
        std::size_t first = 0;

        while (first < text.length() &&
            std::isspace(static_cast<unsigned char>(text[first])))
        {
            ++first;
        }

        std::size_t last = text.length();

        while (last > first &&
            std::isspace(static_cast<unsigned char>(text[last - 1])))
        {
            --last;
        }

        return text.substr(first, last - first);
    }
}

Interface::Interface()
    : input(&std::cin),
    output(&std::cout)
{
}

Interface::Interface(std::istream& inputStream,
    std::ostream& outputStream)
    : input(&inputStream),
    output(&outputStream)
{
}

Interface::~Interface() = default;

std::istream& Interface::getInput()
{
    return *input;
}

std::ostream& Interface::getOutput() const
{
    return *output;
}

bool Interface::readChoice(const std::string& prompt,
    int minimum,
    int maximum,
    int& choice)
{
    std::string line;

    while (true)
    {
        getOutput() << prompt;

        if (!std::getline(getInput(), line))
        {
            return false;
        }

        std::stringstream stream(line);

        int parsedChoice = 0;
        char extraCharacter = '\0';

        if (!(stream >> parsedChoice) ||
            (stream >> extraCharacter) ||
            parsedChoice < minimum ||
            parsedChoice > maximum)
        {
            getOutput()
                << "Please enter a valid number between "
                << minimum
                << " and "
                << maximum
                << ".\n";

            continue;
        }

        choice = parsedChoice;
        return true;
    }
}

bool Interface::readRequiredText(const std::string& prompt,
    std::string& value)
{
    std::string line;

    while (true)
    {
        getOutput() << prompt;

        if (!std::getline(getInput(), line))
        {
            return false;
        }

        const std::string trimmedLine = trimText(line);

        if (trimmedLine.empty())
        {
            getOutput() << "Input cannot be empty.\n";
            continue;
        }

        if (trimmedLine.find(',') != std::string::npos)
        {
            getOutput() << "Input cannot contain commas.\n";
            continue;
        }

        value = trimmedLine;
        return true;
    }
}

bool Interface::readPositivePrice(const std::string& prompt,
    double& price)
{
    std::string line;

    while (true)
    {
        getOutput() << prompt;

        if (!std::getline(getInput(), line))
        {
            return false;
        }

        std::stringstream stream(line);

        double parsedPrice = 0.0;
        char extraCharacter = '\0';

        if (!(stream >> parsedPrice) ||
            (stream >> extraCharacter) ||
            parsedPrice <= 0.0)
        {
            getOutput()
                << "Please enter a valid positive price.\n";

            continue;
        }

        price = parsedPrice;
        return true;
    }
}

bool Interface::readSchedule(const std::string& prompt,
    std::string& schedule)
{
    std::string line;

    while (true)
    {
        getOutput() << prompt;

        if (!std::getline(getInput(), line))
        {
            return false;
        }

        const std::string trimmedLine = trimText(line);

        if (trimmedLine == "M/W" ||
            trimmedLine == "T/R" ||
            trimmedLine == "W/F")
        {
            schedule = trimmedLine;
            return true;
        }

        getOutput()
            << "Please enter one of the following schedules: "
            << "M/W, T/R, or W/F.\n";
    }
}

void Interface::displayDepartments() const
{
    if (TotalDepartments <= 0 ||
        StoreDepartments == nullptr)
    {
        getOutput()
            << "No departments are available.\n";

        return;
    }

    for (int i = 0; i < TotalDepartments; ++i)
    {
        getOutput()
            << i + 1
            << ". "
            << StoreDepartments[i].getName()
            << '\n';
    }
}

bool Interface::selectDepartment(
    std::size_t& departmentIndex)
{
    if (TotalDepartments <= 0 ||
        StoreDepartments == nullptr)
    {
        getOutput()
            << "No departments are available.\n";

        return false;
    }

    displayDepartments();

    int choice = 0;

    if (!readChoice(
        "Enter department number [0 to go back]: ",
        0,
        TotalDepartments,
        choice))
    {
        return false;
    }

    if (choice == 0)
    {
        return false;
    }

    departmentIndex =
        static_cast<std::size_t>(choice - 1);

    return true;
}

void Interface::displayCourses(
    const Department& department) const
{
    if (department.getCourseCount() == 0)
    {
        getOutput()
            << "No courses are available in this department.\n";

        return;
    }

    for (std::size_t i = 0;
        i < department.getCourseCount();
        ++i)
    {
        const Course* course =
            department.getCourse(i);

        if (course == nullptr)
        {
            continue;
        }

        getOutput()
            << i + 1
            << ". "
            << course->getCourseNumber()
            << " - "
            << course->getCourseName()
            << " | Schedule: "
            << course->getSchedule()
            << " | Price: $"
            << std::fixed
            << std::setprecision(2)
            << course->getPrice()
            << '\n';
    }
}