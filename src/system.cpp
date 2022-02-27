#include "system.h"

#include <unistd.h>

#include <array>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "command.h"
#include "format.h"
#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

/**
 * @brief Return the CPU of the system
 *
 * @return Processor&
 */
Processor& System::Cpu() { return _cpu; }

/**
 * @brief Return all the processes in the system
 *
 * @return vector<Process>&
 */
vector<Process>& System::Processes() { return processes_; }

/**
 * @brief Return the kernel of the system
 *
 * @return std::string
 */
std::string System::Kernel() { return LinuxParser::Kernel(); }

/**
 * @brief Returns the current memory utilisation
 * @return float
 */
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

/**
 * @brief Returns the operating system
 *
 * @return std::string
 */
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

/**
 * @brief Returns the number of active processes in the system
 *
 * @return int
 */
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

/**
 * @brief Return the total number of processes in the system
 *
 * @return int
 */
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

/**
 * @brief Returning the uptime of the system
 *
 * @return long int
 */
long int System::UpTime() { return LinuxParser::UpTime(); }

/**
 * @brief Used to update the local process list
 */
void System::Refresh() { loadProcesses(); }

/**
 * @brief Loads all the processes in the system
 * Using the "ps" command, because of platform missmatch
 */
void System::loadProcesses() {
  // Execute command and turn string to stringstream for later use
  std::string res = Command::Exec("ps -e -o pid,args,pcpu,rss,user,time");
  std::istringstream f(res);
  std::string line;

  // Get first line of the response
  // Removing the header from the input data
  std::getline(f, line);

  // Create new process list
  std::vector<Process> processes;

  // Create and add process to the process list
  while (std::getline(f, line)) {
    Process process(line);
    processes.push_back(process);
  }

  // Sort the process list by memory usage
  std::sort(processes.begin(), processes.end());

  // Update the local process list
  processes_ = processes;
}

/**
 * @brief Construct a new System:: System object
 * Loads all the processes
 */
System::System() {
  Processor processor;
  _cpu = processor;

  loadProcesses();
}
