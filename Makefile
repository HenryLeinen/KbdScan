TARGET_MODULE:=KbdScan

# If we are running by kernel building system
# ifneq ($(KERNELRELEASE),)
#	$(TARGET_MODULE)-objs := KbdScan.o mygpio.o
	obj-m := KbdScan.o
# If we are running without kernel build system
# else
	BUILDSYSTEM_DIR:=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)

all:
# run kernel build system to make module
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) modules

clean:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) clean

load:
	sudo insmod ./$(TARGET_MODULE).ko
	tail -n 20 /var/log/kern.log
unload:
	sudo rmmod ./$(TARGET_MODULE).ko
	tail -n 20 /var/log/kern.log
# endif

