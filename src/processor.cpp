#include "processor.h"

#include <iostream>
#include <regex>
#include <sstream>
#include <string>

#include "command.h"

std::string cpu_usage_apple = "top -l 1 | grep -E \"^CPU\"";
std::string cpu_usage_linux = "top -b -n 1 | grep -E \"%Cpu\"";

std::string cpu_usage = __APPLE__ ? cpu_usage_apple : cpu_usage_linux;

/**
 * @brief Get the current usage of the cpu, using the top command
 *
 * @return float
 */
float Processor::Utilization() {
  // Get the response and turn it into string stream
  std::string res = Command::Exec(cpu_usage.c_str());
  std::istringstream f(res);
  std::string line;

  // Create response array
  std::vector<float> values = {};

  if (std::getline(f, line)) {
    // use regex to get all numbers from the response
    std::regex regex("\\d+(.\\d+)?");

    auto words_begin = std::sregex_iterator(line.begin(), line.end(), regex);
    auto words_end = std::sregex_iterator();

    // Map iterator to vector
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
      std::smatch match = *i;
      std::string match_str = match.str();

      values.push_back(std::stof(match_str));
    }
  } else {
    // There is no valid response
    return 0.0;
  }

  // Return zero if there are less than 3 results
  if (values.size() < 3) return 0.0;

  // Return 3rd result for apple devices (user, system, IDLE)
  // Subtrack from one to calculate the usage of the cpu
  if (__APPLE__) return 1 - values[2] / 100;

  // Return 4th result for non-apple devices (user, system, ni, IDLE)
  // Subtrack from one to calculate the usage of the cpu
  return 1 - values[3] / 100;
}