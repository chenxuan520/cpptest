#include "../test.h"

ARGC_FUNC{
  if (argc==2) {
    REGEX_FILT_TEST(argv[1]);
  }
}

TEST(Base, True){
  MUST_EQUAL(0, 0);
}

TEST_DEFAULT{
  MUST_TRUE(true, "");
  MUST_TRUE(true!=false, "");
}
