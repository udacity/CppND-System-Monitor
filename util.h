#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// Classic helper functions
class Util {
    public:
        static string convertToTime ( long int input_seconds );
        static string getProgressBar(string percent);
        static ifstream getStream(string path);
        static string getToken(string steamPath, string header, uint16_t tokenIdx, char splitChar);
};

string Util::convertToTime (long int input_seconds)
{
    long minutes = input_seconds / 60;
    long hours = minutes / 60;
    long seconds = int(input_seconds%60);
    minutes = int(minutes%60);
    string result = to_string(hours) + ":" + to_string(minutes) + ":" + to_string(seconds);
    return result;
}

// constructing string for given percentage
// 50 bars is uniformly streched 0 - 100 %
// meaning: every 2% is one bar(|)
string Util::getProgressBar(string percent)
{
    string result = "0% ";
    int _size= 50;
    int  boundaries = (stof(percent)/100)*_size;

    for (int i=0;i<_size;i++) {
        if (i<=boundaries) {
            result +="|";
        }
        else {
            result +=" ";
        }
    }

    result +=" " + percent.substr(0,5) + " /100%";
    return result;
}

// wrapper for creating streams
ifstream Util::getStream(string path)
{
    ifstream stream(path);
    if  (!stream) {
        throw runtime_error("Non - existing PID");
    }
    return stream;
}

//Extract specific token from line of a stream starting with header
// if header=="", then extract the nth token from the file
string Util::getToken(string filePath, string header, uint16_t tokenIdx, char splitChar)
{
    bool hasHeader = (header.size() != 0);
    ifstream inputStream = Util::getStream(filePath);

    //read stream line by line
    uint64_t currTokenIdx = 0;
    for (string line; getline(inputStream, line); ) 
    {
        //reset token count only if header is specified
        if (hasHeader)
            currTokenIdx = 0;

        //parse line to see if the header is the right one
        istringstream lineStream(line);
        //disable header search if no header wsa provided...
        bool headerFound = !hasHeader;
        bool isfirstToken = true;
        while(!lineStream.eof()){
            //get a line from the stream
            string token;
            getline(lineStream, token, splitChar);

            //Detect the right line if header is provided of skip to next
            if (hasHeader && isfirstToken)
            {
                if (token == header)
                    headerFound = true;
                else
                    continue;                
            }
            isfirstToken = false;

            //Move to next token if index not reached
            if (headerFound && currTokenIdx == tokenIdx)
                return token;
            currTokenIdx++;
        }
    }

    //Token was not found...
    cout << "getToken() error in " << filePath << ", can't find token index=" << tokenIdx 
         << " from header |" << header << "|" << endl;
    throw out_of_range("Token not found");
}
