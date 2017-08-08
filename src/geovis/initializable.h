#ifndef RCR_GEOVIS_INITIALIZABLE_H
#define RCR_GEOVIS_INITIALIZABLE_H

namespace rcr {
namespace geovis {

// Pure-virtual class.
class Initializable {
 public:
  Initializable(const char* display_name);
  
  virtual const char* display_name() const;

  // Returns true if initialization is successful.
  virtual bool Init() = 0;

  // Returns true if initialization was successful.
  virtual bool IsFullyInitialized() const;

  virtual ~Initializable();

 protected:
  const char* const display_name_;
  bool init_result_ = false;
};

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_INITIALIZABLE_H
