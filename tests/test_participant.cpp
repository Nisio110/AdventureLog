#include <gtest/gtest.h>
#include "Participant.h"

TEST(ParticipantTest, NameOnlyConstructorSetsNameAndAutoId){
    Participant p("Bulma");
    EXPECT_EQ(p.getName(), "Bulma");
    EXPECT_GT(p.getId(), 0);
}

TEST(ParticipantTest, NameAndLogIdConstructor){
    Participant p("Vegeta", 42);
    EXPECT_EQ(p.getName(), "Vegeta");
    EXPECT_EQ(p.getLogId(), 42);
    EXPECT_GT(p.getId(), 0);
}

TEST(ParticipantTest, FullConstructorPreservesExplicitId){
    Participant p("Krillin", 7, 99);
    EXPECT_EQ(p.getName(), "Krillin");
    EXPECT_EQ(p.getLogId(), 7);
    EXPECT_EQ(p.getId(), 99);
}

TEST(ParticipantTest, SettersOverrideValues){
    Participant p("Goku");
    p.setName("Piccolo");
    p.setID(123);
    p.setLogID(456);
    EXPECT_EQ(p.getName(), "Piccolo");
    EXPECT_EQ(p.getId(), 123);
    EXPECT_EQ(p.getLogId(), 456);
}

TEST(ParticipantTest, EachAutoIdConstructionAdvancesCounter){
    Participant a("a");
    Participant b("b");
    EXPECT_GT(b.getId(), a.getId());
}

TEST(ParticipantTest, SetNumParticipantsResetsAutoCounterBaseline){
    Participant::setNumParticipants(0);
    Participant first("first");
    EXPECT_EQ(first.getId(), 1);

    Participant second("second");
    EXPECT_EQ(second.getId(), 2);
    EXPECT_EQ(first.getNumParticipants(), 2);
}
