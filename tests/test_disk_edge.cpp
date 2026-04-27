#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include "Disk.h"
#include "User.h"
#include "Log.h"
#include "Participant.h"

namespace {

class DiskFormatTest : public ::testing::Test {
protected:
    std::filesystem::path tempPath;

    void SetUp() override {
        tempPath = std::filesystem::path(::testing::TempDir())
                 / ("adventurelog_format_" + std::to_string(::testing::UnitTest::GetInstance()->random_seed()) + ".yaml");
        std::error_code ec;
        std::filesystem::remove(tempPath, ec);
    }

    void TearDown() override {
        std::error_code ec;
        std::filesystem::remove(tempPath, ec);
    }

    void writeFile(const std::string& content){
        std::ofstream f(tempPath);
        f << content;
    }
};

}  // namespace

// Parser quirks ----------------------------------------------------------

TEST_F(DiskFormatTest, ValueContainingColonSpacePreservedAfterFirstDelimiter){
    // strToKVPair splits on the FIRST `": "` it finds, then the value is
    // substr-to-end (no further splitting). So a name like "Mr: Bond" is
    // preserved verbatim: key="name", value="Mr: Bond".
    writeFile(
        "object: User\n"
        "id: 1\n"
        "name: Mr: Bond\n"
        "password: pw\n"
        "---\n"
    );
    Disk d{tempPath.string()};
    d.loadFromDisk(tempPath.string());
    ASSERT_EQ(d.getUsers().size(), 1u);
    EXPECT_EQ(d.getUsers().at(0)->getName(), "Mr: Bond");
}

TEST_F(DiskFormatTest, EmptyPasswordValueRoundTripsAsEmptyString){
    // The user's actual disk.yaml has User id=3 "Wow" and id=4 "YAY" with empty
    // passwords. Confirm the loader accepts that.
    writeFile(
        "object: User\n"
        "id: 1\n"
        "name: Wow\n"
        "password: \n"
        "---\n"
    );
    Disk d{tempPath.string()};
    d.loadFromDisk(tempPath.string());
    ASSERT_EQ(d.getUsers().size(), 1u);
    EXPECT_EQ(d.getUsers().at(0)->getPasswd(), "");
}

TEST_F(DiskFormatTest, UnknownKeysAfterRequiredFieldsAreSilentlyIgnored){
    writeFile(
        "object: User\n"
        "id: 1\n"
        "name: Alice\n"
        "password: pw\n"
        "favourite-colour: blue\n"
        "completely-unknown: garbage\n"
        "---\n"
    );
    Disk d{tempPath.string()};
    EXPECT_NO_THROW(d.loadFromDisk(tempPath.string()));
    ASSERT_EQ(d.getUsers().size(), 1u);
    EXPECT_EQ(d.getUsers().at(0)->getName(), "Alice");
}

TEST_F(DiskFormatTest, ReorderingUserFieldsBreaksPositionalParse){
    // initUser uses positional access (attrs[1]=id, attrs[2]=name, attrs[3]=passwd).
    // Swap name and password — they get loaded into the wrong slots.
    writeFile(
        "object: User\n"
        "id: 1\n"
        "password: pw\n"
        "name: Alice\n"
        "---\n"
    );
    Disk d{tempPath.string()};
    d.loadFromDisk(tempPath.string());
    ASSERT_EQ(d.getUsers().size(), 1u);
    // Documents broken-but-current behaviour: "pw" lands in name, "Alice" in passwd.
    EXPECT_EQ(d.getUsers().at(0)->getName(), "pw");
    EXPECT_EQ(d.getUsers().at(0)->getPasswd(), "Alice");
}

TEST_F(DiskFormatTest, ReorderingLogFieldsIsToleratedBecauseInitLogSearchesByKey){
    // initLog uses key-search, not positional access. Reordering should be fine.
    writeFile(
        "object: User\n"
        "id: 1\n"
        "name: Alice\n"
        "password: pw\n"
        "---\n"
        "object: CaveLog\n"
        "owner-id: 1\n"            // moved up
        "cave-name: TestCave\n"    // moved up
        "id: 1\n"
        "date: 01/01/2026\n"
        "area: Test\n"
        "note: ok\n"
        "duration-mins: 60\n"
        "rigging: false\n"
        "cave-leader: false\n"
        "srt-cave: false\n"
        "---\n"
    );
    Disk d{tempPath.string()};
    d.loadFromDisk(tempPath.string());
    auto* log = dynamic_cast<CaveLog*>(d.getUsers().at(0)->getLogs().at(0));
    ASSERT_NE(log, nullptr);
    EXPECT_EQ(log->getName(), "TestCave");
    EXPECT_EQ(log->getDate(), "01/01/2026");
}

TEST_F(DiskFormatTest, MissingTrailingDividerStillCapturesLastObject){
    writeFile(
        "object: User\n"
        "id: 1\n"
        "name: Alice\n"
        "password: pw\n"
        // no trailing ---
    );
    Disk d{tempPath.string()};
    EXPECT_NO_THROW(d.loadFromDisk(tempPath.string()));
    ASSERT_EQ(d.getUsers().size(), 1u);
    EXPECT_EQ(d.getUsers().at(0)->getName(), "Alice");
}

TEST_F(DiskFormatTest, EmptyFileThrowsRuntimeError){
    writeFile("");
    Disk d{tempPath.string()};
    EXPECT_THROW(d.loadFromDisk(tempPath.string()), std::runtime_error);
}

TEST_F(DiskFormatTest, EmptyIdValueThrowsBecauseStoulFailsOnEmpty){
    writeFile(
        "object: User\n"
        "id: \n"
        "name: Alice\n"
        "password: pw\n"
        "---\n"
    );
    Disk d{tempPath.string()};
    // strToNum -> stoul("") throws std::invalid_argument
    EXPECT_THROW(d.loadFromDisk(tempPath.string()), std::exception);
}

TEST_F(DiskFormatTest, DuplicateUserIdsAreAcceptedAndBothLoaded){
    writeFile(
        "object: User\n"
        "id: 1\n"
        "name: Alice\n"
        "password: pw\n"
        "---\n"
        "object: User\n"
        "id: 1\n"
        "name: Bob\n"
        "password: pw\n"
        "---\n"
    );
    Disk d{tempPath.string()};
    d.loadFromDisk(tempPath.string());
    EXPECT_EQ(d.getUsers().size(), 2u);
    // No dedupe — system silently allows colliding IDs.
}

// HikeLog distance suffix bug --------------------------------------------

TEST_F(DiskFormatTest, HikeLogDistanceWithKmSuffixSilentlyTruncates){
    // disk-template.yaml writes `distance: 16km` but strToNum -> stoul stops at
    // the first non-digit and silently drops "km".
    writeFile(
        "object: User\n"
        "id: 1\n"
        "name: Alice\n"
        "password: pw\n"
        "---\n"
        "object: HikeLog\n"
        "id: 1\n"
        "duration-mins: 100\n"
        "area: Test\n"
        "note: ok\n"
        "date: 01/01/2026\n"
        "owner-id: 1\n"
        "distance: 16km\n"
        "weather: sunny\n"
        "---\n"
    );
    Disk d{tempPath.string()};
    d.loadFromDisk(tempPath.string());
    auto* log = dynamic_cast<HikeLog*>(d.getUsers().at(0)->getLogs().at(0));
    ASSERT_NE(log, nullptr);
    EXPECT_EQ(log->getDist(), 16);  // suffix lost
}

TEST_F(DiskFormatTest, LiteralStringNullInNoteIsPreservedAsString){
    // The user's actual disk.yaml has `note: null` for a HikeLog. The system
    // stores the literal string "null", it is not converted to an empty/absent
    // value. Document this so it doesn't surprise anyone later.
    writeFile(
        "object: User\n"
        "id: 1\n"
        "name: Alice\n"
        "password: pw\n"
        "---\n"
        "object: HikeLog\n"
        "id: 1\n"
        "duration-mins: 60\n"
        "area: Test\n"
        "note: null\n"
        "date: 01/01/2026\n"
        "owner-id: 1\n"
        "distance: 5\n"
        "weather: sunny\n"
        "---\n"
    );
    Disk d{tempPath.string()};
    d.loadFromDisk(tempPath.string());
    auto* log = d.getUsers().at(0)->getLogs().at(0);
    EXPECT_EQ(log->getNote(), "null");
}

// Orphans ----------------------------------------------------------------

TEST_F(DiskFormatTest, OrphanLogIsLoadedButNotAttachedToAnyUser){
    // owner-id: 999 doesn't match any user. Per CONTEXT.md / PROGRESS.md, the
    // log stays in Disk's internal vector but is NOT attached to a user, then
    // gets dropped on the next save.
    writeFile(
        "object: User\n"
        "id: 1\n"
        "name: Alice\n"
        "password: pw\n"
        "---\n"
        "object: CaveLog\n"
        "id: 100\n"
        "duration-mins: 60\n"
        "area: Nowhere\n"
        "note: orphan\n"
        "date: 01/01/2026\n"
        "owner-id: 999\n"
        "cave-name: GhostCave\n"
        "rigging: false\n"
        "cave-leader: false\n"
        "srt-cave: false\n"
        "---\n"
    );
    Disk d{tempPath.string()};
    d.loadFromDisk(tempPath.string());
    ASSERT_EQ(d.getUsers().size(), 1u);
    EXPECT_EQ(d.getLogs().size(), 1u);                     // present in global list
    EXPECT_EQ(d.getUsers().at(0)->getLogs().size(), 0u);   // not attached
}

TEST_F(DiskFormatTest, OrphanParticipantIsLoadedButNotAttachedToAnyLog){
    writeFile(
        "object: User\n"
        "id: 1\n"
        "name: Alice\n"
        "password: pw\n"
        "---\n"
        "object: CaveLog\n"
        "id: 1\n"
        "duration-mins: 60\n"
        "area: Test\n"
        "note: ok\n"
        "date: 01/01/2026\n"
        "owner-id: 1\n"
        "cave-name: TestCave\n"
        "rigging: false\n"
        "cave-leader: false\n"
        "srt-cave: false\n"
        "---\n"
        "object: Participant\n"
        "id: 1\n"
        "log-id: 999\n"
        "name: GhostCaver\n"
        "---\n"
    );
    Disk d{tempPath.string()};
    d.loadFromDisk(tempPath.string());
    auto* log = d.getUsers().at(0)->getLogs().at(0);
    EXPECT_EQ(log->getParticipants().size(), 0u);   // not attached
    EXPECT_EQ(d.getParticipants().size(), 1u);      // but in global list
}
