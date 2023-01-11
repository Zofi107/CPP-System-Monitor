#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include<iostream>

#include "process.h"
#include "linux_parser.h"

using namespace std;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_utilization_; }

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return up_time_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return this->cpu_utilization_ < a.cpu_utilization_;
}

bool Process::operator>(Process const& a) const { 
  return this->cpu_utilization_ > a.cpu_utilization_;
}

//setters

void Process::Pid(int pid) {
  pid_ = pid;
}

void Process::User(int pid) {
  user_ = LinuxParser::User(pid);
}

void Process::Command(int pid) {
  command_ = LinuxParser::Command(pid);
}

void Process::CpuUtilization(int pid) {
  cpu_utilization_ = (float)LinuxParser::ActiveJiffies(pid) / LinuxParser::ActiveJiffies();
}

void Process::Ram(int pid) {
  int MB = stoi(LinuxParser::Ram(pid))/1000;
  ram_ = to_string(MB);
}

void Process::UpTime(int pid){
  up_time_ = LinuxParser::UpTime(pid);
}