ARCH := ARCH=arm64
CROSS_PATH := CORSS_COMPILE=aarch64-linux-gnu-
KERN_DIR := /chic/linux-5.10.1                                                                                                                                          
obj-m   += simple.o

all:
	make $(ARCH) $(CROSS_PATH) -C $(KERN_DIR) M=$(PWD) modules
clean:
	make $(ARCH) $(CROSS_PATH) -C $(KERN_DIR) M=$(PWD) clean
	rm -f *.ko


