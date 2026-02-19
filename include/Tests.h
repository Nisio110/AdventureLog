#ifndef _TESTS_
#define _TESTS_
#include "User.h"

// User Class Tests
namespace UserTest {
    void testGetSetID();
    void testGetSetName();
    void testGetSetPasswd();
    void testGetSetCaveLogs();
    void testGetSetHikeLogs();
    void testCaveLogsIO(); // add and remove CaveLog objects
    void testHikeLogsIO(); // add and remove HikeLog objects
    void testGenerateID();
    void testConstructors();
}
// Log Class Tests
namespace LogTest{
    void testGetSetDurationMins();
    void testGetSetArea();
    void testGetSetParticipants();
    void testGetSetNote();
    void testGetSetDate();
    void testGetSetID();
    void testGetSetPID();
    void testParticipantsIO(); // add and remove participants
    void testDisplay();
}

#endif