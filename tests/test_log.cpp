#include <gtest/gtest.h>
#include "Log.h"
#include "Participant.h"

TEST(LogTest, DefaultConstructorAdvancesCounter){
    Log a;
    Log b;
    EXPECT_NE(a.getId(), b.getId());
}

TEST(LogTest, UidDateConstructorSkipsIdGeneration){
    Log l(7, "01/02/2026");
    EXPECT_EQ(l.getUserId(), 7);
    EXPECT_EQ(l.getDate(), "01/02/2026");
}

TEST(LogTest, UidDateNoteConstructorAssignsAllThree){
    Log l(7, "01/02/2026", "rainy");
    EXPECT_EQ(l.getUserId(), 7);
    EXPECT_EQ(l.getDate(), "01/02/2026");
    EXPECT_EQ(l.getNote(), "rainy");
}

TEST(LogTest, UidDateAreaNoteConstructorAssignsAllFour){
    Log l(7, "01/02/2026", "Mournes", "windy");
    EXPECT_EQ(l.getUserId(), 7);
    EXPECT_EQ(l.getDate(), "01/02/2026");
    EXPECT_EQ(l.getArea(), "Mournes");
    EXPECT_EQ(l.getNote(), "windy");
}

TEST(LogTest, SettersOverrideAllScalarFields){
    Log l;
    l.setDurationMins(120);
    l.setArea("Wicklow");
    l.setNote("test note");
    l.setDate("31/12/2025");
    l.setUserId(11);
    l.setID(555);
    EXPECT_EQ(l.getDurationMins(), 120);
    EXPECT_EQ(l.getArea(), "Wicklow");
    EXPECT_EQ(l.getNote(), "test note");
    EXPECT_EQ(l.getDate(), "31/12/2025");
    EXPECT_EQ(l.getUserId(), 11);
    EXPECT_EQ(l.getId(), 555);
}

TEST(LogTest, SetDurationFromHoursMinutesConvertsToMinutes){
    Log l;
    l.setDuration(2, 30);
    EXPECT_EQ(l.getDurationMins(), 150);
}

TEST(LogTest, AddParticipantAppends){
    Log l;
    l.addParticipant(new Participant("Alice"));
    l.addParticipant(new Participant("Bob"));
    EXPECT_EQ(l.getParticipants().size(), 2u);
}

TEST(LogTest, RemoveParticipantMatchesByIdAndDropsIt){
    Log l;
    auto* keep = new Participant("Keep", 0, 100);
    auto* drop = new Participant("Drop", 0, 200);
    l.addParticipant(keep);
    l.addParticipant(drop);

    Participant marker("marker", 0, 200);
    l.removeParticipant(&marker);

    ASSERT_EQ(l.getParticipants().size(), 1u);
    EXPECT_EQ(l.getParticipants().at(0)->getId(), 100);
}

TEST(LogTest, SetParticipantsReplacesVector){
    Log l;
    std::vector<Participant*> ps;
    ps.push_back(new Participant("a"));
    ps.push_back(new Participant("b"));
    ps.push_back(new Participant("c"));
    l.setParticipants(ps);
    EXPECT_EQ(l.getParticipants().size(), 3u);
}

TEST(LogTest, SortByIdComparesAscending){
    Log a, b;
    a.setID(1);
    b.setID(2);
    EXPECT_TRUE(Log::sortByID(&a, &b));
    EXPECT_FALSE(Log::sortByID(&b, &a));
}

TEST(LogTest, SortByDurationComparesAscending){
    Log a, b;
    a.setDurationMins(30);
    b.setDurationMins(120);
    EXPECT_TRUE(Log::sortByDuration(&a, &b));
    EXPECT_FALSE(Log::sortByDuration(&b, &a));
}

TEST(LogTest, SetNumLogsAffectsNextGeneratedId){
    Log::setNumLogs(0);
    Log first;
    Log::setNumLogs(0);
    Log reset;
    EXPECT_GT(first.getId(), 0);
    EXPECT_GT(reset.getId(), 0);
}

// CaveLog ----------------------------------------------------------------

TEST(CaveLogTest, DefaultConstructionLeavesAllFlagsFalseAndNameDefault){
    CaveLog c;
    EXPECT_EQ(c.getName(), "N/A");
    EXPECT_FALSE(c.isSRTCave());
    EXPECT_FALSE(c.wasCaveLeader());
    EXPECT_FALSE(c.wasRigger());
}

TEST(CaveLogTest, ThreeArgConstructorSetsName){
    CaveLog c(1, "Faunarooska", "01/01/2026");
    EXPECT_EQ(c.getName(), "Faunarooska");
    EXPECT_EQ(c.getDate(), "01/01/2026");
    EXPECT_EQ(c.getUserId(), 1);
}

TEST(CaveLogTest, FiveArgConstructorSetsAreaAndNote){
    CaveLog c(1, "Mastadon", "05/04/2026", "Fermanagh", "muddy");
    EXPECT_EQ(c.getName(), "Mastadon");
    EXPECT_EQ(c.getArea(), "Fermanagh");
    EXPECT_EQ(c.getNote(), "muddy");
}

TEST(CaveLogTest, FullConstructorWiresAllFlagsAndParticipants){
    std::vector<Participant*> ps {new Participant("X"), new Participant("Y")};
    CaveLog c(1, "Bruce's Pot", "24/11/2025", "Fermanagh", "long", ps, true, true, false);
    EXPECT_EQ(c.getName(), "Bruce's Pot");
    EXPECT_TRUE(c.isSRTCave());
    EXPECT_TRUE(c.wasCaveLeader());
    EXPECT_FALSE(c.wasRigger());
    EXPECT_EQ(c.getParticipants().size(), 2u);
}

TEST(CaveLogTest, SettersToggleEachFlag){
    CaveLog c;
    c.setName("Carrickcoppan");
    c.setSRTCave(true);
    c.setCaveLeader(true);
    c.setRigger(true);
    EXPECT_EQ(c.getName(), "Carrickcoppan");
    EXPECT_TRUE(c.isSRTCave());
    EXPECT_TRUE(c.wasCaveLeader());
    EXPECT_TRUE(c.wasRigger());
}

TEST(CaveLogTest, TypeConstantIsCave){
    EXPECT_EQ(CaveLog::type, "Cave");
}

// HikeLog ----------------------------------------------------------------

TEST(HikeLogTest, DefaultConstructionLeavesDistanceMinusOneAndDefaultWeather){
    HikeLog h;
    EXPECT_EQ(h.getDist(), -1);
    EXPECT_EQ(h.getWeather(), "N/A");
}

TEST(HikeLogTest, UidDateConstructor){
    HikeLog h(3, "22/02/2026");
    EXPECT_EQ(h.getUserId(), 3);
    EXPECT_EQ(h.getDate(), "22/02/2026");
}

TEST(HikeLogTest, UidDateAreaNoteDistConstructor){
    HikeLog h(3, "22/02/2026", "Mournes", "wet", 16);
    EXPECT_EQ(h.getArea(), "Mournes");
    EXPECT_EQ(h.getNote(), "wet");
    EXPECT_EQ(h.getDist(), 16);
}

TEST(HikeLogTest, FullConstructorSetsWeatherAndParticipants){
    std::vector<Participant*> ps {new Participant("X")};
    HikeLog h(3, "22/02/2026", "Mournes", "fine", 12, "sunny", ps);
    EXPECT_EQ(h.getDist(), 12);
    EXPECT_EQ(h.getWeather(), "sunny");
    EXPECT_EQ(h.getParticipants().size(), 1u);
}

TEST(HikeLogTest, SettersOverrideDistAndWeather){
    HikeLog h;
    h.setDist(7);
    h.setWeather("foggy");
    EXPECT_EQ(h.getDist(), 7);
    EXPECT_EQ(h.getWeather(), "foggy");
}

TEST(HikeLogTest, TypeConstantIsHike){
    EXPECT_EQ(HikeLog::type, "Hike");
}
