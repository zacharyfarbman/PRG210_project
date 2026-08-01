#pragma once

#include "interface.h"

class AdminInterface : public Interface
{
private:
    void showMainMenu() const;
    void listDepartments() const;
    void addDepartment();
    void addCourseToDepartment();
    void saveChanges();

public:
    AdminInterface();

    AdminInterface(std::istream& inputStream,
        std::ostream& outputStream);

    void run() override;
};
