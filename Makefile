obj-m = htest.o

all:
	$(MAKE) -C /lib/modules/`uname -r`/build M=$(PWD) modules
clean:
	rm *.o  *.ko
