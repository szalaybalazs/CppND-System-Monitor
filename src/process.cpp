#include "process.h"

#include <unistd.h>

#include <array>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return _pid; }

float Process::CpuUtilization() { return _cpu; }

string Process::Command() { return _cmd; }

/**
 * @brief Get the current ram usage of the process
 *
 * @return string
 */
string Process::Ram() {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(2) << _ram;
  std::string fixed = ss.str();

  return fixed;
}

string Process::User() { return _user; }

std::string Process::UpTime() { return _time; }

/**
 * @brief Sort by ram usage
 *
 * @param a
 * @return true
 * @return false
 */
bool Process::operator<(Process const& a) const { return a._ram < _ram; }

/**
 * @brief Construct a new Process:: Process object
 * Parse the input process line, produced by the ps command
 * Save local variables
 * @param processline
 */
Process::Process(std::string processline) {
  std::istringstream lstream(processline);
  lstream >> _pid >> _cmd >> _cpu >> _ram >> _user >> _time;

  // Convert
  _ram = (_ram / (1024));
  _cpu = _cpu / 100;
}