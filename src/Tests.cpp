#include "../include/Tests.h"
#include <string>
#include <cmath>
#include <numbers>
using std::string;
using std::cout;
using std::cin;

string Tests::ret = " : ";

bool Tests::verifyTest(){
    string response;
    bool result;
    cout << "Was the test successful? [Y/n] ";
    std::getline(cin, response);
    switch (response.front()) {
        case  'Y': case 'y':
        return true;
        break;
        default :
        return false;
    }
}

string Tests::div(){ 
    return "======================== \n"; 
} 
string Tests::div(int x) { // there is no reason to do it this way i just think its fun
    using namespace std::numbers;
    if(x > 5) return "THE DIV IS TOO STRONG!!! ABORT! ABORT! ABORT! ABORT!";
    else {
        string d;
        int size = std::round(std::pow(e, x)); 
        for (int i{}; i < size; ++i) d.append("=");
        return d;
    }
}
