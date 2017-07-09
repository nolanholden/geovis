#include "../geovis/sensor.h"
#include "../geovis/constants.h"

#include "googletest/googletest/include/gtest/gtest.h"

namespace rcr {
namespace geovis {

class DerivedSensor : public Sensor {
 public:

};

// To use a test fixture, derive a class from testing::Test.
class SensorTest : public Sensor, public ::testing::Test {
 protected:  // You should make the members protected s.t. they can be
             // accessed from sub-classes.

  // must implement to make non-abstract
  bool Init() { return true; }

  kalman_t KalmanInitTest(double value) {
    return kalmanInit(value);
  }

  kalman_t UpdateKalmanTest(kalman_t* state, double value) {
    this->kalmanUpdate(state, value);
  }

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
    k1_.process_noise = KALMAN_PROCESS_NOISE;
    k1_.measurement_noise = KALMAN_MEASUREMENT_NOISE;
    k1_.value;
    k1_.error = KALMAN_ERROR;
    k1_.gain; 
  }

  // virtual void TearDown() will be called after each test is run.
  // You should define it if there is cleanup work to do.  Otherwise,
  // you don't have to provide it.
  //
  // virtual void TearDown() {
  // }

  void MapTester() {
    ASSERT_EQ(q->Size(), new_q->Size());
  }

  kalman_t k1_;
  kalman_t k2_;
  kalman_t k3_;
  kalman_t k4_;
};

// When you have a test fixture, you define a test using TEST_F
// instead of TEST.

// Tests the default c'tor.
TEST_F(QueueTest, DefaultConstructor) {
  // You can access data in the test fixture here.
  EXPECT_EQ(0u, q0_.Size());
}

// Tests Dequeue().
TEST_F(QueueTest, Dequeue) {
  int * n = q0_.Dequeue();
  EXPECT_TRUE(n == NULL);

  n = q1_.Dequeue();
  ASSERT_TRUE(n != NULL);
  EXPECT_EQ(1, *n);
  EXPECT_EQ(0u, q1_.Size());
  delete n;

  n = q2_.Dequeue();
  ASSERT_TRUE(n != NULL);
  EXPECT_EQ(2, *n);
  EXPECT_EQ(1u, q2_.Size());
  delete n;
}

// Tests the Queue::Map() function.
TEST_F(QueueTest, Map) {
  MapTester(&q0_);
  MapTester(&q1_);
  MapTester(&q2_);
}

} // namespace geovis
} // namespace rcr
