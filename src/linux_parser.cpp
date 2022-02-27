#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

/**
 * @brief Get the path for the file
 * Only used on MAC_OS, to access the mock data
 * @param path
 * @return string
 */
string LinuxParser::GetFilePath(string path) {
#if __APPLE__
  return kBasePath + path;
#else
  return path;
#endif
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(GetFilePath(kOSPath));
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }

  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(GetFilePath(kProcDirectory + kVersionFilename));
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(GetFilePath(kProcDirectory).c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

/**
 * @brief Calculate the utilisation of the memory
 *
 * @return float
 */
float LinuxParser::MemoryUtilization() {
  string line;
  float total{1};
  float available{0};

  std::string name;
  float value;

  // Read the file and parse it line by line
  std::ifstream stream(GetFilePath(kProcDirectory + kMeminfoFilename));
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      if (linestream >> name >> value) {
        // Save the values if the keys match either memtotal, or memavailable
        if (name == "MemTotal:") total = value;
        if (name == "MemAvailable:") available = value;

        // We can break, if both of the values are greater than zero;
        // To prevent further, unnecesarry looping
        if (total && available) break;
      } else {
        break;
      }
    }
  }

  return available / total;
}

/**
 * @brief Read and return the uptime of the computer
 *
 * @return long
 */
long LinuxParser::UpTime() {
  string line;
  long uptime{0};

  // Load and parse file
  std::ifstream stream(GetFilePath(kProcDirectory + kUptimeFilename));
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }

  return uptime;
}

/**
 * @brief Read and return the total number of processes
 *
 * @return int
 */
int LinuxParser::TotalProcesses() {
  string line;

  std::ifstream stream(GetFilePath(kProcDirectory + kStatFilename));
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      string name;
      int value{0};
      if (linestream >> name >> value) {
        if (name == "processes") return value;
      }
    }
  }
  return 0;
}

/**
 * @brief Read and return the total number of processes
 *
 * @return int
 */
int LinuxParser::RunningProcesses() {
  string line;

  std::ifstream stream(GetFilePath(kProcDirectory + kStatFilename));
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      string name;
      int value{0};
      if (linestream >> name >> value) {
        if (name == "procs_running") return value;
      }
    }
  }
  return 0;
}