## Recommended architecture

Inheritance and file separation are independent. Both derived classes still inherit from `Interface`, but separate files will let you and your partner work concurrently with fewer merge conflicts:

```text
interface.h / interface.cpp
admin_interface.h / admin_interface.cpp
student_interface.h / student_interface.cpp
```

All data members remain private.

### Base `Interface`

Responsibilities shared by both roles:

```cpp
class Interface {
private:
    std::istream* input;
    std::ostream* output;

protected:
    std::istream& getInput();
    std::ostream& getOutput();

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
    Interface(std::istream& input, std::ostream& output);
    virtual ~Interface();

    virtual void run() = 0;
};
```

The second constructor allows tests to provide `std::stringstream` input and output instead of changing `std::cin` and `std::cout`.

Every validation function should:

* Print the prompt
* Read an entire line
* Reject malformed or out-of-range input
* Print a helpful error
* Repeat the prompt
* Return `false` if end-of-input is reached, preventing infinite loops in tests

For CSV-backed text, reject empty, whitespace-only, and comma-containing values.

## Work division

Since you implemented Department/CSV work and your partner implemented Cart, the natural split is:

| Branch                         | Owner                      | Responsibility                                       |
| ------------------------------ | -------------------------- | ---------------------------------------------------- |
| `feature/interface-foundation` | Shared/you                 | Base class, validation, test globals, file skeletons |
| `feature/admin-interface`      | You                        | Departments, course creation, CSV saving             |
| `feature/student-interface`    | Partner                    | Browsing, cart menus, checkout                       |
| `feature/main-loop`            | Either, after merging both | Role selection, polymorphism, cleanup                |

Do not create the admin and student branches until `feature/interface-foundation` has been merged.

## Phase 1: Interface foundation

Create:

```text
interface.h
interface.cpp
admin_interface.h
admin_interface.cpp
student_interface.h
student_interface.cpp
tests/interface_tests.cpp
tests/admin_interface_tests.cpp
tests/student_interface_tests.cpp
tests/test_globals.cpp
```

The foundation branch should:

* Implement the base constructors and virtual destructor
* Implement all shared validation functions
* Implement shared department/course listing
* Declare both derived classes
* Register all new `.cpp` and test files in CMake
* Unit-test the validation methods
* Leave the role-specific test files empty initially

Move test globals out of `csv_utils_tests.cpp` into `tests/test_globals.cpp`:

```cpp
#include "../department.h"

Department* StoreDepartments = nullptr;
int TotalDepartments = 0;
const char* csvFile = "test_courses.csv";
```

Remove the existing `StoreDepartments` and `TotalDepartments` definitions from `csv_utils_tests.cpp`.

Foundation tests should cover:

* Valid menu choice
* Non-numeric menu input
* Partially numeric input such as `2abc`
* Out-of-range choices
* Empty and whitespace-only text
* Commas in CSV-backed text
* Invalid, zero, and negative prices
* Every permitted schedule
* Invalid schedules
* End-of-input handling

Suggested commit:

```bash
git commit -m "Add shared interface foundation"
```

## Phase 2A: Admin interface

`AdminInterface` should have no additional data members. Its private functions can be:

```cpp
void showMainMenu();
void listDepartments();
void addDepartment();
void addCourseToDepartment();
void saveChanges();
```

`run()` controls the main admin loop.

### Adding a department

Resize the global dynamic array:

1. Allocate `TotalDepartments + 1` departments.
2. Copy existing departments using their deep-copy assignment operator.
3. Assign the new department into the final position.
4. Delete the old array.
5. Update `StoreDepartments`.
6. Increment `TotalDepartments`.

### Admin tests

Test:

* Immediate exit
* Invalid main-menu choices
* Empty department listing
* Numbered department listing
* Adding the first department
* Adding another department without losing existing courses
* Selecting `0` to go back
* Invalid department selections
* Invalid course number/name/schedule/price
* Adding courses to departments in the order department 1, department 2, department 1
* Successful CSV save
* Failed CSV save

Suggested commits:

```bash
git commit -m "Implement admin department management"
git commit -m "Add admin interface tests"
```

## Phase 2B: Student interface

`StudentInterface` owns its cart:

```cpp
class StudentInterface : public Interface {
private:
    Cart cart;

    void showCartMenu();
    void browseDepartmentsMenu();
    void browseDepartmentCourses();
    void addCourseToCart(const Department& department);
    void listCartCourses() const;

public:
    StudentInterface();
    StudentInterface(std::istream& input, std::ostream& output);

    void run() override;
};
```

Add one read-only accessor to `Cart`:

```cpp
const Course* getCourse(std::size_t index) const;
```

It should return `nullptr` for an invalid index, matching `Department::getCourse()`. This allows the interface to print cart contents through its injected output stream.

### Student tests

Test:

* Immediate exit
* Empty cart listing
* Empty-cart checkout remains in the cart menu
* Department and course numbering
* Invalid department/course selections
* Every `0`/back navigation path
* Adding one course
* Adding several courses
* Cart total includes 13% tax
* Successful checkout clears the cart
* Returning from checkout to the student main menu
* Reusing the cart after checkout

Suggested commits:

```bash
git commit -m "Implement student course browsing"
git commit -m "Implement student cart checkout"
git commit -m "Add student interface tests"
```

## Phase 3: Main-loop integration

After both role branches are merged, replace the temporary Cart demonstration in `main.cpp`.

The final flow should be:

1. Define the three required globals.
2. Call `loadFromCSV(csvFile)`.
3. Validate the initial Student/Admin/Exit selection.
4. Allocate the selected derived interface through an `Interface*`.
5. Call `run()` polymorphically.
6. Delete the interface through the virtual base destructor.
7. Delete `StoreDepartments`.
8. Exit.

The role-specific Exit option should end the program, not return to role selection, because both interfaces specify “Exit Program.”

Canonical CSV course records should remain:

```text
courseNumber,courseName,schedule,price
```

The assignment’s final CSV example omits the course number, but that conflicts with its course-number prompt and your existing tested CSV implementation.

## Merge order

```text
feature/interface-foundation
             ↓
          main
         ↙    ↘
feature/admin-interface
feature/student-interface
         ↘    ↙
          main
             ↓
    feature/main-loop
```

Before starting, verify the merged branch because the attached `main.cpp`, `CSVUtils.cpp`, and CMake snapshot appear older than the CSV integration you just tested:

```bash
git switch main
git pull --ff-only origin main
git status
git log --oneline -5
ctest --test-dir build --output-on-failure
```
