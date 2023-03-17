#include <gtest/gtest.h>

#include "a_b.hpp"

TEST(AB_TEST, ab) {
  ASSERT_EQ(2, a_minus_b(4,2));
  //ASSERT_TRUE(a_minus_b(4,2) == 2);
}

TEST(AB_TEST, abc) {
  ASSERT_EQ(3, a_minus_b(4,1));
  ASSERT_TRUE(a_minus_b(4,2) == 2);
}


TEST(AB_TEST_NEGATIVE, ab) {
  ASSERT_EQ(-8, a_minus_b(4,12));
  //ASSERT_TRUE(a_minus_b(4,2) == 2);
}

TEST(AB_TEST_NEGATIVE, abc) {

  ASSERT_EQ(-1, a_minus_b(4,5));
  //ASSERT_TRUE(a_minus_b(4,2) == 2);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

