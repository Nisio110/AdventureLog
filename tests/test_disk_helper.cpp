#include <gtest/gtest.h>
#include <stdexcept>
#include "Disk.h"

TEST(DiskHelperTest, StrToBoolTrue){
    EXPECT_TRUE(DiskHelper::strToBool("true"));
}

TEST(DiskHelperTest, StrToBoolFalse){
    EXPECT_FALSE(DiskHelper::strToBool("false"));
}

TEST(DiskHelperTest, StrToBoolThrowsOnUnknown){
    EXPECT_THROW(DiskHelper::strToBool("yes"), std::runtime_error);
}

TEST(DiskHelperTest, BoolToStrRoundtrip){
    EXPECT_EQ(DiskHelper::boolToStr(true), "true");
    EXPECT_EQ(DiskHelper::boolToStr(false), "false");
}
