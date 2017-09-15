ifneq ($(KERNELRELEASE),)
obj-m := led.o
else
KDIR := /home/wang/work/ok6410/linux2.6.28
all:
	make -C $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=/usr/local/arm/4.2.2-eabi/usr/bin/arm-linux-
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers  modul*
endif
