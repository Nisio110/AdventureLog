#include <gtest/gtest.h>
#include "User.h"
#include "Log.h"

TEST(UserTest, DefaultConstructorLeavesIdZeroAndDefaultName){
    User u;
    EXPECT_EQ(u.getId(), 0);
    EXPECT_EQ(u.getName(), "N/A");
    EXPECT_TRUE(u.getLogs().empty());
}

TEST(UserTest, NameOnlyConstructorSetsNameAndAssignsId){
    User u("Alice");
    EXPECT_EQ(u.getName(), "Alice");
    EXPECT_GT(u.getId(), 0);
}

TEST(UserTest, FullConstructorSetsAllFields){
    User u("Alice", "secret");
    EXPECT_EQ(u.getName(), "Alice");
    EXPECT_EQ(u.getPasswd(), "secret");
    EXPECT_GT(u.getId(), 0);
}

TEST(UserTest, EachConstructionGetsUniqueId){
    User a("a", "1");
    User b("b", "2");
    EXPECT_NE(a.getId(), b.getId());
}

TEST(UserTest, SettersOverrideValues){
    User u("a", "1");
    u.setID(999);
    u.setName("Zelda");
    u.setPasswd("triforce");
    EXPECT_EQ(u.getId(), 999);
    EXPECT_EQ(u.getName(), "Zelda");
    EXPECT_EQ(u.getPasswd(), "triforce");
}

TEST(UserTest, AddLogAppendsToLogsVector){
    User u("Alice", "secret");
    u.addLog(new CaveLog());
    EXPECT_EQ(u.getLogs().size(), 1u);
}

TEST(UserTest, AddMultipleLogsAccumulates){
    User u("Alice", "secret");
    u.addLog(new CaveLog());
    u.addLog(new HikeLog());
    u.addLog(new CaveLog());
    EXPECT_EQ(u.getLogs().size(), 3u);
}

TEST(UserTest, RemoveLogMatchesByIdAndDropsThatLog){
    User u("Alice", "secret");
    auto* keep = new CaveLog();
    auto* drop = new HikeLog();
    keep->setID(101);
    drop->setID(202);
    u.addLog(keep);
    u.addLog(drop);

    Log marker;
    marker.setID(202);
    u.removeLog(&marker);

    ASSERT_EQ(u.getLogs().size(), 1u);
    EXPECT_EQ(u.getLogs().at(0)->getId(), 101);
}

TEST(UserTest, RemoveLogWithUnknownIdIsNoOp){
    User u("Alice", "secret");
    auto* log = new CaveLog();
    log->setID(101);
    u.addLog(log);

    Log marker;
    marker.setID(999);
    u.removeLog(&marker);

    EXPECT_EQ(u.getLogs().size(), 1u);
}

TEST(UserTest, SetLogsReplacesVector){
    User u;
    std::vector<Log*> replacement;
    replacement.push_back(new HikeLog());
    replacement.push_back(new HikeLog());
    u.setLogs(replacement);
    EXPECT_EQ(u.getLogs().size(), 2u);
}

TEST(UserTest, SetNumUsersAffectsNextGeneratedId){
    User::setNumUsers(500);
    User next("a", "1");
    EXPECT_EQ(next.getId(), 501);
}
