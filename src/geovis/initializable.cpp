#include "initializable.h"

namespace rcr {
namespace geovis {

const char* Initializable::display_name() const { return display_name_; }

bool Initializable::IsFullyInitialized() const { return init_result_; }

Initializable::~Initializable() {}

} // namespace geovis
} // namespace rcr
