#pragma once

#include "department.h"

#include <cstddef>
#include <iostream>
#include <string>

class Interface
{
private:
    std::istream* input;
    std::ostream* output;

protected:
    std::istream& getInput();
    std::ostream& getOutput() const;

    bool readChoice(const std::string& prompt,
        int minimum,
        int maximum,
        int& choice);

    bool readRequiredText(const std::string& prompt,
        std::string& value);

    bool readPositivePrice(const std::string& prompt,
        double& price);

    bool readSchedule(const std::string& prompt,
        std::string& schedule);

    void displayDepartments() const;

    bool selectDepartment(std::size_t& departmentIndex);

    void displayCourses(const Department& department) const;

public:
    Interface();

    Interface(std::istream& inputStream,
        std::ostream& outputStream);

    virtual ~Interface();

    virtual void run() = 0;
};