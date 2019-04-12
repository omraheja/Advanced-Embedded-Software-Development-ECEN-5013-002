# _*_ Makefile _*_

LINUX_PATH:=$(HOME)/buildroot/output/build/linux-4796173fc58688055a99a1cef19a839174067220

COMPILE_PATH:=$(HOME)/buildroot/output/host/bin/

obj-m += sort_list.o

all:
	make ARCH=arm CROSS_COMPILE=$(COMPILE_PATH)arm-buildroot-linux-uclibcgnueabihf- -C $(LINUX_PATH) M=$(shell pwd) modules

clean:
	make -C $(LINUX_PATH) M=$(shell pwd) clean