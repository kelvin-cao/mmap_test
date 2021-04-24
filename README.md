# A simple mmap kernel module
A couldn't be simpler kernel module to demo the mmap() implemenation in the
kernel. An user space applicaiton is also included to initiate the call.

The kernel module `mmap_module` initializes a page of memory to zeor when it's
loaded. The user space process `mmap_test` map this page to its address space
and advance the first int value (at offset 0 of the page) by one in each run.

## Build
Simply run `make` to build the kernel module `mmap_module.ko` and the user space
application `mmap_test`.

## Run
```
$ sudo mknod /dev/mmap_dev c 44 0
$ sudo insmod mmap_module.ko
$ sudo dmesg | tail -n 2
[114343.123000] mmap_module: init value (offset 0x0): 0x0.
[114343.123005] mmap_module: loaded.
$ sudo ./mmap_test
value (offset 0x0): 0x0
$ sudo ./mmap_test
value (offset 0x0): 0x1
$ sudo ./mmap_test
value (offset 0x0): 0x2
$ sudo ./mmap_test
value (offset 0x0): 0x3
$ sudo ./mmap_test
value (offset 0x0): 0x4
$ sudo ./mmap_test
value (offset 0x0): 0x5
$ sudo ./mmap_test
value (offset 0x0): 0x6
$ sudo ./mmap_test
value (offset 0x0): 0x7
$ sudo rmmod mmap_module
$ sudo dmesg | tail -n 2
[114410.397036] mmap_module: exit value (offset 0x0): 0x8.
[114410.397042] mmap_module: unloaded.

```

