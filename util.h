#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

// Classic helper function
class Util {

public:

    static std::string convertToTime ( long int input_seconds );
    static std::string getProgressBar(std::string percent);
    static void getStream(std::string path, std::ifstream& stream);
    template <typename T>
    static T getItemFromStream(std::ifstream &s, std::string field, char sep=':');             //(mine) done
    static std::string getStringFromStream(std::ifstream &s, std::string field, char sep=':'); //(mine) done
    template <typename T>
    static std::vector<T> getSpacedList(std::istringstream &line);                                  //(mine) done
};

std::string Util::convertToTime (long int input_seconds){
    long minutes = input_seconds / 60;
    long hours = minutes / 60;
    long seconds = int(input_seconds%60);
    minutes = int(minutes%60);
    std::stringstream result;
    result << std::setfill('0') <<std::setw(2) << hours << ":" 
           << std::setw(2) << minutes <<  ":" 
           << std::setw(2) << seconds;
    return result.str();
}
// constructing string for given percentage
// 50 bars is uniformly streched 0 - 100 %
// meaning: every 2% is one bar(|)
std::string Util::getProgressBar(std::string percent){

    std::string result = "0% ";
    int _size= 50;
    int  boundaries;
    try {
        boundaries = (stof(percent)/100)*_size;
    } catch (...){
    boundaries = 0;
    }

    for(int i=0;i<_size;i++){
        if(i<=boundaries){
        result +="|";
        }
        else{
        result +=" ";
        }
    }

    result +=" " + percent.substr(0,5) + "/100%";
    return result;
}

// wrapper for creating streams
void Util::getStream(std::string path, std::ifstream& stream){
    stream.open (path, std::ifstream::in);
    if (!stream && !stream.is_open()){
        stream.close();
        throw std::runtime_error("Non - existing PID");
    }
    //return stream;
}

template <typename T>
T Util::getItemFromStream(std::ifstream &s, std::string field, char sep){
    std::string line, key, value;
    while(getline(s, line)){
        std::istringstream ss(line);
        getline(ss, key, sep); 
        if (key == field){
            T value;
            ss >> value;             
            
            return value;
        }
    }    
   
}

// the function below could probably be avoided but I could not get the branching for std::string
// to use getline instead of cin to work with the above template function.
std::string Util::getStringFromStream(std::ifstream &s, std::string field, char sep){
    std::string line, key, value;
    while(getline(s, line)){
        std::istringstream ss(line);
        getline(ss, key, sep);
        if (key == field){
            std::string value;
            getline(ss, value);             
            return value;
        }
    }
    
}

template <typename T>
std::vector<T> Util::getSpacedList(std::istringstream &line){
    std::vector<T> v;
    T val;
    while (line >> val){
        v.push_back(val);
    }
    return v;
}