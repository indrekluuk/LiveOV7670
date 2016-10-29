//
// Created by indrek on 29.10.2016.
//


#include "gtest/gtest.h"
#include "Wire.h"

TEST(basic_check2, test_eq) {
  EXPECT_EQ(1, 0);
  Wire.begin();
}

TEST(basic_check2, test_neq) {
  EXPECT_NE(1, 0);
}

