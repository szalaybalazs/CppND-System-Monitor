#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(std::string processline);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  std::string UpTime();
  bool operator<(Process const& a) const;

 private:
  int _pid = 12;
  std::string _user{""};
  std::string _cmd{""};
  float _ram{0};
  float _cpu{0};
  std::string _time{""};
  std::string _uptime{""};
};

#endif