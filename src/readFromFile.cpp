#include  <stdexcept> // runtime error
#include <string>
#include <iostream>
#include <fstream>

std::string readFromFile(std::string path, std::string flag){
    
    // initialize output
    std::string output{"not found"};

    // string to hold each streamed line
    std::string line;

    // Read from the text file
    std::ifstream MyReadFile(path);

    // read file line by line as long as no error
    while (getline (MyReadFile, line)) {
        // Output the text from the file
        std::size_t found_pos = line.find(flag);
        // if matches were found
        if (found_pos!=std::string::npos){
            
            std::size_t pos = found_pos+flag.length();
            
            // output until line end if no length defined
            // std::size_t len = line.length()-1; 
            // std::string output = line.substr (pos,len);
            output = line.substr (pos);
            break;
            // std::cout << "found: " << output << "\n";  
        } 
    }; 

    // Close the file
    MyReadFile.close();

    return output;
};

