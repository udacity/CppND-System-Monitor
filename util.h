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

        //Extract specific token index "tokenIdx" from file
        static string getToken(string filePath, uint16_t tokenIdx, char splitChar);

        //Extract specific token at index "tokenIdx" from line "header" at position "headerIdx"
        // if header=="", then extract the nth token from the file
        static string getToken(string filePath, string header, uint16_t headerIdx, uint16_t tokenIdx, char splitChar);

        //build empty string of specific size
        static string emptyString(int size);        

        //Trim string to fixed size of add spaces if too small
        static string fixedHeadSize(string input, int maxSize);        
        static string fixedTailSize(string input, int maxSize);        
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
    int _size= 15;
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

//Extract specific token index "tokenIdx" from file
string Util::getToken(string filePath, uint16_t tokenIdx, char splitChar)
{
    return Util::getToken(filePath, "", 0, tokenIdx, splitChar);
}

//Extract specific token at index "tokenIdx" from line "header" at position "headerIdx"
// if header=="", then extract the nth token from the file
string Util::getToken(string filePath, string header, uint16_t headerIdx, uint16_t tokenIdx, char splitChar)
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
        bool tokenFound = false;
        string token;
        while(!lineStream.eof()){
            //get a line from the stream
            string currToken;
            getline(lineStream, currToken, splitChar);

            //jump over the empty tokens (ex multiple spaces in a row)
            if (currToken.size() == 0)
                continue;

            //Detect the right line if header is provided of skip to next
            if (hasHeader && currTokenIdx == headerIdx)
            {
                if (currToken == header)
                    headerFound = true;
                else
                    continue;                
            }

            //Save token if index reached
            if (currTokenIdx == tokenIdx)
            {
                token = currToken;
                tokenFound = true;
            }

            //Both header & token found, return value
            if (tokenFound && headerFound)
                return token;

            //go to next token
            currTokenIdx++;
        }
    }

    //Token was not found...
    string errorMsg = "getToken() error in " + filePath + ", can't find token index=" + to_string(tokenIdx) 
         + " from header |" + header + "|";
    throw out_of_range(errorMsg.c_str());
}

//build empty string of specific size
string Util::emptyString(int size)
{
    string res(size, ' ');
    return res;
}       

//Trim string to fixed size of add spaces if too small
string Util::fixedHeadSize(string input, int maxSize)
{
    if (input.size() < maxSize)
    {
        //add head spaces
        return Util::emptyString(maxSize - input.size()) + input;
    }
    else
    {
        //trim len
        return input.substr(0, maxSize);
    }
}

//Trim string to fixed size of add spaces if too small
string Util::fixedTailSize(string input, int maxSize)
{
    if (input.size() < maxSize)
    {
        //add Tail spaces
        return input + Util::emptyString(maxSize - input.size());
    }
    else
    {
        //trim len
        return input.substr(0, maxSize);
    }
}