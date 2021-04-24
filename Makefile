#
# By default, the build is done against the running linux kernel source.
# To build against a different kernel source tree, set KDIR:
#
#    make KDIR=/path/to/kernel/source

ifdef KDIR
 KERNEL_SOURCES	 = $(KDIR)
else
 KERNEL_UNAME	:= $(shell uname -r)
 KERNEL_SOURCES	 = /lib/modules/$(KERNEL_UNAME)/build
endif

default: modules mmap_test
.PHONY: default
install: modules_install

.PHONY: install

modules:

%::
	$(MAKE) -C $(KERNEL_SOURCES) M=$$PWD $@

mmap_test: mmap_test.c
	$(CC) -o $@ $^ 

clean::
	$(MAKE) -C $(KERNEL_SOURCES) M=$$PWD $@
	rm mmap_test

.PHONY: FORCE
