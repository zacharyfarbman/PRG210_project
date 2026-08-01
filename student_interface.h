#pragma once

#include "cart.h"
#include "interface.h"

class StudentInterface : public Interface
{
private:
    Cart cart;

    void showMainMenu() const;
    void showCartMenu();
    void browseDepartments();
    void checkout();

public:
    StudentInterface();

    StudentInterface(std::istream& inputStream,
        std::ostream& outputStream);

    void run() override;
};