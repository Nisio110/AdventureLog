#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <vector>
#include "Disk.h"

// Boolean conversion -----------------------------------------------------

TEST(DiskHelperTest, StrToBoolTrue){
    EXPECT_TRUE(DiskHelper::strToBool("true"));
}

TEST(DiskHelperTest, StrToBoolFalse){
    EXPECT_FALSE(DiskHelper::strToBool("false"));
}

TEST(DiskHelperTest, StrToBoolThrowsOnUnknown){
    EXPECT_THROW(DiskHelper::strToBool("yes"), std::runtime_error);
    EXPECT_THROW(DiskHelper::strToBool(""), std::runtime_error);
}

TEST(DiskHelperTest, BoolToStrRoundtrip){
    EXPECT_EQ(DiskHelper::boolToStr(true), "true");
    EXPECT_EQ(DiskHelper::boolToStr(false), "false");
    EXPECT_TRUE(DiskHelper::strToBool(DiskHelper::boolToStr(true)));
    EXPECT_FALSE(DiskHelper::strToBool(DiskHelper::boolToStr(false)));
}

// Numeric conversion -----------------------------------------------------

TEST(DiskHelperTest, IntToStrPositive){
    EXPECT_EQ(DiskHelper::intToStr(42), "42");
}

TEST(DiskHelperTest, IntToStrZero){
    EXPECT_EQ(DiskHelper::intToStr(0), "0");
}

TEST(DiskHelperTest, IntToStrNegative){
    EXPECT_EQ(DiskHelper::intToStr(-7), "-7");
}

TEST(DiskHelperTest, StrToNumBasic){
    EXPECT_EQ(DiskHelper::strToNum("42"), 42u);
}

TEST(DiskHelperTest, StrToNumZero){
    EXPECT_EQ(DiskHelper::strToNum("0"), 0u);
}

TEST(DiskHelperTest, IntToStrAndStrToNumRoundtrip){
    for (int v : {0, 1, 42, 99999}) {
        EXPECT_EQ(DiskHelper::strToNum(DiskHelper::intToStr(v)),
                  static_cast<unsigned long>(v));
    }
}

// KeyValue accessors -----------------------------------------------------

TEST(DiskHelperTest, GetKeyAndGetValReadKVPair){
    KeyValue kv {"name", "Alice"};
    EXPECT_EQ(DiskHelper::getKey(kv), "name");
    EXPECT_EQ(DiskHelper::getVal(kv), "Alice");
}

TEST(DiskHelperTest, GetAttrKeyAndGetAttrValueReadByIndex){
    KeyValueList kvl {{"object", "User"}, {"id", "1"}, {"name", "Alice"}};
    EXPECT_EQ(DiskHelper::getAttrKey(kvl, 0), "object");
    EXPECT_EQ(DiskHelper::getAttrValue(kvl, 0), "User");
    EXPECT_EQ(DiskHelper::getAttrKey(kvl, 2), "name");
    EXPECT_EQ(DiskHelper::getAttrValue(kvl, 2), "Alice");
}

TEST(DiskHelperTest, GetKeyLocationFindsExistingKey){
    KeyValueList kvl {{"object", "User"}, {"id", "1"}, {"name", "Alice"}};
    EXPECT_EQ(DiskHelper::getKeyLocationInObj("name", kvl), 2u);
}

TEST(DiskHelperTest, GetKeyLocationThrowsWhenMissing){
    KeyValueList kvl {{"object", "User"}};
    EXPECT_THROW(DiskHelper::getKeyLocationInObj("nope", kvl), std::runtime_error);
}

// Substring search -------------------------------------------------------

TEST(DiskHelperTest, DoesSubstrExistFindsMatch){
    EXPECT_TRUE(DiskHelper::doesSubstrExist("hello world", "world"));
    EXPECT_TRUE(DiskHelper::doesSubstrExist("---", "---"));
}

TEST(DiskHelperTest, DoesSubstrExistRejectsNonMatch){
    EXPECT_FALSE(DiskHelper::doesSubstrExist("hello", "xyz"));
}

// strToKVPair ------------------------------------------------------------

TEST(DiskHelperTest, StrToKVPairParsesBasicKVLine){
    auto kv = DiskHelper::strToKVPair("name: Alice");
    EXPECT_EQ(DiskHelper::getKey(kv), "name");
    EXPECT_EQ(DiskHelper::getVal(kv), "Alice");
}

TEST(DiskHelperTest, StrToKVPairPreservesSpacesInValue){
    auto kv = DiskHelper::strToKVPair("area: Near Lisdoonvarna, Clare");
    EXPECT_EQ(DiskHelper::getKey(kv), "area");
    EXPECT_EQ(DiskHelper::getVal(kv), "Near Lisdoonvarna, Clare");
}

TEST(DiskHelperTest, StrToKVPairOnDividerYieldsDividerKey){
    auto kv = DiskHelper::strToKVPair("---");
    EXPECT_EQ(DiskHelper::getKey(kv), "---");
    EXPECT_EQ(DiskHelper::getVal(kv), "");
}

TEST(DiskHelperTest, StrToKVPairOnNonKvNonDividerYieldsEmpty){
    auto kv = DiskHelper::strToKVPair("garbage line");
    EXPECT_EQ(DiskHelper::getKey(kv), "");
    EXPECT_EQ(DiskHelper::getVal(kv), "");
}

TEST(DiskHelperTest, StrVecToKVLConvertsEachLine){
    std::vector<std::string> lines {"object: User", "id: 1", "name: Alice"};
    auto kvl = DiskHelper::StrVecToKVL(lines);
    ASSERT_EQ(kvl.size(), 3u);
    EXPECT_EQ(DiskHelper::getAttrKey(kvl, 0), "object");
    EXPECT_EQ(DiskHelper::getAttrValue(kvl, 1), "1");
    EXPECT_EQ(DiskHelper::getAttrValue(kvl, 2), "Alice");
}
