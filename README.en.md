# Lightweight Cpp Testing Framework

- A simple C++ testing framework

## Author

- Chenxuan

![](http://cdn.androidftp.top/pic/chenxuanweb/dog.png)

## Address

- [Gitee](https://gitee.com/chenxuan520/cpptest)

- [GitHub](https://github.com/chenxuan520/cpptest)

## Features

1. It supports the TEST macro similar to GTest for testing, and most of the function usages are the same as GTest.

2. It supports custom parameters, initialization functions, and end functions.

3. It supports **regular expression** to filter test cases.

4. **Just a 500-line header file**, lightweight, and supports management by CMake, Makefile, etc.

5. Cross-platform support (Linux/Mac/Win).

6. Easy to expand. Since there is only one.h file and very little code, it can be easily understood and modified.

7. It comes with a large number of functions that are convenient for testing, like DEFER, GO.

8. It includes a stress testing framework, which is convenient for testing the time-consuming of functions.

## Reasons

- I had been using GTest before, and it was very useful. However, GTest is too large, which is reflected in the following two aspects.

    1. When using CMake for building, of course, it's understandable to use CMake for building large projects. But sometimes for some small projects with only a few thousand lines of code, it may be more appropriate to use Makefile for building or even compile with g++. At this time, it becomes troublesome to have to introduce CMake when using GTest.

    2. In terms of the amount of code, it's frustrating to find that the testing framework is larger than your own code in many cases, especially when writing a small project. People would rather conduct quick tests, but it's annoying to slowly pull in GTest.

    3. There are a lot of macros flying around, and various features are supported, but in many cases, we only use `TEST`.

- Besides, other Cpp testing frameworks are either too large, have other dependencies (Python or Node), or are completely independent, resulting in new learning costs.

- Therefore, I decided to create a lightweight testing framework, which needs to implement the following functions.

    1. The basic functions are similar to GTest to reduce the learning cost.

    2. There is only one header file, and it can be used directly after downloading.

    3. Since there is only one header file, it naturally supports CMake, Makefile, and various building tools.

    4. It has cross-platform support and supports filtering similar to GTest.

## Installation
> The compiler needs to support at least C++11.

- For Linux/Mac, use `wget https://gitee.com/chenxuan520/cpptest/raw/master/test.h -O test.h`, and then create a Cpp file that includes the test header file.

- For Windows, download `https://gitee.com/chenxuan520/cpptest/raw/master/test.h`, and then create a Cpp file that includes the test header file.

## Usage

- It's better to create a test folder to keep isolation.

- There is no need to actively define the main function. Just including the header file is enough.

    - Of course, if you don't want to use the default main function, you can add the `TEST_CUSTOM_MAIN` macro during compilation to prevent the main function from taking effect.

    - After adding it, run the test cases through `RUN_ALL_TESTS`.

### Test Cases

- **TEST Macro**

    - The first parameter is for grouping test cases, and the second parameter is for distinguishing test cases in the same group.
```cpp
TEST(TestGroup, TestName){
  MUST_EQUAL(1, 1);
}
```

- **TEST_DEFAULT Macro**

    - If you want to conduct a simple and temporary test and don't want to give a name, you can use this macro to avoid parameters.
```cpp
TEST_DEFAULT{
  MUST_EQUAL(1, 1);
}
```

- **TEST_F Macro**

    - Refer to the documentation of Google Test. It's basically similar, **but the namespace is cpptest**.
```cpp
class Example : public cpptest::Test {
public:
  void SetUp() override{
    arr.push_back(1);
  }
  std::vector<int> arr_;
};
TEST_F(Example, ArrValue){
  MUST_EQUAL(arr_[0], 1);
}
```

### Printing Information
> This part will be accompanied by the file name and line number by default.

- **DEBUG(text)**

    - Print on the standard output.

- **ERROR(text)**

    - Print on the standard error, and the case will be set as failed.

- **FATAL(text)**

    - It's the same as ERROR, but this test case will return directly.

- PANIC(text)

    - It's the same as ERROR, but it will terminate the program directly.

### Judgment Conditions

- **EXPECT_EQ(result, expect)**

    - Judge whether they are equal. If not, the test case will fail, similar to ERROR (it won't return immediately).

- **EXPECT_TRUE(flag, text)**

    - Judge whether the flag is true. If it's false, it will fail and print the text, similar to ERROR.

- **ASSERT_EQ(result, expect)**

    - Judge whether they are equal. If not, the test case will fail, similar to FATAL (it will end this test case immediately).

- **MUST_EQUAL(result, expect)** and **ASSERT_EQ(result, expect)**

    - Judge whether they are equal. If not, the test case will fail, similar to FATAL (it will end this test case immediately).

- **MUST_TRUE(flag, text)**

    - Judge whether the flag is true. If it's false, it will fail and print the text, similar to FATAL.

### Initialization

- INIT(init_name)

    - It will be called before the main function is executed, and multiple ones can appear.
```cpp
INIT(TestInit){
	...
}
```

### End Cleanup

- END(end_name)

    - It will be called after the main function ends, and multiple ones can appear.
```cpp
END(TestEnd){
	...
}
```

### Parameter Handling

- ARGC_FUNC

    - argc and argv can be directly called as they have been passed within the function.

    - Only one can exist.
```cpp
ARGC_FUNC{
  if (argc==2) {
    REGEX_FILT_TEST(argv[1]);// Here is to pass the regular expression for filtering.
  }
}
```

### Skipping Cases

- SKIP() or SUCCESS()

    - They are counted as successful by default.
```cpp
TEST(TestGroup, TestName){
  SKIP();
  MUST_EQUAL(1, 1);
}
```

### Defer

- Similar to the defer statement in Go.

- `DEFER(std::function<void(void)>)`

    - Similar to the defer statement in Go, it will run by default after the scope ends and won't be interrupted by MUST_EQUAL.

    - It runs in the order of last in first out, similar to a stack.
```cpp
DEFER([&]() -> void {
  MUST_EQUAL(temp, 2);
});
```

- DEFER_DEFAULT

    - Similar to defer but doesn't need to pass a pointer parameter. You can directly write code within {}. It will automatically capture variables by reference by default.

    - Note that **a semicolon needs to be added after the closing curly brace {}** because it's a statement.
```cpp
DEFER_DEFAULT {
  MUST_EQUAL(temp, 1);
};
```

### Go

- Similar to the go statement in Go.

- `GO(std::function<void(void)>)`

    - Pass a function. It's recommended to use lambda expressions. In this case, the thread is detached by default.
```cpp
TEST(MutiThread, Go) {
  SKIP();
  GO([]() {
    DEBUG("thread 1");
    usleep(10);
    DEBUG("thread 1");
  });
  GO([]() { DEBUG("thread 2"); });
  GO([]() { DEBUG("thread 3"); });
  usleep(30);
}

```

- `GO_JOIN(std::function<void(void)>)`

    - It's joined by default, and the joining time is after the current scope ends.
```cpp
TEST(MutiThread, GoJoin) {
  GO_JOIN([]() {
    DEBUG("thread 1");
    usleep(10);
    DEBUG("thread 1");
  });
  GO_JOIN([]() { DEBUG("thread 2"); });
  GO_JOIN([]() { DEBUG("thread 3"); });
}

```

- `GO_WAIT(std::function<void(void)>)`

    - It's used in the Add function of WaitGroup to implement the function similar to Go's WaitGroup.
```cpp
TEST(MutiThread, GoJoinFor) {
  cpptest::WaitGroup wait_group;
  for (int i = 0; i < 3; i++) {
    wait_group.Add(GO_WAIT([=]() {
      DEBUG("thread " << i);
      usleep((3 - i) * 100);
      DEBUG("thread again " << i);
    }));
  }
  wait_group.Wait();
}

```

### Stress Testing

- `BENCHMARK(BenchGroup, BenchName)` is similar to the TEST macro and is mainly used to define cases.

- `BENCHFUNC(std::function<void(void)>)` tests the execution time of the function, and the number of loops is dynamically and automatically adjusted internally by the system.
```cpp
BENCHMARK(Bench, FuncEmpty) {
  set_bench_msg("empty func");
  BENCHFUNC([&]() {});
}
```

- `BENCH_INDEPENDENT(func, run_time)` is an independent stress test, which can be used in combination with TEST and assertions. The return value is the running time in nanoseconds.

## QA

### How to Filter Test Cases for Execution with Regular Expressions

- Use ARGC_FUNC. Refer to the demo in the Parameter Handling section.

### How to Separate Error and Correct Outputs

- By default, failed cases are printed on the standard error, and correct ones are printed on the standard output. They can be separated by piping.

### Compilation Warning - misc-definitions-in-headers Appears

- Create a `.clang-tidy` file and add `Checks: -misc-definitions-in-headers` to it.

## Demo

- Refer to test/main.cpp which basically includes all usages.

- Recommended writing method
```cpp
// Include the header file
#include "../test.h"
// Include other unit test header files
// Set to be able to use regular expression filtering
ARGC_FUNC {
  if (argc == 2) {
    REGEX_FILT_TEST(argv[1]);
  }
}
INIT(Begin) {
  DEBUG("begin");
  return 0;
}
END(End) { DEBUG("end"); }
// Write basic tests. The core is placed in other header files.
TEST(Base, True) { MUST_EQUAL(0, 0); }
```

## TODO

- [ ] Support multi-thread concurrent testing 
