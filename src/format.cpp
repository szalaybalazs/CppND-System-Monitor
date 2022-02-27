#include "format.h"

#include <cmath>
#include <string>

using std::string;

// Adds zeros before the number
// Makes the total length of the string to l
// (eg: 1 -> 01, 12 -> 12, 0 -> 00)
std::string padStart(int i, int l) {
  std::string str = std::to_string(i);
  while (str.length() < l) str = "0" + str;

  return str;
}

// Format input seconds to HH:MM:SS
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int secs = (int)(seconds % 60);
  int minutes = (int)((seconds / 60) % 60);
  int hours = (int)(seconds / 60 / 60);

  string secs_str = padStart(hours, 2);
  string minutes_str = padStart(minutes, 2);
  string hours_str = padStart(secs, 2);

  return secs_str + ":" + minutes_str + ":" + hours_str;  // string();
}