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

// Return the aggregate CPU utilization per:
// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization() {
  unsigned long long int usertime;
  unsigned long long int nicetime;
  // unsigned long long int idlealltime;
  // unsigned long long int systemalltime;
  // unsigned long long int virtalltime;
  // unsigned long long int totaltime;
  unsigned long long int PrevIdle;
  unsigned long long int Idle;
  unsigned long long int PrevNonIdle;
  unsigned long long int NonIdle;
  unsigned long long int PrevTotal;
  unsigned long long int Total;
  unsigned long long int totald;
  unsigned long long int idled;

  // Get latest CPU stats
  ReadStatus();

  // Guest time is already accounted in usertime, same with nice
  usertime = current_.user - current_.guest;
  nicetime = current_.nice - current_.guestnice;

  // Calculate cpu percentage
  // idlealltime = current_.idle + current_.iowait;
  // systemalltime = current_.system + current_.irq + current_.softirq;
  // virtalltime = current_.guest + current_.guestnice;
  // totaltime =
  //   usertime +
  //   nicetime +
  //   systemalltime +
  //   idlealltime +
  //   current_.steal +
  //   virtalltime;

  PrevIdle = previous_.idle + previous_.iowait;
  Idle = current_.idle + current_.iowait;
  PrevNonIdle = previous_.user + previous_.nice + previous_.system + previous_.irq + previous_.softirq + previous_.steal;
  NonIdle = current_.user + current_.nice + current_.system + current_.irq + current_.softirq + current_.steal;
  PrevTotal = PrevIdle + PrevNonIdle;
  Total = Idle + NonIdle;

  // differentiate: actual value minus the previous one
  totald = Total - PrevTotal;
  idled = Idle - PrevIdle;

  return (totald - idled) / static_cast<float>(totald);

}