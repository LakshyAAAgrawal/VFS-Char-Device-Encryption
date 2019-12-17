# VFS Block Encryption
## Description of the module and implementation:
    The system consists of 4 files:
	1. encdev.c
	2. decdev.c
	3. userprog.c
	4. decryption.c

	The files encdev.c and decdev.c define the module for device encdev and decdev respectively.
	encdev: The file mainatins the number of processes having opened the file. If it is 1, no more processes are allowed to open it.
			Whenever the file is opened, a random 128-bit or 16-byte encryption key is generated using generate_random_bytes.
			The user must then supply the text to be encrypted, which is encrypted in blocks. It is stored internally in a data structure(array).
			The file supplies the functions for file_operations struct for VFS. Memory allocations and deallocations are handled approperiately.
			The userInput text is padded with a random string to make it a multiple of 16-bytes in length.
	decdev: The file mainatins the number of processes having opened the file. If it is 1, no more processes are allowed to open it.
			The user must then supply the encrypted text, which is decrypted in blocks. It is stored internally in a data structure(array).
			The file supplies the functions for file_operations struct for VFS.

	userprog.c: This file reads the toEncrypted file, and writes to encdev. Then it reads the encryption key from the device.
				Then the encrypted text and writes it to the file "encrypted" prefixed with the encryption key.
	decryption.c: This file reads the "encrypted" file and writes to decdev. It then reads the decrypted file and writes it to
				  "decrypted".

## Compilation and running:
	Compilation of the modules and programs:
		make
	Installation of kernel modules:
		sudo ./install.sh

	The userprograms can be run:
		sudo ./userprog
		sudo ./decryption
	Ensure that a file named toEncrypt is present in the (pwd) before running userprog.

## Inputs from user:
	For user space programs, the user must supply the file toEncrypt, which upon encryption by userprog is written along with encryption
	key to a file named encrypted. The program decryption generates a file named decrypted using the file as input.

	The kernel devices work as follows:
	encdev: The user must first open the device. The user then writes a string to the device. The write() returns an integer. The user
	   		calls the read function with argument size as 16. This returns the encryption key used. The next read must be of the size
			previously returned by the write function. This returns the encrypted text.
	decdev: The user must first open the device. The user then writes a string to the device prefixed with the 16-byte encryption key
	   		used for encryption. The user can then read in the next step the number of bytes of the text, and this will give the decrypted
			text.

## Expected output:
	The file named "decrypted" after execution of both the programs must contain the original text in "toEncrypt" padded with some
	random characters.

## Error values:
	A negative return value from any of the function signifies an error in one of the operations. If the device open throws an error,
	it is mostly due to being open in some other process. If the read function throws an error it is because of the length not being
	16 in the first call, and not being the expected length of string in the second.
