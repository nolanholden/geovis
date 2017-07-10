#ifndef RCR_GEOVIS_INITIALIZABLE_H
#define RCR_GEOVIS_INITIALIZABLE_H

namespace rcr {
namespace geovis {

class Initializable {
 public:
  virtual const char* display_name() const;

  // Returns true if initialization is successful.
  virtual bool Init() = 0;

  virtual bool IsFullyInitialized() const;

  virtual ~Initializable();
 protected:
  const char* display_name_;
  bool init_result_ = false;
};

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_INITIALIZABLE_H
