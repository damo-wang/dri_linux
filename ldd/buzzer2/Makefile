ifneq ($(KERNELRELEASE),)
obj-m := buzzer.o
else
KDIR := $(OK6410KDIR)
all:
	make -C $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=/usr/local/arm/4.2.2-eabi/usr/bin/arm-linux-
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers  modul*
endif
