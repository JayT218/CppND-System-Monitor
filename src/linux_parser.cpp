#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
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
  string os, kernel, version;
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
  string line, str;
  float total, free, res;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> str >> res;
      if (str == "MemTotal:") {
        total = res;
      } else if (str == "MemFree:") {
        free = res;
        break;
      }
    }
  }
  return (total - free) / total;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long secs;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    stream >> secs;
  }
  return secs;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
/*vector<string> LinuxParser::CpuUtilization() { 
  vector<string> vec;
  long uptime = LinuxParser::UpTime();
  long uptime_pid;
  long cpuUsage;
  vector<int> pids = LinuxParser::Pids();
  for (int pid: pids) {
    string line, str, res;
    long sum = 0;
    long secs = 0;
    uptime_pid = LinuxParser::UpTime(pid);
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    if (stream.is_open()) {
      for (int i = 1; i < 18; i++) {
        stream >> res;
        if (i == 14 || i == 15 || i == 16 || i == 17) {
          sum += std::stol(res);
        }
      }
      secs = uptime - uptime_pid;
      cpuUsage = 100 * ((uptime_pid / sysconf(_SC_CLK_TCK)) / secs);
      vec.push_back(to_string(cpuUsage));
    }
  }
  return vec;
}*/
float LinuxParser::CpuUtilization(int pid) {
  float uptime = LinuxParser::UpTime();
  float uptime_pid =  LinuxParser::UpTime(pid);
  string value;
  float sum = 0;
  float secs = 0;
  float cpuUsage = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    for (int i = 1; i < 23; i++) {
      stream >> value;
      if (i == 14 || i == 15 || i == 16 || i == 17) {
        sum += std::stof(value);
      }
    }
    secs = uptime - uptime_pid;
    cpuUsage = (sum / sysconf(_SC_CLK_TCK)) / secs;
  }
  return cpuUsage;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, str;
  int res;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> str >> res;
      if (str == "processes") {
        return res;
      }
    }
  }
  return res;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, str;
  int res;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> str >> res;
      if (str == "procs_running") {
        return res;
      }
    }
  }
  return res;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line, res;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> res;
      return res;
    }
  }
  return res;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, str;
  int res;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> str >> res;
      if (str == "VmSize:") {
        res = res / 1024;
        return to_string(res);
      }
    }
  }
  return to_string(res);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, str;
  int res;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> str >> res;
      if (str == "Uid:") {
        return to_string(res);
      }
    }
  }
  return to_string(res);
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line, user, x, id;
  string target;
  target = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> id;
      if (id == target) {
        return user;
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, str, res;
  long ans = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    for (int i = 1; i < 23; i++) {
      stream >> res;
    }
    ans = std::stol(res);
    ans = ans / sysconf(_SC_CLK_TCK);
    return ans;
  }
  return ans;
}