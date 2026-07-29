#include "../department.h"

#include <gtest/gtest.h>

TEST(DepartmentTest, DefaultConstructorCreatesEmptyDepartment) {
  Department department;

  EXPECT_STREQ(department.getName(), "");
  EXPECT_EQ(department.getCourseCount(), 0U);
  EXPECT_EQ(department.getCourse(0), nullptr);
}

TEST(DepartmentTest, NamedConstructorStoresDepartmentName) {
  const Department department("Computer Programming");

  EXPECT_STREQ(department.getName(), "Computer Programming");
  EXPECT_EQ(department.getCourseCount(), 0U);
}

TEST(DepartmentTest, AddCourseAppendsCoursesInOrder) {
  Department department("Computer Programming");

  department.addCourse(Course("PRG210", "C++ Programming", "M/W", 499.99));
  department.addCourse(Course("PRG250", "Database Systems", "T/R", 449.50));

  ASSERT_EQ(department.getCourseCount(), 2U);

  const Course *firstCourse = department.getCourse(0);
  const Course *secondCourse = department.getCourse(1);

  ASSERT_NE(firstCourse, nullptr);
  ASSERT_NE(secondCourse, nullptr);

  EXPECT_EQ(firstCourse->getCourseNumber(), "PRG210");
  EXPECT_EQ(firstCourse->getCourseName(), "C++ Programming");
  EXPECT_EQ(firstCourse->getSchedule(), "M/W");
  EXPECT_DOUBLE_EQ(firstCourse->getPrice(), 499.99);

  EXPECT_EQ(secondCourse->getCourseNumber(), "PRG250");
  EXPECT_EQ(secondCourse->getCourseName(), "Database Systems");
  EXPECT_EQ(secondCourse->getSchedule(), "T/R");
  EXPECT_DOUBLE_EQ(secondCourse->getPrice(), 449.50);
}

TEST(DepartmentTest, GetCourseRejectsInvalidIndexes) {
  Department department("Computer Programming");
  department.addCourse(Course("PRG210", "C++ Programming", "M/W", 499.99));

  EXPECT_EQ(department.getCourse(1), nullptr);
  EXPECT_EQ(department.getCourse(100), nullptr);
}

TEST(DepartmentTest, ConstDepartmentProvidesReadOnlyCourseAccess) {
  Department department("Computer Programming");
  department.addCourse(Course("PRG210", "C++ Programming", "M/W", 499.99));

  const Department &departmentView = department;
  const Course *course = departmentView.getCourse(0);

  ASSERT_NE(course, nullptr);
  EXPECT_EQ(course->getCourseNumber(), "PRG210");
}

TEST(DepartmentTest, CopyConstructorPerformsDeepCopy) {
  Department original("Computer Programming");
  original.addCourse(Course("PRG210", "C++ Programming", "M/W", 499.99));

  Department copy(original);

  ASSERT_EQ(copy.getCourseCount(), 1U);
  ASSERT_NE(original.getCourse(0), nullptr);
  ASSERT_NE(copy.getCourse(0), nullptr);

  EXPECT_STREQ(copy.getName(), "Computer Programming");
  EXPECT_NE(copy.getName(), original.getName());
  EXPECT_NE(copy.getCourse(0), original.getCourse(0));
  EXPECT_EQ(copy.getCourse(0)->getCourseNumber(), "PRG210");

  original.addCourse(Course("PRG250", "Database Systems", "T/R", 449.50));

  EXPECT_EQ(original.getCourseCount(), 2U);
  EXPECT_EQ(copy.getCourseCount(), 1U);
}

TEST(DepartmentTest, AssignmentOperatorPerformsDeepCopy) {
  Department source("Computer Programming");
  source.addCourse(Course("PRG210", "C++ Programming", "M/W", 499.99));

  Department destination("Mathematics");
  destination.addCourse(
      Course("MTH200", "Discrete Mathematics", "W/F", 399.00));

  Department &assignmentResult = (destination = source);

  EXPECT_EQ(&assignmentResult, &destination);
  ASSERT_EQ(destination.getCourseCount(), 1U);
  ASSERT_NE(source.getCourse(0), nullptr);
  ASSERT_NE(destination.getCourse(0), nullptr);

  EXPECT_STREQ(destination.getName(), "Computer Programming");
  EXPECT_NE(destination.getName(), source.getName());
  EXPECT_NE(destination.getCourse(0), source.getCourse(0));
  EXPECT_EQ(destination.getCourse(0)->getCourseNumber(), "PRG210");

  source.addCourse(Course("PRG250", "Database Systems", "T/R", 449.50));

  EXPECT_EQ(source.getCourseCount(), 2U);
  EXPECT_EQ(destination.getCourseCount(), 1U);
}

TEST(DepartmentTest, SelfAssignmentPreservesDepartment) {
  Department department("Computer Programming");
  department.addCourse(Course("PRG210", "C++ Programming", "M/W", 499.99));

  Department &assignmentResult = (department = department);

  EXPECT_EQ(&assignmentResult, &department);
  EXPECT_STREQ(department.getName(), "Computer Programming");
  ASSERT_EQ(department.getCourseCount(), 1U);
  ASSERT_NE(department.getCourse(0), nullptr);
  EXPECT_EQ(department.getCourse(0)->getCourseNumber(), "PRG210");
}

TEST(DepartmentTest, EmptyDepartmentCanBeCopiedAndAssigned) {
  Department empty;
  Department copy(empty);
  Department destination("Temporary");

  destination = empty;

  EXPECT_STREQ(copy.getName(), "");
  EXPECT_EQ(copy.getCourseCount(), 0U);
  EXPECT_EQ(copy.getCourse(0), nullptr);

  EXPECT_STREQ(destination.getName(), "");
  EXPECT_EQ(destination.getCourseCount(), 0U);
  EXPECT_EQ(destination.getCourse(0), nullptr);
}
