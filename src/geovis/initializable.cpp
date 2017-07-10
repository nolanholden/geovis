#include "initializable.h"

namespace rcr {
namespace geovis {

bool Initializable::IsFullyInitialized() { return init_result_; }

Initializable::~Initializable() {}

} // namespace geovis
} // namespace rcr
