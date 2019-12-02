#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "encdev"
#define CLASS_NAME "lakshyaEnc"
#define TRUE 1
#define FALSE 0

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lakshya A Agrawal");
MODULE_DESCRIPTION("A simple device driver for XOR encryption");

static int majorNumber;
static struct class * encdevClass  = NULL; 
static struct device * encdev = NULL;

static int numberOpens;
char[16] encryptionKey;
char * encryptedMessage;

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

static int __init encdev_init(void){
	numberOpens = 0;
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber<0){
		return majorNumber;
	}
	encdevClass = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(encdevClass)){
		unregister_chrdev(majorNumber, DEVICE_NAME);
		return PTR_ERR(encdevClass);
	}
	encdev = device_create(encdevClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	if (IS_ERR(encdev)){
		class_destroy(encdevClass);
		unregister_chrdev(majorNumber, DEVICE_NAME);
		return PTR_ERR(ebbcharDevice);
	}
	printk(KERNALERT "encdev: Module initialized");
	return 0;
}

static void __exit encdev_exit(void){
	device_destroy(encdevClass, MKDEV(majorNumber, 0));
	class_unregister(encdevClass);
	class_destroy(encdevClass);
	unregister_chrdev(majorNumber, DEVICE_NAME);
}

static int dev_open(struct inode *inodep, struct file *filep){
	if(numberOpens == 0){
	    get_random_bytes(&encryptionKey, 16);
		numberOpens = 1;
		printk(KERNALERT "encdev: Device opened");
		printk(KERNALERT "encdev: Encryption key: %s", encryptionKey);
		return 0;
	}
	return -1;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
	int error_count = 0;
	error_count = copy_to_user(buffer, message, size_of_message);
	if (error_count==0){
		return (size_of_message=0);
	}else{
		return -EFAULT;
	}
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
	
	sprintf(message, "%s(%zu letters)", buffer, len);
	size_of_message = strlen(message);
	return len;
}

static int dev_release(struct inode *inodep, struct file *filep){
	return 0;
}

module_init(encdev_init);
module_exit(encdev_exit);
