obj-m += encdev.o

all: userprog
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
userprog: userprog.c
	gcc userprog.c -o userprog
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
