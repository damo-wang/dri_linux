ifneq ($(KERNELRELEASE),)
	obj-m+=demo.o
	demo-objs+=driver.o
else
	KERNELDIR?=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
.PHNOY:clean
clean:
	rm -rf *.o *.cmd *.ko .*.mod.* modules.* Module.* .tmp*
