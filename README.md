# UEFI libc

UEFI 能用的标准 C 库

并没有写完 只是大概能用

## 编译

必须使用 clang >= 20.1.3

目标`<arch>-unknown-uefi`或`<arch>-unknown-win32`

clang 的 uefi 可能有问题 尝试使用 win32 但注意不要引入 msvc 头

## 已知问题

### 部分函数未实现

链接时找不符号 自己实现一下

### 没有 libm <math.h>

用 rust 的独立 libm https://crates.io/crates/libm

`no_std`目标`<arch>-unknown-uefi`编译静态库
