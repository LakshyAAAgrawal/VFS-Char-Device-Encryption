obj-m += encdev.o

default: all
all: userprog
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
userprog: userprog.c
	gcc userprog.c -o userprog
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
