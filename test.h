/***********************************************
 * Author: chenxuan-1607772321@qq.com
 * change time:2023-04-14 20:56:46
 * description: this is a simple C++ testing framework
 * download or update:
 *    `wget https://gitee.com/chenxuan520/cpptest/raw/master/test.h -O test.h`
 ***********************************************/
#pragma once
#include <cstdlib>
#include <iostream>
#include <regex>
#include <string>
#include <utility>
#include <vector>

namespace cpptest {

// base class
class _test_base {
public:
  bool result_ = true;
  static std::string regex_filt_;
  static std::vector<std::pair<_test_base *, std::string>> test_arr_;
  static int success_;
  static int fail_;
  virtual void TestBody(){};
};
int _test_base::success_ = 0;
int _test_base::fail_ = 0;
std::string _test_base::regex_filt_ = "";
std::vector<std::pair<_test_base *, std::string>> _test_base::test_arr_;

// for TEST_F
class Test : public _test_base {
public:
  virtual void SetUp() {}
};

// std out or stderr
#define _TESTSTDOUT_(text) std::cout << text << std::endl;
#define _TESTSTDERR_(text) std::cerr << text << std::endl;

// print kinds of color
#define _TESTRED_(text) "\033[31m" << text << "\033[0m"
#define _TESTGREEN_(text) "\033[32m" << text << "\033[0m"
#define _TESTYELLOW_(text) "\033[33m" << text << "\033[0m"
#define _TESTBLUE_(text) "\033[34m" << text << "\033[0m"
#define _TESTCAR_(text) "\033[35m" << text << "\033[0m"
#define _TESTCYAN_(text) "\033[36m" << text << "\033[0m"

// setting macro
#define REGEX_FILT_TEST(test_regex)                                            \
  cpptest::_test_base::regex_filt_ = test_regex;

// util macro
#define _CONNECTSTR_(...) #__VA_ARGS__
#define _CLASS_FAIL_                                                           \
  cpptest::_test_base::fail_++;                                                \
  cpptest::_test_base::success_--;                                             \
  this->result_ = false
#define _FILE_LINE_MSG_ __FILE__ << ":" << __LINE__

// test name
#define _TEST_NAME__CREATE_(test_group, test_name)                             \
  test_group##test_name##_create
#define _TEST_NAME_(test_group, test_name) test_group##test_name##_test
#define _TEST_INIT_NAME__CREATE_(init_name) init_name##_init##_create
#define _TEST_INIT_NAME_(init_name) init_name##_init
#define _TEST_END_NAME__CREATE_(end_name) end_name##_end##_create
#define _TEST_END_NAME_(end_name) end_name##_end

// test init function,run before all test example
#define INIT(init_name)                                                        \
  int _TEST_INIT_NAME_(init_name)();                                           \
  auto _TEST_INIT_NAME__CREATE_(init_name) = _TEST_INIT_NAME_(init_name)();    \
  int _TEST_INIT_NAME_(init_name)()

// test end function,run after all test example
#define END(end_name)                                                          \
  class _TEST_END_NAME_(end_name) {                                            \
  public:                                                                      \
    ~_TEST_END_NAME_(end_name)();                                              \
  };                                                                           \
  _TEST_END_NAME_(end_name) _TEST_END_NAME__CREATE_(end_name);                 \
  _TEST_END_NAME_(end_name)::~_TEST_END_NAME_(end_name)()

// test function for users
#define TEST(test_group, test_name)                                            \
  class _TEST_NAME_(test_group, test_name) : public cpptest::_test_base {      \
  public:                                                                      \
    _TEST_NAME_(test_group, test_name)() {                                     \
      test_arr_.push_back({this, _CONNECTSTR_(test_group test_name)});         \
      this->success_++;                                                        \
    }                                                                          \
    void TestBody();                                                           \
  };                                                                           \
  _TEST_NAME_(test_group, test_name)                                           \
  _TEST_NAME__CREATE_(test_group, test_name);                                  \
  void _TEST_NAME_(test_group, test_name)::TestBody()

#define TEST_F(test_class, test_name)                                          \
  class _TEST_NAME_(test_class, test_name) : public test_class {               \
  public:                                                                      \
    _TEST_NAME_(test_class, test_name)() {                                     \
      test_arr_.push_back({this, _CONNECTSTR_(test_class test_name)});         \
      this->success_++;                                                        \
      this->SetUp();                                                           \
    }                                                                          \
    void TestBody();                                                           \
  };                                                                           \
  _TEST_NAME_(test_class, test_name)                                           \
  _TEST_NAME__CREATE_(test_class, test_name);                                  \
  void _TEST_NAME_(test_class, test_name)::TestBody()

// for use default name for create test example
#define TEST_DEFAULT TEST(TestDefault, __COUNTER__)

// some function for debug and judge
#define SKIP()                                                                 \
  _TESTSTDOUT_(_TESTYELLOW_("[SKIP] in [" << _FILE_LINE_MSG_ << "] : "));      \
  return;
#define DEBUG(text)                                                            \
  _TESTSTDOUT_(                                                                \
      _TESTYELLOW_("[DEBUG] in [" << _FILE_LINE_MSG_ << "] : " << text))
#define ERROR(text)                                                            \
  _TESTSTDERR_(_TESTCAR_("[ERROR] in [" << _FILE_LINE_MSG_ << "] : " << text)) \
  _CLASS_FAIL_
#define FATAL(text)                                                            \
  _TESTSTDERR_(_TESTRED_("[FATAL] in [" << _FILE_LINE_MSG_ << "] : " << text)) \
  _CLASS_FAIL_;                                                                \
  return;
#define PANIC(text)                                                            \
  _TESTSTDERR_(                                                                \
      _TESTRED_("[PANIC] in [" << _FILE_LINE_MSG_ << "] : " << text));         \
  exit(-1);
#define EXPECT_EQ(result, expect)                                              \
  if (result != expect) {                                                      \
    ERROR(_CONNECTSTR_(result want get expect but get) << " " << result)       \
  }
#define ASSERT_EQ(result, expect)                                              \
  if (result != expect) {                                                      \
    FATAL(result << " " << _CONNECTSTR_(!= expect));                           \
  }
#define MUST_EQUAL(result, expect)                                             \
  if (result != expect) {                                                      \
    FATAL(result << " " << _CONNECTSTR_(!= expect));                           \
  }
#define MUST_TRUE(flag, text)                                                  \
  if (!(flag)) {                                                               \
    FATAL(text);                                                               \
  }

// for gtest compatible
#define SUCCEED() SKIP()
#define FAIL() FATAL("")

// for argc message
static void (*__test_argc_funcpr__)(int argc, char *argv[]) = nullptr;
static void ArgcFunc(int argc, char **argv);
#define ARGC_FUNC                                                              \
  auto __test_temp_argc__ =                                                    \
      (cpptest::__test_argc_funcpr__ = cpptest::ArgcFunc);                     \
  void cpptest::ArgcFunc(int argc, char *argv[])

// the main function
#define RUN_ALL_TESTS                                                          \
  cpptest::_test_base base;                                                    \
  for (int i = 0; i < base.test_arr_.size(); i++) {                            \
    if (base.regex_filt_ != "") {                                              \
      std::regex pattern(base.regex_filt_);                                    \
      if (!std::regex_search(base.test_arr_[i].second, pattern)) {             \
        continue;                                                              \
      }                                                                        \
    }                                                                          \
    _TESTSTDOUT_(_TESTCYAN_("Runing:" << base.test_arr_[i].second));           \
    base.test_arr_[i].first->TestBody();                                       \
    if (base.test_arr_[i].first->result_) {                                    \
      _TESTSTDOUT_(_TESTGREEN_("Result:PASS") << std::endl);                   \
    } else {                                                                   \
      _TESTSTDOUT_(_TESTRED_("Result:Fail") << std::endl);                     \
    }                                                                          \
  }                                                                            \
  if (base.regex_filt_ != "") {                                                \
    _TESTSTDOUT_(_TESTBLUE_("Regex Filt:" << base.regex_filt_) << std::endl)   \
  }                                                                            \
  _TESTSTDOUT_(_TESTBLUE_("Total Run:" << base.success_ + base.fail_))         \
  _TESTSTDOUT_(_TESTBLUE_("Success Run:" << base.success_))                    \
  _TESTSTDERR_(_TESTBLUE_("Fail Run:" << base.fail_))

#define _RUN_TEST_MAIN_                                                        \
  int main(int argc, char *argv[]) {                                           \
    if (cpptest::__test_argc_funcpr__ != nullptr) {                            \
      cpptest::__test_argc_funcpr__(argc, argv);                               \
    }                                                                          \
    RUN_ALL_TESTS                                                              \
    return base.fail_;                                                         \
  }

}; // namespace cpptest

// default run auto
#ifndef TEST_CUSTOM_MAIN
_RUN_TEST_MAIN_
#endif
