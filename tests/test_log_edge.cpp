#include <gtest/gtest.h>
#include "Log.h"
#include "Participant.h"

// Documents the double-increment quirk in Log::generateID:
//   ++numLogs;            // counter goes up by 1
//   setID(++numObject);   // numObject IS numLogs by reference, so it bumps AGAIN
// Each default-constructed Log advances the static counter by 2, not 1.
// If anyone "fixes" this to single-increment, the test will fail and the fixer
// can decide whether to update the test or revisit the change.
TEST(LogQuirkTest, DefaultConstructorAdvancesNumLogsByTwoNotOne){
    Log::setNumLogs(0);
    Log a;
    EXPECT_EQ(a.getId(), 2);

    Log b;
    EXPECT_EQ(b.getId(), 4);

    Log c;
    EXPECT_EQ(c.getId(), 6);
}

TEST(LogEdgeTest, SortByIdReturnsFalseForEqualIds){
    // Strict-weak-ordering check: a < a must be false, otherwise std::sort UB.
    Log a;
    Log b;
    a.setID(5);
    b.setID(5);
    EXPECT_FALSE(Log::sortByID(&a, &b));
    EXPECT_FALSE(Log::sortByID(&b, &a));
}

TEST(LogEdgeTest, SortByDurationReturnsFalseForEqualDurations){
    Log a;
    Log b;
    a.setDurationMins(60);
    b.setDurationMins(60);
    EXPECT_FALSE(Log::sortByDuration(&a, &b));
    EXPECT_FALSE(Log::sortByDuration(&b, &a));
}

TEST(LogEdgeTest, SetDurationFromZeroHoursAndZeroMinutesIsZero){
    Log l;
    l.setDuration(0, 0);
    EXPECT_EQ(l.getDurationMins(), 0);
}

TEST(LogEdgeTest, SetDurationFromNegativeMinutesIsAccepted){
    // No validation — documents that negative values are stored verbatim.
    Log l;
    l.setDurationMins(-30);
    EXPECT_EQ(l.getDurationMins(), -30);
}

TEST(LogEdgeTest, AddSameParticipantNameTwiceCreatesTwoEntries){
    // No dedupe in addParticipant.
    Log l;
    l.addParticipant(new Participant("Alice", 0, 1));
    l.addParticipant(new Participant("Alice", 0, 2));
    EXPECT_EQ(l.getParticipants().size(), 2u);
}

TEST(LogEdgeTest, RemoveParticipantOnEmptyVectorIsNoOp){
    Log l;
    Participant marker("X", 0, 1);
    EXPECT_NO_THROW(l.removeParticipant(&marker));
    EXPECT_EQ(l.getParticipants().size(), 0u);
}

TEST(LogEdgeTest, RemoveParticipantWithNoMatchingIdIsNoOp){
    Log l;
    l.addParticipant(new Participant("Keep", 0, 100));
    Participant marker("marker", 0, 999);
    l.removeParticipant(&marker);
    EXPECT_EQ(l.getParticipants().size(), 1u);
}

TEST(CaveLogEdgeTest, ConstructorWithEmptyDateAndAreaAcceptsEmptyValues){
    CaveLog c(1, "", "", "", "");
    EXPECT_EQ(c.getName(), "");
    EXPECT_EQ(c.getDate(), "");
    EXPECT_EQ(c.getArea(), "");
    EXPECT_EQ(c.getNote(), "");
}

TEST(HikeLogEdgeTest, NegativeDistanceIsAcceptedNoValidation){
    HikeLog h;
    h.setDist(-100);
    EXPECT_EQ(h.getDist(), -100);
}
