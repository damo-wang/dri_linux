ifneq ($(KERNELRELEASE),)
	#obj-m+=tempM.o
	#tempM-objs+=test.o
	obj-m+=test.o
else
	KERNELDIR?=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)
default::
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
