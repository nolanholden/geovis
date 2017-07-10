#ifndef RCR_GEOVIS_INITIALIZABLE_H
#define RCR_GEOVIS_INITIALIZABLE_H

namespace rcr {
namespace geovis {

// Requires method bool Init().
class Initializable {
 public:
  // Returns true if initialization is successful.
  virtual bool Init() = 0;

  virtual bool IsFullyInitialized();

  virtual ~Initializable();
 protected:
  bool init_result_ = false;
};

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_INITIALIZABLE_H
