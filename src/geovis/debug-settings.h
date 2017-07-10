#ifndef RCR_GEOVIS_DEBUGSETTINGS_H_
#define RCR_GEOVIS_DEBUGSETTINGS_H_

namespace rcr {
namespace geovis {

// Set false to disable all debugging.
#define DEBUG_ANY true

#define DEBUG_LOGGER            true
#define DEBUG_ATMOSPHERICSENSOR true
#define DEBUG_GPSRECEIVER       true
#define DEBUG_IMU               true

#if !DEBUG_ANY // then redefine them all as false.
#undef DEBUG_LOGGER
#undef DEBUG_ATMOSPHERICSENSOR
#undef DEBUG_GPSRECEIVER
#undef DEBUG_IMU
#define DEBUG_LOGGER            false
#define DEBUG_ATMOSPHERICSENSOR false
#define DEBUG_GPSRECEIVER       false
#define DEBUG_IMU               false
#endif // !DEBUG_ANY

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_DEBUGSETTINGS_H_
