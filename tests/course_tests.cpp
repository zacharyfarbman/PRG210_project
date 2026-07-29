#include "../course.h"

#include <gtest/gtest.h>

TEST(CourseTest, DefaultConstructorCreatesEmptyCourse) {
  const Course course;

  EXPECT_TRUE(course.getCourseNumber().empty());
  EXPECT_TRUE(course.getCourseName().empty());
  EXPECT_TRUE(course.getSchedule().empty());
  EXPECT_DOUBLE_EQ(course.getPrice(), 0.0);
}

TEST(CourseTest, ParameterizedConstructorStoresCourseData) {
  const Course course("PRG210", "C++ Programming", "M/W", 499.99);

  EXPECT_EQ(course.getCourseNumber(), "PRG210");
  EXPECT_EQ(course.getCourseName(), "C++ Programming");
  EXPECT_EQ(course.getSchedule(), "M/W");
  EXPECT_DOUBLE_EQ(course.getPrice(), 499.99);
}
