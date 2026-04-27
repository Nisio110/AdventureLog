#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>
#include "Disk.h"
#include "User.h"
#include "Log.h"
#include "Participant.h"

namespace {

class DiskIOTest : public ::testing::Test {
protected:
    std::filesystem::path tempPath;

    void SetUp() override {
        tempPath = std::filesystem::path(::testing::TempDir())
                 / ("adventurelog_test_" + std::to_string(::testing::UnitTest::GetInstance()->random_seed()) + ".yaml");
        std::error_code ec;
        std::filesystem::remove(tempPath, ec);
    }

    void TearDown() override {
        std::error_code ec;
        std::filesystem::remove(tempPath, ec);
    }

    static bool vecContains(const std::vector<std::string>& v, const std::string& needle) {
        for (const auto& s : v) {
            if (s == needle) return true;
        }
        return false;
    }
};

}  // anonymous namespace

// Serialization (object → string-vector) ---------------------------------

TEST_F(DiskIOTest, UserToStrEmitsExpectedHeaderAndFields){
    Disk d{tempPath.string()};
    User u("Alice", "secret");
    u.setID(42);

    auto lines = d.userToStr(&u);
    EXPECT_TRUE(vecContains(lines, "object: User"));
    EXPECT_TRUE(vecContains(lines, "id: 42"));
    EXPECT_TRUE(vecContains(lines, "name: Alice"));
    EXPECT_TRUE(vecContains(lines, "password: secret"));
    EXPECT_TRUE(vecContains(lines, "---"));
}

TEST_F(DiskIOTest, CaveLogToStrEmitsCaveSpecificFields){
    Disk d{tempPath.string()};
    CaveLog c;
    c.setID(7);
    c.setUserId(1);
    c.setDate("01/01/2026");
    c.setArea("Clare");
    c.setNote("note");
    c.setDurationMins(120);
    c.setName("Faunarooska");
    c.setSRTCave(true);
    c.setCaveLeader(false);
    c.setRigger(true);

    auto lines = d.logToStr(&c);
    EXPECT_TRUE(vecContains(lines, "object: CaveLog"));
    EXPECT_TRUE(vecContains(lines, "cave-name: Faunarooska"));
    EXPECT_TRUE(vecContains(lines, "srt-cave: true"));
    EXPECT_TRUE(vecContains(lines, "cave-leader: false"));
    EXPECT_TRUE(vecContains(lines, "rigging: true"));
    EXPECT_TRUE(vecContains(lines, "duration-mins: 120"));
}

TEST_F(DiskIOTest, HikeLogToStrEmitsHikeSpecificFields){
    Disk d{tempPath.string()};
    HikeLog h;
    h.setID(9);
    h.setUserId(1);
    h.setDate("22/02/2026");
    h.setArea("Mournes");
    h.setNote("ok");
    h.setDurationMins(190);
    h.setDist(16);
    h.setWeather("cloudy");

    auto lines = d.logToStr(&h);
    EXPECT_TRUE(vecContains(lines, "object: HikeLog"));
    EXPECT_TRUE(vecContains(lines, "distance: 16"));
    EXPECT_TRUE(vecContains(lines, "weather: cloudy"));
}

TEST_F(DiskIOTest, ParticipantToStrEmitsAllFields){
    Disk d{tempPath.string()};
    Participant p("Adam", 5, 12);

    auto lines = d.partToStr(&p);
    EXPECT_TRUE(vecContains(lines, "object: Participant"));
    EXPECT_TRUE(vecContains(lines, "name: Adam"));
    EXPECT_TRUE(vecContains(lines, "id: 12"));
    EXPECT_TRUE(vecContains(lines, "log-id: 5"));
}

// Round-trip (object graph → file → fresh load) --------------------------

TEST_F(DiskIOTest, RoundtripPreservesUserAndCaveLog){
    {
        Disk writer{tempPath.string()};
        User u("Alice", "pw");
        u.setID(1);

        auto* log = new CaveLog();
        log->setID(10);
        log->setUserId(1);
        log->setDate("01/01/2026");
        log->setArea("Clare");
        log->setNote("first cave");
        log->setDurationMins(180);
        log->setName("Faunarooska");
        log->setSRTCave(true);
        log->setCaveLeader(false);
        log->setRigger(true);
        u.addLog(log);

        std::vector<std::string> buf;
        auto uv = writer.userToStr(&u);
        buf.insert(buf.end(), uv.begin(), uv.end());
        auto lv = writer.logToStr(log);
        buf.insert(buf.end(), lv.begin(), lv.end());
        writer.writeToDisk(buf);
    }

    Disk reader{tempPath.string()};
    reader.loadFromDisk(tempPath.string());

    ASSERT_EQ(reader.getUsers().size(), 1u);
    auto* loadedUser = reader.getUsers().at(0);
    EXPECT_EQ(loadedUser->getId(), 1);
    EXPECT_EQ(loadedUser->getName(), "Alice");
    EXPECT_EQ(loadedUser->getPasswd(), "pw");

    ASSERT_EQ(loadedUser->getLogs().size(), 1u);
    auto* loadedCave = dynamic_cast<CaveLog*>(loadedUser->getLogs().at(0));
    ASSERT_NE(loadedCave, nullptr);
    EXPECT_EQ(loadedCave->getName(), "Faunarooska");
    EXPECT_EQ(loadedCave->getDate(), "01/01/2026");
    EXPECT_EQ(loadedCave->getArea(), "Clare");
    EXPECT_EQ(loadedCave->getDurationMins(), 180);
    EXPECT_TRUE(loadedCave->isSRTCave());
    EXPECT_FALSE(loadedCave->wasCaveLeader());
    EXPECT_TRUE(loadedCave->wasRigger());
}

TEST_F(DiskIOTest, RoundtripPreservesHikeLog){
    {
        Disk writer{tempPath.string()};
        User u("Bob", "x");
        u.setID(2);

        auto* h = new HikeLog();
        h->setID(20);
        h->setUserId(2);
        h->setDate("22/02/2026");
        h->setArea("Mournes");
        h->setNote("good");
        h->setDurationMins(190);
        h->setDist(16);
        h->setWeather("cloudy");
        u.addLog(h);

        std::vector<std::string> buf;
        auto uv = writer.userToStr(&u);
        buf.insert(buf.end(), uv.begin(), uv.end());
        auto lv = writer.logToStr(h);
        buf.insert(buf.end(), lv.begin(), lv.end());
        writer.writeToDisk(buf);
    }

    Disk reader{tempPath.string()};
    reader.loadFromDisk(tempPath.string());

    ASSERT_EQ(reader.getUsers().size(), 1u);
    auto* loadedHike = dynamic_cast<HikeLog*>(reader.getUsers().at(0)->getLogs().at(0));
    ASSERT_NE(loadedHike, nullptr);
    EXPECT_EQ(loadedHike->getDist(), 16);
    EXPECT_EQ(loadedHike->getWeather(), "cloudy");
    EXPECT_EQ(loadedHike->getArea(), "Mournes");
    EXPECT_EQ(loadedHike->getDurationMins(), 190);
}

TEST_F(DiskIOTest, RoundtripWiresParticipantsToTheirLog){
    {
        Disk writer{tempPath.string()};
        User u("Carol", "x");
        u.setID(3);

        auto* log = new CaveLog();
        log->setID(30);
        log->setUserId(3);
        log->setDate("01/01/2026");
        log->setName("Mastadon");
        log->setSRTCave(false);
        log->setCaveLeader(false);
        log->setRigger(false);

        auto* p1 = new Participant("Adam", 30, 100);
        auto* p2 = new Participant("Sean", 30, 101);
        log->addParticipant(p1);
        log->addParticipant(p2);
        u.addLog(log);

        std::vector<std::string> buf;
        auto uv = writer.userToStr(&u);
        buf.insert(buf.end(), uv.begin(), uv.end());
        auto lv = writer.logToStr(log);
        buf.insert(buf.end(), lv.begin(), lv.end());
        for (auto* p : log->getParticipants()) {
            auto pv = writer.partToStr(p);
            buf.insert(buf.end(), pv.begin(), pv.end());
        }
        writer.writeToDisk(buf);
    }

    Disk reader{tempPath.string()};
    reader.loadFromDisk(tempPath.string());

    ASSERT_EQ(reader.getUsers().size(), 1u);
    auto* loadedLog = reader.getUsers().at(0)->getLogs().at(0);
    ASSERT_EQ(loadedLog->getParticipants().size(), 2u);

    auto names = std::vector<std::string>{};
    for (auto* p : loadedLog->getParticipants()) {
        names.push_back(p->getName());
        EXPECT_EQ(p->getLogId(), 30);
    }
    EXPECT_NE(std::find(names.begin(), names.end(), "Adam"), names.end());
    EXPECT_NE(std::find(names.begin(), names.end(), "Sean"), names.end());
}

// Error path -------------------------------------------------------------

TEST_F(DiskIOTest, LoadingNonexistentFileThrowsDiskAccessError){
    auto missing = std::filesystem::path(::testing::TempDir()) / "definitely_does_not_exist.yaml";
    std::error_code ec;
    std::filesystem::remove(missing, ec);

    Disk d{missing.string()};
    EXPECT_THROW(d.loadFromDisk(missing.string()), DiskAccessError);
}

// ID-counter restoration on load -----------------------------------------

TEST_F(DiskIOTest, LoadAdvancesStaticIdCountersToMaxSeen){
    {
        Disk writer{tempPath.string()};
        User u("Dave", "x");
        u.setID(50);

        std::vector<std::string> buf;
        auto uv = writer.userToStr(&u);
        buf.insert(buf.end(), uv.begin(), uv.end());
        writer.writeToDisk(buf);
    }

    User::setNumUsers(0);
    Disk reader{tempPath.string()};
    reader.loadFromDisk(tempPath.string());

    User next("e", "y");
    EXPECT_EQ(next.getId(), 51);
}
