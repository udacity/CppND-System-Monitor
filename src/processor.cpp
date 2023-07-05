#include <string>
#include "../include/processor.h"
#include "../include/linux_parser.h"

// Read from /proc/stat and store in private member data
void Processor::ReadStatus() {
  std::string line;
  std::string key;
  previous_ = current_;

  // Access file, read, and store stat data
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "cpu") {
          linestream
            >> current_.user
            >> current_.nice
            >> current_.system
            >> current_.idle
            >> current_.iowait
            >> current_.irq
            >> current_.softirq
            >> current_.steal
            >> current_.guest
            >> current_.guestnice;
          break;
        }
      }
    }
  }
}

// Return the aggregate CPU utilization
float Processor::Utilization() {
  unsigned long long int prevIdle;
  unsigned long long int idle;
  unsigned long long int prevNonIdle;
  unsigned long long int nonIdle;
  unsigned long long int prevTotal;
  unsigned long long int total;
  unsigned long long int totald;
  unsigned long long int idled;

  // Get latest CPU stats
  ReadStatus();

  // Calculate CPU usage per
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  prevIdle = previous_.idle + previous_.iowait;
  idle = current_.idle + current_.iowait;
  prevNonIdle =
    previous_.user +
    previous_.nice +
    previous_.system +
    previous_.irq +
    previous_.softirq +
    previous_.steal;
  nonIdle = current_.user +
    current_.nice +
    current_.system +
    current_.irq +
    current_.softirq +
    current_.steal;
  prevTotal = prevIdle + prevNonIdle;
  total = idle + nonIdle;
  totald = total - prevTotal;
  idled = idle - prevIdle;

  return (totald - idled) / static_cast<float>(totald);
}
