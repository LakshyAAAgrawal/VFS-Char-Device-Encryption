obj-m += encdev.o
obj-m += decdev.o

default: all
all: userprog decryption
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
userprog: userprog.c
	gcc userprog.c -o userprog
decryption: decryption.c
	gcc decryption.c -o decryption
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
