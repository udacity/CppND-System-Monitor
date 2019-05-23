#include <iostream>
#include <fstream>
#include "util.h"

using namespace std;

int main(){
    string t = Util::convertToTime(64000);
    cout << "test convertToTime: "<< t << "\n";
    
    cout << "test progressbar: " << Util::getProgressBar(" 23%") << "\n";
    cout << "test progressbar: " << Util::getProgressBar("100%") << "\n";
    
    cout << "test getstream exists:"<< "\n";
    string filename = "main.cpp";
    ifstream s1;
    Util::getStream(filename, s1);
    string line;
    getline(s1,line);
    cout<< line << endl;
    s1.close();
    
    cout << "test getstream non-existing file:"<< "\n";
    Util::getStream("bullshit", s1);
    
    return 0;
}