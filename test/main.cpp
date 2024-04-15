#include "../test.h"
#include <cstdio>
#ifndef _WIN32
#include <unistd.h>
#endif

class Example : public cpptest::Test {
public:
  void SetUp() override { arr_.push_back(1); }
  std::vector<int> arr_;
};

INIT(Begin) { DEBUG("begin"); }
INIT(Begin1) { DEBUG("begin1"); }

END(End) { DEBUG("end"); }
END(End1) { DEBUG("end1"); }

ARGC_FUNC {
  if (argc == 2) {
    REGEX_FILT_TEST(argv[1]);
  }
}

TEST(Base, True) {
  MUST_EQUAL(0, 0);
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(1 == 1);
  MUST_TRUE(1 == 1, "");
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

TEST(Defer, VarValue) {
  int temp = 0;
  DEFER([&]() -> void {
    MUST_EQUAL(temp, 4);
    temp++;
  });
  DEFER([&]() -> void {
    MUST_EQUAL(temp, 3);
    temp++;
  });
  MUST_EQUAL(temp, 0);
  temp++;
  {
    DEFER([&]() -> void {
      MUST_EQUAL(temp, 2);
      temp++;
    });
    DEFER_DEFAULT {
      MUST_EQUAL(temp, 1);
      temp++;
    };
  }
}

#ifndef _WIN32
TEST(CostTime, Func) {
  auto cost = COSTTIME([]() { usleep(10); });
  DEBUG("cost time " << cost);
  MUST_TRUE(cost > 10000, "cost time wrong");
}

TEST(MutiThread, GoJoin) {
  GO_JOIN([]() {
    DEBUG("thread 1");
    usleep(10);
    DEBUG("thread 1");
  });
  GO_JOIN([]() { DEBUG("thread 2"); });
  GO_JOIN([]() { DEBUG("thread 3"); });
}

TEST(MutiThread, Go) {
  GO([]() {
    DEBUG("thread 1");
    usleep(10);
    DEBUG("thread 1");
  });
  GO([]() { DEBUG("thread 2"); });
  GO([]() { DEBUG("thread 3"); });
  usleep(30);
}
#endif
