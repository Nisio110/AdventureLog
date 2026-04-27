#include <gtest/gtest.h>
#include "User.h"
#include "Log.h"

TEST(UserTest, ConstructorSetsName){
    User u("Alice", "secret");
    EXPECT_EQ(u.getName(), "Alice");
}

TEST(UserTest, ConstructorSetsPassword){
    User u("Alice", "secret");
    EXPECT_EQ(u.getPasswd(), "secret");
}

TEST(UserTest, ConstructorAssignsId){
    User u("Bob", "pw");
    EXPECT_GT(u.getId(), 0);
}

TEST(UserTest, EachConstructionGetsUniqueId){
    User a("a", "1");
    User b("b", "2");
    EXPECT_NE(a.getId(), b.getId());
}

TEST(UserTest, AddLogAppendsToLogsVector){
    User u("Alice", "secret");
    u.addLog(new CaveLog());
    EXPECT_EQ(u.getLogs().size(), 1u);
}
