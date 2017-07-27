#ifndef RCR_GEOVIS_UPDATEABLE_H
#define RCR_GEOVIS_UPDATEABLE_H

namespace rcr {
namespace geovis {

class Updateable {
 public:
  // Do this every time in loop();
  virtual void Update() = 0;

  virtual ~Updateable();
};

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_UPDATEABLE_H
