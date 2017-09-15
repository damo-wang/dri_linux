obj-m+=demo.o
KDIR?=/lib/modules/$(shell uname -r)/build
KDIRARM?=../../../../..
PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

