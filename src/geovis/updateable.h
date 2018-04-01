#ifndef RCR_GEOVIS_UPDATEABLE_H
#define RCR_GEOVIS_UPDATEABLE_H

namespace rcr {
namespace geovis {

// Use this interface when class instances must be constantly updated.
// This is a workaround for the lack of (reliable) support of threading
// on embedded architectures.
class Updateable {
 public:
  // Do this every time in loop();
  virtual void Update() = 0;

  virtual ~Updateable() {}
};

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_UPDATEABLE_H
