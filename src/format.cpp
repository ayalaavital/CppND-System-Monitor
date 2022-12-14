#include <string>
#include <chrono>

#include "format.h"

using std::string;
using std::to_string;
using namespace std::chrono;


string Format::ElapsedTime(long seconds) {
  std::chrono::seconds secs = std::chrono::seconds(seconds);
  std::string formatted_secs;
  auto h = duration_cast<hours>(secs);
  secs -= h;
  auto m = duration_cast<minutes>(secs);
  secs -= m;
  if (h < 10h)
    formatted_secs.push_back('0');
  formatted_secs += to_string(h/1h);
  formatted_secs += ':';
  if (m < 10min)
    formatted_secs.push_back('0');
  formatted_secs += to_string(m/1min);
  formatted_secs += ':';
  if (secs < 10s)
    formatted_secs.push_back('0');
  formatted_secs += to_string(secs/1s);
  return formatted_secs;
}