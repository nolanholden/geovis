#include "initializable.h"

namespace rcr {
namespace geovis {

Initializable::Initializable(const char* const display_name)
    : display_name_(display_name) {}

const char* Initializable::display_name() const { return display_name_; }

bool Initializable::IsFullyInitialized() const { return init_result_; }

Initializable::~Initializable() {}

} // namespace geovis
} // namespace rcr
