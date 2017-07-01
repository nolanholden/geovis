namespace rcr {
namespace level1payload {
  
// TODO: setup runtime filename resolution. (likely use millis())
// File path names
constexpr const char* kLogPath = "flight.log";

// CSV file headers
constexpr const char* kBarometricCsvHeader = "*C,Pa,%,m,";
constexpr const char* kImuCsvHeader = "x(heading),y(roll),z(pitch),Lx,Ly,Lg,Gx,Gy,Gz,";
constexpr const char* kGpsCsvHeader = "lat,long,alt";

// Main loop delay (milliseconds)
constexpr const uint32_t kLoopDelay = 1024;

} // namespace level1_payload
} // namespace rcr
