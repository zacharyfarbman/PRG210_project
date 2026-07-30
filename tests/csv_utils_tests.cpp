#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

#include "../CSVUtils.h"

Department* StoreDepartments = nullptr;
int TotalDepartments = 0;

class CSVUtilsTest : public testing::Test {
 protected:
  const char* testFilename = "csv_utils_test_data.csv";

  void resetStore() {
    delete[] StoreDepartments;
    StoreDepartments = nullptr;
    TotalDepartments = 0;
  }

  void writeTestFile(const std::string& contents) {
    std::ofstream file(testFilename);

    ASSERT_TRUE(file.is_open());

    file << contents;
    file.close();

    ASSERT_FALSE(file.fail());
  }

  std::string readTestFile() {
    std::ifstream file(testFilename);

    EXPECT_TRUE(file.is_open());

    std::stringstream contents;
    contents << file.rdbuf();
    return contents.str();
  }

  void SetUp() override {
    resetStore();
    std::remove(testFilename);
  }

  void TearDown() override {
    resetStore();
    std::remove(testFilename);
  }
};

TEST_F(CSVUtilsTest, MissingFileReturnsFalseAndPreservesExistingData) {
  TotalDepartments = 1;
  StoreDepartments = new Department[TotalDepartments];
  StoreDepartments[0] = Department("Computer Programming");
  StoreDepartments[0].addCourse(
      Course("PRG210", "C++ Programming", "M/W", 499.99));

  EXPECT_FALSE(loadFromCSV(testFilename));

  ASSERT_EQ(TotalDepartments, 1);
  ASSERT_NE(StoreDepartments, nullptr);
  EXPECT_STREQ(StoreDepartments[0].getName(), "Computer Programming");
  ASSERT_EQ(StoreDepartments[0].getCourseCount(), 1U);
  ASSERT_NE(StoreDepartments[0].getCourse(0), nullptr);
  EXPECT_EQ(StoreDepartments[0].getCourse(0)->getCourseNumber(), "PRG210");
}

TEST_F(CSVUtilsTest, EmptyStoreSavesZeroDepartments) {
  ASSERT_TRUE(saveToCSV(testFilename));

  EXPECT_EQ(readTestFile(), "0\n");
}

TEST_F(CSVUtilsTest, SaveWritesStableCSVFormat) {
  TotalDepartments = 2;
  StoreDepartments = new Department[TotalDepartments];

  StoreDepartments[0] = Department("Computer Programming");
  StoreDepartments[0].addCourse(
      Course("PRG210", "C++ Programming", "M/W", 499.99));
  StoreDepartments[0].addCourse(
      Course("PRG250", "Database Systems", "T/R", 449.50));

  StoreDepartments[1] = Department("Mathematics");
  StoreDepartments[1].addCourse(
      Course("MTH200", "Discrete Mathematics", "W/F", 399.00));

  ASSERT_TRUE(saveToCSV(testFilename));

  const std::string expected =
      "2\n"
      "Computer Programming,2\n"
      "PRG210,C++ Programming,M/W,499.99\n"
      "PRG250,Database Systems,T/R,449.50\n"
      "Mathematics,1\n"
      "MTH200,Discrete Mathematics,W/F,399.00\n";

  EXPECT_EQ(readTestFile(), expected);
}

TEST_F(CSVUtilsTest, LoadReadsWhitespaceAndWindowsLineEndings) {
  writeTestFile(
      "2\r\n"
      "Computer Programming, 2\r\n"
      "PRG210, C++ Programming, M/W, 499.99\r\n"
      "PRG250, Database Systems, T/R, 449.50\r\n"
      "Mathematics, 1\r\n"
      "MTH200, Discrete Mathematics, W/F, 399.00\r\n");

  ASSERT_TRUE(loadFromCSV(testFilename));

  ASSERT_EQ(TotalDepartments, 2);
  ASSERT_NE(StoreDepartments, nullptr);

  EXPECT_STREQ(StoreDepartments[0].getName(), "Computer Programming");
  ASSERT_EQ(StoreDepartments[0].getCourseCount(), 2U);

  const Course* firstCourse = StoreDepartments[0].getCourse(0);
  const Course* secondCourse = StoreDepartments[0].getCourse(1);

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

  EXPECT_STREQ(StoreDepartments[1].getName(), "Mathematics");
  ASSERT_EQ(StoreDepartments[1].getCourseCount(), 1U);
  ASSERT_NE(StoreDepartments[1].getCourse(0), nullptr);
  EXPECT_EQ(StoreDepartments[1].getCourse(0)->getCourseNumber(), "MTH200");
}

TEST_F(CSVUtilsTest, SaveAndLoadRoundTripPreservesAllData) {
  TotalDepartments = 2;
  StoreDepartments = new Department[TotalDepartments];

  StoreDepartments[0] = Department("Computer Programming");
  StoreDepartments[0].addCourse(
      Course("PRG210", "C++ Programming", "M/W", 499.99));
  StoreDepartments[0].addCourse(
      Course("PRG250", "Database Systems", "T/R", 449.50));

  StoreDepartments[1] = Department("Mathematics");
  StoreDepartments[1].addCourse(
      Course("MTH200", "Discrete Mathematics", "W/F", 399.00));

  ASSERT_TRUE(saveToCSV(testFilename));

  resetStore();

  ASSERT_TRUE(loadFromCSV(testFilename));
  ASSERT_EQ(TotalDepartments, 2);

  EXPECT_STREQ(StoreDepartments[0].getName(), "Computer Programming");
  ASSERT_EQ(StoreDepartments[0].getCourseCount(), 2U);
  EXPECT_EQ(StoreDepartments[0].getCourse(0)->getCourseNumber(), "PRG210");
  EXPECT_EQ(StoreDepartments[0].getCourse(1)->getCourseNumber(), "PRG250");

  EXPECT_STREQ(StoreDepartments[1].getName(), "Mathematics");
  ASSERT_EQ(StoreDepartments[1].getCourseCount(), 1U);
  EXPECT_EQ(StoreDepartments[1].getCourse(0)->getCourseNumber(), "MTH200");
}

TEST_F(CSVUtilsTest, RepeatedRoundTripDoesNotAccumulateWhitespace) {
  TotalDepartments = 1;
  StoreDepartments = new Department[TotalDepartments];
  StoreDepartments[0] = Department("Computer Programming");
  StoreDepartments[0].addCourse(
      Course("PRG210", "C++ Programming", "M/W", 499.99));

  ASSERT_TRUE(saveToCSV(testFilename));
  const std::string firstSave = readTestFile();

  resetStore();

  ASSERT_TRUE(loadFromCSV(testFilename));
  ASSERT_TRUE(saveToCSV(testFilename));

  EXPECT_EQ(readTestFile(), firstSave);
}

TEST_F(CSVUtilsTest, NegativeDepartmentCountIsRejectedWithoutReplacingData) {
  TotalDepartments = 1;
  StoreDepartments = new Department[TotalDepartments];
  StoreDepartments[0] = Department("Existing Department");

  writeTestFile("-1\n");

  EXPECT_FALSE(loadFromCSV(testFilename));

  ASSERT_EQ(TotalDepartments, 1);
  ASSERT_NE(StoreDepartments, nullptr);
  EXPECT_STREQ(StoreDepartments[0].getName(), "Existing Department");
}

TEST_F(CSVUtilsTest, NonNumericDepartmentCountIsRejected) {
  writeTestFile("two\n");

  EXPECT_FALSE(loadFromCSV(testFilename));
  EXPECT_EQ(TotalDepartments, 0);
  EXPECT_EQ(StoreDepartments, nullptr);
}

TEST_F(CSVUtilsTest, MissingDepartmentRecordIsRejected) {
  writeTestFile("1\n");

  EXPECT_FALSE(loadFromCSV(testFilename));
  EXPECT_EQ(TotalDepartments, 0);
  EXPECT_EQ(StoreDepartments, nullptr);
}

TEST_F(CSVUtilsTest, MissingCourseRecordIsRejected) {
  writeTestFile(
      "1\n"
      "Computer Programming,1\n");

  EXPECT_FALSE(loadFromCSV(testFilename));
  EXPECT_EQ(TotalDepartments, 0);
  EXPECT_EQ(StoreDepartments, nullptr);
}

TEST_F(CSVUtilsTest, InvalidCoursePriceIsRejected) {
  writeTestFile(
      "1\n"
      "Computer Programming,1\n"
      "PRG210,C++ Programming,M/W,free\n");

  EXPECT_FALSE(loadFromCSV(testFilename));
  EXPECT_EQ(TotalDepartments, 0);
  EXPECT_EQ(StoreDepartments, nullptr);
}

TEST_F(CSVUtilsTest, InvalidCourseScheduleIsRejected) {
  writeTestFile(
      "1\n"
      "Computer Programming,1\n"
      "PRG210,C++ Programming,M/F,499.99\n");

  EXPECT_FALSE(loadFromCSV(testFilename));
  EXPECT_EQ(TotalDepartments, 0);
  EXPECT_EQ(StoreDepartments, nullptr);
}

TEST_F(CSVUtilsTest, SaveRejectsNamesContainingCommas) {
  TotalDepartments = 1;
  StoreDepartments = new Department[TotalDepartments];
  StoreDepartments[0] = Department("Business, Law");

  EXPECT_FALSE(saveToCSV(testFilename));
}

TEST_F(CSVUtilsTest, NullAndEmptyFilenamesAreRejected) {
  EXPECT_FALSE(loadFromCSV(nullptr));
  EXPECT_FALSE(saveToCSV(nullptr));
  EXPECT_FALSE(loadFromCSV(""));
  EXPECT_FALSE(saveToCSV(""));
}
