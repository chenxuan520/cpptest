#include "../test.h"

class Example : public cpptest::Test {
public:
  void SetUp() override { arr.push_back(1); }
  std::vector<int> arr;
};

INIT(Begin) {
  DEBUG("begin");
  return 0;
}

END(End) { DEBUG("end"); }

ARGC_FUNC {
  if (argc == 2) {
    REGEX_FILT_TEST(argv[1]);
  }
}

TEST(Base, True) { MUST_EQUAL(0, 0); }

TEST_F(Example, ArrSize) {
  MUST_EQUAL(arr.size(), 1);
  arr.resize(0);
  MUST_EQUAL(arr.size(), 0);
}

TEST_F(Example, ArrValue) { MUST_EQUAL(arr[0], 1); }

TEST_DEFAULT {
  MUST_TRUE(true, "");
  MUST_TRUE(true != false, "");
}
