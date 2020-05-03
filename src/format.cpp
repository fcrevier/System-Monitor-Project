#include <string>
#include "format.h"

using std::string;
using std::to_string;
const long H2S = 3600;
const long M2S = 60;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  long hours = seconds/H2S;
  long mins = (seconds - hours*H2S)/M2S;
  long secs = seconds - hours*H2S - mins*M2S;
  
  string HH = (hours > 9 ? "" : "0") + to_string(hours);
  string MM = (mins  > 9 ? "" : "0") + to_string(mins);
  string SS = (secs  > 9 ? "" : "0") + to_string(secs);
  return HH + ":" + MM + ":" + SS;
}