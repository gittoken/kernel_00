KDIR=/lib/modules/`uname -r`/build
kbuild:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` clean
	rm -f test

test:
	#insmod kernel_module.ko
	gcc test.c -o test -Wall
	#rmmod kernel_module

restart:
	make -C $(KDIR) M=`pwd` clean
	rm -f test
	rmmod kernel_module
	make -C $(KDIR) M=`pwd`
	insmod kernel_module.ko
	gcc test.c -o test -Wall
