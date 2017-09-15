KDIR?=$(OK6410KDIR)
PWD:=$(shell pwd)
obj-m+=mkpwm.o

default:
	make -C $(KDIR) M=$(PWD) modules
