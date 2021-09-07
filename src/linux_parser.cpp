#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream> // debugging
#include <filesystem>
#include <regex>

#include "linux_parser.h"

using std::filesystem::directory_iterator;
using std::filesystem::is_directory;
using std::stof;
using std::string;
using std::to_string;
using std::vector;


string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_'); 
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' '); // replace characters so that we end up with a file of key value pairs separated by space                                               
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


vector<int> LinuxParser::Pids() {
  vector<int> pids;
  for (const auto &entry: directory_iterator(kProcDirectory)) {
    string filename = entry.path().filename();
    if (entry.is_directory() && std::all_of(filename.begin(), filename.end(), isdigit)) {
      int pid = stoi(filename);
      pids.push_back(pid);
    }
  }
  return pids;
}



float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  long MemTotal {};
  long MemFree {};
  std::ifstream fs(kProcDirectory + kMeminfoFilename);
  if (fs.is_open()) {
    while (std::getline(fs, line)) {
      std::istringstream ls(line);
      while (ls >> key >> value) {
        if (key == "MemTotal:") {
          MemTotal = std::stoi(value);
        }
        else if (key == "MemFree:") {
          MemFree = std::stoi(value);
        }
      }
    }
  }
  return MemTotal - MemFree;
}


long LinuxParser::UpTime() { 
  std::ifstream fs(kProcDirectory + kUptimeFilename);
  string line;
  string value;
  long UpTime {};
  if (fs.is_open()) {
    while (std::getline(fs, line)) {
    std::istringstream ls(line);
    ls >> value;
    }
    UpTime = std::stoi(value);
  }
  return UpTime;
}


long LinuxParser::Jiffies() { // total jiffies are given by sys ticks per second multiplied by sys uptime
  return UpTime() * sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  std::ifstream fs{};
  return 0;
}

  long LinuxParser::ActiveJiffies() {
  std::ifstream fs{kProcDirectory + kStatFilename};
  vector<string> cpuStats {}; //format: {user, nice, system, idel, iowait, irq, softirq}
  string key {};
  string value {};
  string line {};
  while (std::getline(fs, line)) 
  
  return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization() { return 0.0; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line {};
  string key {};
  string value {};
  std::ifstream fs{kProcDirectory + kStatFilename};
  int numOfProcs {};
    if (fs.is_open()) {
    while (std::getline(fs, line)) {
      std::istringstream ls(line);
      while (ls >> key >> value) {
        if (key == "processes") {   
          numOfProcs = std::stoi(value);
        }
      }
    }
  }
  return numOfProcs;
}


int LinuxParser::RunningProcesses() {
  string line {};
  string key {};
  string value {};
  std::ifstream fs{kProcDirectory + kStatFilename};
  int runningProcs {100000000};
    if (fs.is_open()) {
    while (std::getline(fs, line)) {
      std::istringstream ls(line);
      while (ls >> key >> value) {
        if (key == "procs_running") {
          runningProcs = std::stoi(value);
        }
      }
    }
  }
  return runningProcs;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::ifstream fs{kProcDirectory + to_string(pid) + kStatusFilename};
  string line {};
  string key {};
  string value {};
  string uid {};
  if (fs.is_open()) {
    while (std::getline(fs, line)) {
      std::istringstream ls(line);
      while (ls >> key >> value) {
        if (key == "Uid:") {
          uid = value;
        }
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string user {};
  // std::cout << "UID from User func: " << uid << std::endl;
  string line {};
  string key {};
  string value {};
  std::ifstream fs {kPasswordPath};
  while (std::getline(fs, line)) {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::replace(line.begin(), line.end(), 'x', ' ');
    std::istringstream ls(line);
    // std::cout << "line: " << line << std::endl;
    ls >> key >> value;
    // std::cout << "key from User: " << key << " | " << "value from User: " << value << "\n"; 
    if (value == uid) {
    user = key;
    }
  }
  return user;
}

// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }

