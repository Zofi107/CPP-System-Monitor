#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using namespace std;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  //new
  void Pid(int);
  void User(int);
  void Command(int);
  void CpuUtilization(int);
  void Ram(int);
  void UpTime(int);
  bool operator>(Process const& a) const;
  //
  // TODO: Declare any necessary private members
 private:
  int pid_{0};
  string user_{""};
  string command_{""};
  float cpu_utilization_{0.0};
  string ram_{""};
  long up_time_{0};
};

#endif