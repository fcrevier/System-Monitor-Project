#include "ncurses_display.h"
#include "system.h"

int main() {
  // Set buffer size for cpu utilization
  int bufferLength = 100;
  System system(bufferLength);

  //System system; // default buffer
  NCursesDisplay::Display(system);
}