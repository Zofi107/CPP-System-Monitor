#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;



// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value = "n/a";
  std::ifstream filestream(kOSPath);
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
  string os;
  string version;
  string kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
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
  DIR* directory = opendir(kProcDirectory.c_str());
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string next, temp, line;
  float mem = 0.0;
  vector<string> memory;
  std::ifstream s(kProcDirectory + kMeminfoFilename);
  if (s.is_open()) {
    int i = 0;
    while(i<2){
      std::getline(s, line);
      std::istringstream ls(line);
      ls >> next >> temp >> next;
      memory.push_back(temp);
      i++;
    }
  }
  float mem_total = std::stof(memory[0]);
  float mem_free = std::stof(memory[1]);
  mem = (mem_total - mem_free) / mem_total;
  return mem;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime = 0.0;
  string curr, line;
  std::ifstream s(kProcDirectory + kUptimeFilename);
  if (s.is_open()) {
    std::getline(s, line);
    std::istringstream ls(line);
    ls >> curr;
  }
  uptime = std::atoi(curr.c_str());
  return uptime; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<string> jiffies = CpuUtilization();
  long jif = 0;
  for(string j : jiffies) {
    jif += std::stoi(j);
  }
  return jif;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  long jif = 0;
  string utime, stime, line, blank;
  std::ifstream s(kProcDirectory + std::to_string(pid)+ kStatFilename);
  if (s.is_open()) {
    std::getline(s, line);
    std::istringstream ls(line); 
    int i = 1;
    while(i<14){
      ls >> blank;
      i++;
    }
    ls >> utime >> stime;
  }
  jif = std::atol(utime.c_str()) + std::atol(stime.c_str());
  return jif;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  return Jiffies() - IdleJiffies();
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> jiffies = CpuUtilization();
  return  (long)std::stoi(jiffies[3]) + (long)std::stoi(jiffies[4]);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> util;
  string timer, line, blank;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line); 
    linestream >> blank;
    int i = 0;
    while(i<10){
      linestream >> timer;
      util.push_back(timer);
      i++;
    }
  }
  return util; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return std::stoi(LinuxParser::Parser("processes", kProcDirectory + kStatFilename));
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return std::stoi(LinuxParser::Parser("procs_running", kProcDirectory + kStatFilename));
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string retval = "";
  std::ifstream s(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (s.is_open()) {
    std::getline(s, retval);
  }
  return retval;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  return LinuxParser::Parser("VmSize:", kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  return LinuxParser::Parser("Uid:", kProcDirectory + "/" + to_string(pid) + kStatusFilename);
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, getuser, getuid, blank;
  string user = "";
  string uid = LinuxParser::Uid(pid);
  std::ifstream s(kPasswordPath);
  if (s.is_open()) {
    while(s.peek() != EOF ) {
      std::getline(s, line);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream ls(line); 
      ls >> getuser >> blank >> getuid;
      if(uid == getuid) {
        user = getuser;
        break;
      }
    }
  }
  return user; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long time = 0;
  string line, blank;
  std::ifstream s(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (s.is_open()) {
    std::getline(s, line);
    std::istringstream ls(line); 
    int i = 1;
    while(i<22){
      ls >> blank;
      i++;
    }
    ls >> time;
  }
  return time / sysconf(_SC_CLK_TCK);
}

string LinuxParser::Parser(string key, string path) {
  string value = "n/a";
  bool search = true;
  string line;
  string temp;
  std::ifstream stream(path);
  if(stream.is_open()) {
    while(search == true && stream.peek() != EOF) {
      std::getline(stream, line);
      std::istringstream linestream(line); 
      linestream >> temp;
      if(temp == key) {
        linestream >> temp;
        value = temp;
        search = false;
      }
    }
  }
  return value; 
}
