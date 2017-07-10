#include "logger.h"


// This is a hardware test; verification will be physically inspecting sd card
// contents.
void cppsetup() {
  rcr::geovis::Logger logger;
  logger.Open("logger-test-path.txt");
  logger.WriteLine("line one");
  logger.WriteLine("line two");
  logger.Close();
}

void cpploop() {}