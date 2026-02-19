#include "../include/Tests.h"
#include <string>
using std::string;
using std::cout;
using std::cin;

string Tests::ret = " returns ";

bool Tests::verifyTest(){
    bool response;
    cout << "Was the test successful? [Y/n] ";
    cin >> response;
    return response;
}
string Tests::div(int size) {
    string d;
    for (int i{}; i < size; ++i){ d.append("==="); }
    return d;
}
