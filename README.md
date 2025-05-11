# UEFI libc

UEFI 能用的标准 C 库

并没有写完 只是大概能用

一旦 clang 的 uefi libc 完全可用 这个项目应该被弃用依赖应该转向 clang 的 uefi libc

## 编译

必须使用 clang >= 20.1.3

支持目标`<arch>-unknown-uefi`或`<arch>-unknown-win32`

clang 的 uefi 可能有问题 尝试使用 win32 但注意不要引入 msvc 头

```bash
cmake -S . -B build \
    -DCMAKE_SYSTEM_PROCESSOR="<arch>" \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_INSTALL_PREFIX="/path/to/sysroot"
cmake --build build
cmake --install build
```

`<arch>`可以是`x86_64` `aarch64` `riscv64`等
但目前仅有`x86_64`能通过编译

## 使用

见 [example/README.md](example/README.md)

不应该使用源文件依赖

## 已知问题

### 部分函数未实现

链接时找不符号 自己实现一下

### 没有 libm <math.h>

用 rust 的独立 libm https://crates.io/crates/libm

`no_std`目标`<arch>-unknown-uefi`编译静态库

### libc 居然需要 c++ 运行时

哎呀 能用就行

用到的 c++ 函数只有`new`和`delete` 应该不会有循环依赖的问题
