KDIR?=/home/wang/work/ok6410/Linux2.6.28/kernel/linux2.6.28
PWD:=$(shell pwd)
obj-m:=buzzer.o
default:
	echo $(KDIR)
	echo $(PWD)
	make -C $(KDIR) M=$(PWD) modules

