TARGET_MODULE:=kbdscan

# If we are running by kernel building system
	obj-m += $(TARGET_MODULE).o
	$(TARGET_MODULE)-objs := KbdScan.o mygpio.o myqueue.o keymap.o
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


