# 轻量级cpp测试框架
- 使用不到200行实现一个简单的C++测试框架
## 作者
- chenxuan

![{text}](http://cdn.androidftp.top/pic/chenxuanweb/dog.png)

## 特性
1. 支持类似gtest的TEST宏进行测试
2. 支持参数自定义,初始化函数,结束函数
3. 支持**正则表达式**过滤
4. 仅仅一个头文件,轻量级,支持cmake,Makefile等管理
5. 跨平台支持(linux/mac/win)
## 下载
- linux/mac `wget https://gitee.com/chenxuan520/cpptest/raw/master/test.h`
- win 下载`https://gitee.com/chenxuan520/cpptest/raw/master/test.h`
## 使用
- 最好创建test文件夹保持隔离
- 不需要主动定义main函数,包含头文件就可以了
	- 当然如果不需要使用默认的main函数,可以在编译的时候加入`TEST_CUSTOM_MAIN`宏来避免main函数生效
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
- **EXPECT_EQ(result, expect)**和**MUST_EQUAL(result, expect)**
	- 判断是否相等,不相等测试用例会**失败**,类似FATAL
- **MUST_TRUE(flag, text)**
	- 判断flag是否为真,为false会失败并打印text,类似FATAL
### 初始化
- INIT(init_name)
	- 在main函数执行之前调用,可以出现多个
```cpp
INIT(TestInit){
	...
}
```
### 结束清理
- END(end_name)
	- 在main函数结束之后调用,可以相互先多个
```cpp
INIT(TestEnd){
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
    REGEX_FILT_TEST(argv[1]);
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
