# timingWheeler - 高精度时间轮定时器

`timingWheeler` 是一款开源的定时器，采用时间轮实现高精度定时触发注册函数

<!-- PROJECT SHIELDS -->

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![Mozilla License][license-shield]][license-url]

## 目录

- [快速开始](#快速开始)
  - [开发前的配置要求](#开发前的配置要求)
  - [安装步骤](#安装步骤)
- [作者](#作者)
- [鸣谢](#鸣谢)

---

## 快速开始


### 开发前的配置要求

1. gcc >= 4.7
2. cmake >= 3.2


### **编译**

```sh
git clone https://github.com/lervisnh/timingWheeler.git
cd timingWheeler
git submodule update --init --recursive
./tools/cmake_build
```


### **运行示例**
```sh
./build/bin/test/test_c_api

# 输出
1
2
3
unregister 4 : true
timer event using-size = 0,      expired-size = 3
```
以上二进制对应文件 : [test_c_api.cpp](../src/test/test_c_api.cpp)


### **安装**

```sh
cd build # timingWheeler/build 文件夹
make install
```


### **如何使用**
```c++
#include "timingWheeler/cAPI.h"

# 初始化定时器
timingWheeler::init_timer()

# 注册定时事件
auto timer_id = timingWheeler::register_timer(重复次数, 间隔时间, 定时函数, 定时函数入参1, 定时函数入参2, ...)

# 注销定时事件
timingWheeler::unregister_timer(register_timer返回的定时器ID)

# 终止定时器
timingWheeler::terminate_timer()
```

### 作者

lervisnh

Email : lervisnh@qq.com

### 版权说明

本项目授权 `Mozilla Public License 2.0` 许可，详情请参阅 [LICENSE.txt](https://github.com/lervisnh/timingWheeler/blob/master/LICENSE)

### 鸣谢

- [Img Shields](https://shields.io)
- [Choose an Open Source License](https://choosealicense.com)
- [GitHub Pages](https://pages.github.com)

<!-- links -->
[contributors-shield]: https://img.shields.io/github/contributors/lervisnh/timingWheeler.svg?style=flat-square
[contributors-url]: https://github.com/lervisnh/timingWheeler/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/lervisnh/timingWheeler.svg?style=flat-square
[forks-url]: https://github.com/lervisnh/timingWheeler/network/members
[stars-shield]: https://img.shields.io/github/stars/lervisnh/timingWheeler.svg?style=flat-square
[stars-url]: https://github.com/lervisnh/timingWheeler/stargazers
[issues-shield]: https://img.shields.io/github/issues/lervisnh/timingWheeler.svg?style=flat-square
[issues-url]: https://img.shields.io/github/issues/lervisnh/timingWheeler.svg
[license-shield]: https://img.shields.io/github/license/lervisnh/timingWheeler.svg?style=flat-square
[license-url]: https://github.com/lervisnh/timingWheeler/blob/master/LICENSE
