#ifndef RCR_GEOVIS_INITIALIZABLE_H
#define RCR_GEOVIS_INITIALIZABLE_H

namespace rcr {
namespace geovis {

// Pure-virtual class.
class Initializable {
 public:
  // Provide an (immutable) display name.
  Initializable(const char* display_name);
  
  const char* display_name() const;

  // Returns true if initialization is successful.
  bool Init();

  // Returns true if initialization was successful.
  bool IsFullyInitialized() const;

  virtual ~Initializable();

 protected:
  // Initialize the inherited class. (used in public method 'Init()')
  virtual bool ProtectedInit() = 0;

 private:
  const char* const display_name_;

  bool init_result_ = false;
};

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_INITIALIZABLE_H
