#include "../test.h"

class Example : public cpptest::Test {
public:
  void SetUp() override { arr_.push_back(1); }
  std::vector<int> arr_;
};

INIT(Begin) { DEBUG("begin"); }

END(End) { DEBUG("end"); }

ARGC_FUNC {
  if (argc == 2) {
    REGEX_FILT_TEST(argv[1]);
  }
}

TEST(Base, True) {
  MUST_EQUAL(0, 0);
  EXPECT_EQ(1, 1);
}

TEST_F(Example, ArrSize) {
  MUST_EQUAL(arr_.size(), 1);
  arr_.resize(0);
  MUST_EQUAL(arr_.size(), 0);
}

TEST_F(Example, ArrValue) { MUST_EQUAL(arr_[0], 1); }

TEST_DEFAULT {
  MUST_TRUE(true, "");
  MUST_TRUE(true != false, "");
}
