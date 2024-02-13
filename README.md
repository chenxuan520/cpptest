# 轻量级cpp测试框架

- 使用200行实现一个简单的C++测试框架

## 作者

- chenxuan

![](http://cdn.androidftp.top/pic/chenxuanweb/dog.png)

## 地址

- [gitee](https://gitee.com/chenxuan520/cpptest)

- [github](https://github.com/chenxuan520/cpptest)

## 特性

1. 支持类似gtest的TEST宏进行测试,其中大部分函数用法和gtest相同

2. 支持参数自定义,初始化函数,结束函数

3. 支持**正则表达式**过滤测试样例

4. **仅仅一个200行头文件**,轻量级,支持cmake,Makefile等管理

5. 跨平台支持(linux/mac/win)

## 起因

- 之前一致使用gtest,也非常好用,但是gtest太大了,体现在下面两个方面

	1. 使用cmake构建,当然大项目使用cmake构建无可厚非,但是有时候一些几千行的小项目的代码,可能使用Makefile构建,或者甚至使用g++编译,这个时候使用gtest必须引入cmake就显得麻烦了

	2. 代码量方面,很多时候发现测试框架比自己的代码体积还大是个沮丧的事情,特别是写个小项目的时候,大家更情愿进行快速测试,但是慢慢拉gtest就很烦

	3. 很多漫天飞舞的宏,如果各种各样的特性支持,但是很多情况下我们用的只有`TEST`

- 除此以外,其他的cpp测试框架要么太大,要么其他依赖(py or node),要么完全独立,导致新的学习成本

- 因此我决定做一个轻量化的测试框架,需要实现以下功能

	1. 基本函数和gtest类似,减低学习成本

	2. 只有一个头文件,直接下载下来就可以用

	3. 因为只有一个头文件,自然支持cmake,Makefile,以及各种构建工具

	4. 跨平台支持,支持类似gtest的过滤

## 安装

- linux/mac `wget https://gitee.com/chenxuan520/cpptest/raw/master/test.h`,然后创建一个cpp文件包含test头文件

- win 下载`https://gitee.com/chenxuan520/cpptest/raw/master/test.h`,然后创建一个cpp文件包含test头文件

## 使用

- 最好创建test文件夹保持隔离

- 不需要主动定义main函数,包含头文件就可以了

	- 当然如果不需要使用默认的main函数,可以在编译的时候加入`TEST_CUSTOM_MAIN`宏来避免main函数生效

	- 加入之后通过 RUN_ALL_TESTS 来运行测试用例

### 测试用例

- **TEST 宏**

	- 第一个参数是为了测试用例分组,第二个参数为了区分同组的测试用例
```cpp
TEST(TestGroup, TestName){
  MUST_EQUAL(1, 1);
}
```

- **TEST_DEFAULT 宏**

	- 如果简单临时测试,不想取名,使用这个宏可以避免参数
```cpp
TEST_DEFAULT{
  MUST_EQUAL(1, 1);
}
```

- **TEST_F 宏**

	- 参考google test的文档,基本类似,**不过命名空间是cpptest**
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

### 打印信息
> 这部分都会默认附带文件名和行数量

- **DEBUG(text)**

	- 打印在标准输出

- **ERROR(text)**

	- 打印在标准错误

- **FATAL(text)**

	- 和error相同,但是这个测试用例会直接返回

- PANIC(text)

	- 和eroor相同,但是直接终止程序

### 判断条件

- **EXPECT_EQ(result, expect)**

	- 判断是否相等,不相等测试用例会失败,类似ERROR(不会立刻返回)

- **MUST_EQUAL(result, expect)** 和 **ASSERT_EQ(resule, expect)**

	- 判断是否相等,不相等测试用例会失败,类似FATAL(立刻结束该测试用例)

- **MUST_TRUE(flag, text)**

	- 判断flag是否为真,为false会失败并打印text,类似FATAL

### 初始化

- INIT(init_name)

	- 在main函数执行之前调用,可以出现多个
```cpp
INIT(TestInit){
	...
	return 0; // 返回值没有意义
}
```

### 结束清理

- END(end_name)

	- 在main函数结束之后调用,可以相互先多个
```cpp
END(TestEnd){
	...
}
```

### 参数处理

- ARGC_FUNC

	- argc和argv直接调用就行,函数内已经传递

	- 只能存在一个
```cpp
ARGC_FUNC{
  if (argc==2) {
    REGEX_FILT_TEST(argv[1]);// 这里是传递正则表达式筛选的
  }
}
```

### 跳过用例

- SKIP()

	- 默认是作为成功统计
```cpp
TEST(TestGroup, TestName){
  SKIP();
  MUST_EQUAL(1, 1);
}
```

## QA

### 如何正则表达式筛选测试用例执行

- 使用ARGC_FUNC,参考 参数处理 部分的demo

### 如何实现错误和正确输出分开

- 默认失败的用例打印在标准错误,正确的在标准输出,两者管道就可以分开

### 出现编译警告-misc-definitions-in-headers

- 创建`.clang-tidy`,其中加入`Checks: -misc-definitions-in-headers`

## Demo

- 推荐写法
```cpp
// 包含头文件
#include "../test.h"
// 包含其他的单元测试头文件

// 设置可以使用正则表达式过滤
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

// 编写基础测试,核心放在其他头文件中
TEST(Base, True) { MUST_EQUAL(0, 0); }
```

## TODO

- [ ] 支持多线程并发测试
