#ifndef _TESTS_
#define _TESTS_
#include <string>
#include <iostream>

namespace Tests{
    using std::string;
    bool verifyTest();
    string ret { " returns " };
    string div() { return "======================== \n"; }
    string div(int x);
}
#endif