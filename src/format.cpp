#include <string>

#include "format.h"

using namespace std;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long ss) {
  long int mm = ss / 60;
  long int hh = mm / 60;
  string hours;
  string minutes;
  string seconds;

  if (hh < 10) {
    hours = "0" + to_string(hh);
  } else {
    hours = to_string(hh);
  }
  
  if ((mm % 60) < 10) {
    minutes = "0" + to_string(mm % 60);
  } else {
    minutes = to_string(mm % 60);
  }
  
  if ((ss % 60) < 10) {
    seconds = "0" + to_string(ss % 60);
  } else {
    seconds = to_string(ss % 60);
  }

  return hours + ":" + minutes + ":" + seconds;
}