#ifndef FOOLGO_TEST_TEST_H_
#define FOOLGO_TEST_TEST_H_

#include <gtest/gtest.h>

namespace foolgo {

class Test: public ::testing::Test {
 public:
  virtual void SetUp() {
  }
};

}  // namespace foolgo


#endif
