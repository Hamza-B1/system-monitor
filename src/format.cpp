#include <string>
#include <sstream>
#include <iomanip>
#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
  float hours = seconds / 60;
  seconds %= 3600;
  float minutes = seconds / 60;
  seconds %= 60;
  string time {};
  std::stringstream ss(time);
  ss << std::setfill('0') << std::setw(2) << std::to_string(hours) << ':';
  ss << std::setfill('0') << std::setw(2) << std::to_string(minutes) << ':';
  ss << std::setfill('0') << std::setw(2) << std::to_string(minutes) << ':';
  return ss.str();
}