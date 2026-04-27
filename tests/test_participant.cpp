#include <gtest/gtest.h>
#include "Participant.h"

TEST(ParticipantTest, NameOnlyConstructor){
    Participant p("Bulma");
    EXPECT_EQ(p.getName(), "Bulma");
}

TEST(ParticipantTest, NameAndLogIdConstructor){
    Participant p("Vegeta", 42);
    EXPECT_EQ(p.getName(), "Vegeta");
    EXPECT_EQ(p.getLogId(), 42);
}

TEST(ParticipantTest, FullConstructorPreservesId){
    Participant p("Krillin", 7, 99);
    EXPECT_EQ(p.getName(), "Krillin");
    EXPECT_EQ(p.getLogId(), 7);
    EXPECT_EQ(p.getId(), 99);
}
