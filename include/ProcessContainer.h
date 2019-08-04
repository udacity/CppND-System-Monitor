#ifndef PROCESS_CONTAINER_H
#define PROCESS_CONTAINER_H

#include <string>
#include <vector>
#include "process.h"
using std::string;
using std::vector;

class ProcessContainer {
 public:
  ProcessContainer() { this->refreshList(); }
  void refreshList();
  string printList();
  vector<string> getList();

 private:
  vector<Process> _list;
};
#endif
