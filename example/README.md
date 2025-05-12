# 使用

一旦编译并安装 uefi-libc 任何仅使用标准 C 库的程序应该可以轻松交叉编译到 uefi

标准头不暴露也不引入 uefi 平台特性 理论上可以配合任何 uefi 平台库使用

## 例子

### 不使用构建系统的简单例子

```c
// main.c

#include <stdio.h>

int main(int argc, char** argv) {
    printf("Hello, world!\n");
    printf("argc: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
}
```

编译成 efi 可执行文件

```bash
clang --target=x86_64-unknown-uefi \
    -o hello.efi main.c \
    -I/path/to/sysroot/include \
    -L/path/to/sysroot/lib \
    /path/to/sysroot/lib/libc.a \
    /path/to/sysroot/lib/libc++.a
```

生成 hello.efi: PE32+ executable for EFI (application), x86-64, 5 sections

使用 qemu 运行

```bash
qemu-system-x86_64 \
    -bios /path/to/OVMF \
    -drive file=fat:rw:.,format=raw
```

```nsh
FS0:
.\hello.efi 123
```

输出

```text
Hello, world!
argc: 2
argv[0]: FS0:\hello.efi
argv[1]: 123
```
### 使用 CMake 的例子

当前目录

```bash
cmake -S . -B build \
    --toolchain /path/to/x86_64-uefi-clang.cmake
cmake --build build
```

### 使用自定义构建脚本的例子

编译 zlib 1.3.1

```bash
export PREFIX=/path/to/sysroot
export CC=clang
export CFLAGS="--target=x86_64-uefi -I$PREFIX/include $PREFIX/lib/libc.a $PREFIX/lib/libc++.a"
./configure \
    --static \
    --prefix=$PREFIX \
    --solo
make
make install
```

这只是个长得有点像 GNU autotools 的东西 它不是

`--solo`因为我们没有`sys/*`

`--static`因为 UEFI 不支持动态链接
