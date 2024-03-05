target=test

CC=aarch64-linux-gnu-gcc

all:$(target)


test : test.o
	$(CC) $^ -o $@ $(LDFLAGS) -lpthread
	@echo "测试使用register_chrdev注册的字符设备..."

%.o : %.c
	$(CC) -c $^ -o $@  $(CFLAGS) $(DEFINES)

clean :
	rm -rf *.o
	rm -rf $(target)




.PHONY	: clean run
