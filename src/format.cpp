#include <string>

#include "format.h"
#include<iostream>
using std::string;
using std::to_string;
using namespace std;
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    long sec, min, hr;
    sec = seconds % 60;
    min = (seconds / 3600) % 60;
    hr = seconds / 3600;
    string HH = std::to_string(hr);
    string MM = std::to_string(min);
    string SS = std::to_string(sec);
    
    // HH might be over 100 hours
    if( HH.size() == 1){
        string HH_mod;
        HH_mod.append("0");
        HH_mod.push_back(HH.back());
        HH = HH_mod;
    }

    MM = string(2 - MM.length(), '0') + MM;
    SS = string(2 - SS.length(), '0') + SS;
    return HH + ":" + MM + ":" + SS; 
}