#include "ncurses_display.h"
#include "system.h"
#include "stdlib.h"
#include "linux_parser.h" // debugging
#include <iostream> // debugging
using std::cout;

int main() {
  System system;
  NCursesDisplay::Display(system);
  cout << std::fixed << "MemUtil Test:"<< LinuxParser::MemoryUtilization() << '\n';
  cout << std::fixed << "Uptime Test:" << LinuxParser::UpTime() << "\n";
  auto pids = LinuxParser::Pids();
  cout << "PIDs" << "\n";
  for (auto i: pids) {cout << std::fixed << i << ", ";}
  cout << "Number of Processes:" << LinuxParser::TotalProcesses() << std::endl;
  cout << "Number of Running Processes:" << LinuxParser::RunningProcesses() << std::endl;
  cout << "Testing PID UID parser for 20 - Expected Result: 1000 | Actual Result: " << LinuxParser::Uid(20) << std::endl;
  cout << "Testing PID Username parser for 20 - Expected Result: abdul | Actual Result: " << LinuxParser::User(20) << std::endl;
}
