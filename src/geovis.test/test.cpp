#include "pch.h"
#include "../geovis/config.h"

#include <functional>
#include <vector>

auto init_called_flags() {
  std::vector<bool> flags{};
  flags.reserve(5);
  return flags;
}
auto current_init_called_flags = init_called_flags();
auto reset_init_flags() {
  for (auto& flag : current_init_called_flags)
    flag = false;
}
void init0() { current_init_called_flags[0] = true; }
void init1() { current_init_called_flags[1] = true; }
void init2() { current_init_called_flags[2] = true; }
void init3() { current_init_called_flags[3] = true; }
bool init4() { current_init_called_flags[4] = true; return false; }

template <typename T>
constexpr auto 
none_false(const T& collection) {
  for (const auto& item : collection)
    if (item == false) return false;

  return true;
}

TEST(test, test1) {

}

//TEST(GeovisConfig, CanHaveNoInits) {
//  auto config = geovis::make_config();
//
//  for (const auto& init : config.unconditional_inits)
//    init();
//  for (const auto& failing_init : config.fallable_inits)
//    failing_init();
//}
//
//TEST(GeovisConfig, CanHaveNoUnconditionalInits) {
//  bool(*const fallable_inits[])() = { init4 };
//  auto config = geovis::make_config()
//    .with_fallable_inits(fallable_inits);
//
//  current_init_called_flags[4] = false;
//  for (const auto& init : config.unconditional_inits)
//    init();
//  for (const auto& failing_init : config.fallable_inits)
//    failing_init();
//
//  ASSERT_TRUE(current_init_called_flags[4]);
//}
//
//TEST(GeovisConfig, CanHaveNoFallableInits) {
//  void(*const inits[])() = { init0 };
//  auto config = geovis::make_config()
//    .with_inits(inits);
//
//  current_init_called_flags[0] = false;
//  for (const auto& init : config.unconditional_inits)
//    init();
//  for (const auto& failing_init : config.fallable_inits)
//    failing_init();
//
//  ASSERT_TRUE(current_init_called_flags[0]);
//}
//
//TEST(GeovisConfig, CallsAllInits) {
//  auto inits = { init0, init1, init2, init3 };
//  bool(* const fallable_inits[])() = { init4 };
//  auto config = geovis::make_config()
//    .with_inits(inits)
//    .with_fallable_inits(fallable_inits);
//
//  for (const auto& init : config.unconditional_inits)
//    init();
//  for (const auto& failing_init : config.fallable_inits)
//    failing_init();
//
//  ASSERT_TRUE(none_false(current_init_called_flags));
//}
