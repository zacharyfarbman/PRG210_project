#include "../cart.h"

#include <gtest/gtest.h>

TEST(CartTest, DefaultConstructorCreatesEmptyCart) {
  const Cart cart;

  EXPECT_TRUE(cart.isEmpty());
  EXPECT_EQ(cart.getCourseCount(), 0U);
  EXPECT_DOUBLE_EQ(cart.getTotalCost(), 0.0);
}

TEST(CartTest, AddCourseUpdatesCountAndTotalWithTax) {
  Cart cart;
  const Course course("PRG210", "C++ Programming", "M/W", 500.00);

  cart.addCourse(course);

  EXPECT_FALSE(cart.isEmpty());
  EXPECT_EQ(cart.getCourseCount(), 1U);
  EXPECT_NEAR(cart.getTotalCost(), 565.00, 0.001);
}

TEST(CartTest, AddMultipleCoursesAccumulatesTotalWithTax) {
  Cart cart;

  cart.addCourse(Course("PRG210", "C++ Programming", "M/W", 500.00));
  cart.addCourse(Course("NTF201", "Networking Fundamentals", "T/R", 450.00));
  cart.addCourse(Course("MTH200", "Discrete Mathematics", "W/F", 300.00));

  EXPECT_FALSE(cart.isEmpty());
  EXPECT_EQ(cart.getCourseCount(), 3U);
  EXPECT_NEAR(cart.getTotalCost(), 1412.50, 0.001);
}

TEST(CartTest, ClearCartResetsAllPublicState) {
  Cart cart;

  cart.addCourse(Course("PRG210", "C++ Programming", "M/W", 500.00));
  cart.addCourse(Course("NTF201", "Networking Fundamentals", "T/R", 450.00));

  cart.clearCart();

  EXPECT_TRUE(cart.isEmpty());
  EXPECT_EQ(cart.getCourseCount(), 0U);
  EXPECT_DOUBLE_EQ(cart.getTotalCost(), 0.0);
}

TEST(CartTest, CartCanBeReusedAfterBeingCleared) {
  Cart cart;

  cart.addCourse(Course("PRG210", "C++ Programming", "M/W", 500.00));
  cart.clearCart();

  cart.addCourse(Course("NTF201", "Networking Fundamentals", "T/R", 450.00));

  EXPECT_FALSE(cart.isEmpty());
  EXPECT_EQ(cart.getCourseCount(), 1U);
  EXPECT_NEAR(cart.getTotalCost(), 508.50, 0.001);
}
