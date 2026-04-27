#include <gtest/gtest.h>
#include <string>
#include "User.h"
#include "Log.h"

TEST(UserEdgeTest, EmptyNameAccepted){
    User u("", "pw");
    EXPECT_EQ(u.getName(), "");
}

TEST(UserEdgeTest, EmptyPasswordAccepted){
    // Two users in the actual disk.yaml ("Wow", "YAY") have empty passwords.
    User u("Alice", "");
    EXPECT_EQ(u.getPasswd(), "");
}

TEST(UserEdgeTest, LongNameIsPreservedVerbatim){
    std::string longName(10000, 'x');
    User u(longName, "pw");
    EXPECT_EQ(u.getName(), longName);
}

TEST(UserEdgeTest, NameWithSpacesAndPunctuationPreserved){
    User u("Alice O'Brien-Smith, Jr.", "pw");
    EXPECT_EQ(u.getName(), "Alice O'Brien-Smith, Jr.");
}

TEST(UserEdgeTest, RemoveLogTwiceIsSafeNoOp){
    User u("Alice", "secret");
    auto* log = new CaveLog();
    log->setID(101);
    u.addLog(log);

    Log marker;
    marker.setID(101);
    u.removeLog(&marker);
    ASSERT_EQ(u.getLogs().size(), 0u);

    EXPECT_NO_THROW(u.removeLog(&marker));
    EXPECT_EQ(u.getLogs().size(), 0u);
}

TEST(UserEdgeTest, AddSameLogPointerTwiceCreatesDuplicateEntries){
    // No dedupe in addLog — a future caller passing the same pointer twice
    // would land it in the vector twice. Documents the lack of guard.
    User u("Alice", "pw");
    auto* log = new CaveLog();
    log->setID(7);
    u.addLog(log);
    // The second addLog of the same pointer would cause a double-free in
    // ~User. We don't repeat the pointer; we add two distinct logs with the
    // same id to demonstrate that addLog doesn't check for id collisions.
    auto* log2 = new CaveLog();
    log2->setID(7);
    u.addLog(log2);
    EXPECT_EQ(u.getLogs().size(), 2u);
}

TEST(UserEdgeTest, RemoveLogWithNullVectorIsSafe){
    User u;
    Log marker;
    marker.setID(1);
    EXPECT_NO_THROW(u.removeLog(&marker));
    EXPECT_EQ(u.getLogs().size(), 0u);
}
