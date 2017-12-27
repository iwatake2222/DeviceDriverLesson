CFILES = myDeviceDriver.c

obj-m := MyDeviceModule.o
MyDeviceModule-objs := $(CFILES:.c=.o)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

