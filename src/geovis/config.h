#ifndef GEOVIS_CONFIG_H_
#define GEOVIS_CONFIG_H_

#include <array>
#include <cstddef>
#include <chrono>
#include <functional>
#include <ratio>
#include <type_traits>
#include <utility>
#include <unordered_map>
#include <vector>

using nanoseconds = std::chrono::duration<long long, std::nano>;
using microseconds = std::chrono::duration<long long, std::micro>;
using milliseconds = std::chrono::duration<long long, std::milli>;
using seconds = std::chrono::duration<long long>;
using minutes = std::chrono::duration<int, std::ratio<60>>;
using hours = std::chrono::duration<int, std::ratio<3600>>;
using days = std::chrono::duration<int, std::ratio<86400>>;

namespace geovis {

namespace detail {

template <typename T_return_type>
struct dummy_func {
  constexpr auto operator()() const {
    return T_return_type{};
  }
};

template <> struct dummy_func<void> {
  constexpr auto operator()() const {}
};

// Permit config to have no init functions; use this in place. See usage below.
template <typename T_func_return_type = void>
struct empty {
  struct itr {
    using deref_return_type = dummy_func<T_func_return_type>;
    constexpr auto operator *() const { return deref_return_type{}; }
    constexpr auto operator !=(const itr& other) const { return false; }
    constexpr itr& operator ++() { return *this; }
  };
  constexpr auto begin() const { return itr{}; }
  constexpr auto end() const { return itr{}; }
};

} // namespace detail

enum class failure_severity_t {
  trivial,
  minor,
  moderate,
  critical,
  total_failure,
};

enum class initialization_id_t {
  poweron_status_lights,
  enable_antenna,
  enable_radio,
};

template <initialization_id_t id>
struct initialization_info {
  static constexpr const char* description = "default description.";
  static constexpr failure_severity_t failure_severity = failure_severity_t::total_failure;
};

template <>
struct initialization_info<initialization_id_t::poweron_status_lights> {
  static constexpr const char* description = "power on status lights";
  static constexpr failure_severity_t failure_severity = failure_severity_t::trivial;
};

auto go() {
  auto t = std::make_tuple(4, 3., std::string{ " yo " });
  auto f = std::get<2>(t);
}

template <typename T_init_functions,
  typename T_failing_init_functions>
struct sys_config {
  const T_init_functions unconditional_inits;
  const T_failing_init_functions fallable_inits; // return false on failure

  // assert fallable_inits return bool.
  static_assert(std::is_same<bool, decltype( (*fallable_inits.begin()) () )>::value, "fallable_inits must be collection returning bool");

  template <typename T, typename U> constexpr
  sys_config(T&& inits, U&& _fallable_inits)
    : unconditional_inits(std::forward<T>(inits))
    , fallable_inits(std::forward<U>(_fallable_inits))
  {}
};

template <typename T_init_functions, typename T_failing_init_functions>
constexpr auto 
make_config(T_init_functions&& inits, T_failing_init_functions&& fallable_inits) {
  return sys_config<T_init_functions, T_failing_init_functions>{
      std::forward<T_init_functions>(inits),
      std::forward<T_failing_init_functions>(fallable_inits)
    };
}

//constexpr auto
//  make_config_with_fallable_inits() {
//  return make_config(detail::empty<void>{}, detail::empty<bool>{});
//}
//
//template <typename T_init_functions>
//constexpr auto
//make_config(T_init_functions&& inits, T_failing_init_functions&& fallable_inits) {
//  return sys_config<T_init_functions, T_failing_init_functions>{
//    std::forward<T_init_functions>(inits),
//      std::forward<T_failing_init_functions>(fallable_inits)
//  };






} // namespace geovis

#endif // GEOVIS_CONFIG_H_
