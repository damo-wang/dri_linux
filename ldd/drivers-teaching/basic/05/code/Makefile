ifneq ($(KERNELRELEASE),)
	obj-m+=demo.o
	demo-objs+=test05.o
else
	KERNELDIR?=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
.PHNOY:clean
clean:
	rm -rf *.o *.ko *.mod.* modules.* Module.* 
