CC = gcc
obj-m := driver_cd.o
KDIR := /lib/modules/`uname -r`/build
all:
	make -C $(KDIR) M=`pwd` modules
clean:
	make -C $(KDIR) M=`pwd` clean
