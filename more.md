# 一些问题

## 为什么要在 clang 的 uefi libc 完全可用后弃用项目

我觉得我应该相信我写的代码存在更多问题

## clang libc 看起来有个 overlay mode 能不能用

不能

overlay mode 重写了一些平台无关函数 但 overlay mode 似乎不是平台无关的

## 什么时候有 c++ 标准库用

不知道

clang libcxx 说有 libc 用就可以有 libcxx 用

但我没看懂怎么编译

## 能不能整点 gcc

不知道

但 [posix-uefi](https://gitlab.com/bztsrc/posix-uefi) 可以用 gcc 编译 可以看看
