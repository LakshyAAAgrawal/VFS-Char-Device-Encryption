#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DEVICE_NAME "decdev"
#define CLASS_NAME "lakshyaDec"
#define TRUE 1
#define FALSE 0

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lakshya A Agrawal");
MODULE_DESCRIPTION("A simple device driver for XOR decryption");

static int majorNumber;
static struct class * decdevClass  = NULL; 
static struct device * decdev = NULL;

static int numberOpens;
char * encryptionKey;
char * encryptedMessage;
int isKeyRead = FALSE;
int Length;

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

static int __init decdev_init(void){
  int xx;
    numberOpens = 0;
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    encryptedMessage = kmalloc(200, GFP_KERNEL);
    encryptionKey = kmalloc(17, GFP_KERNEL);
    Length = 0;
    for(xx = 0; xx < 200; xx++){
      encryptedMessage[xx] = '0';
    }
    if (majorNumber<0){
	return majorNumber;
    }
    decdevClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(decdevClass)){
	unregister_chrdev(majorNumber, DEVICE_NAME);
	return PTR_ERR(decdevClass);
    }
    decdev = device_create(decdevClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(decdev)){
	class_destroy(decdevClass);
	unregister_chrdev(majorNumber, DEVICE_NAME);
	return PTR_ERR(decdev);
    }
    printk(KERN_ALERT "decdev: Module initialized");
    return 0;
}

static void __exit decdev_exit(void){
  kfree(encryptedMessage);
  kfree(encryptionKey);
  device_destroy(decdevClass, MKDEV(majorNumber, 0));
  class_unregister(decdevClass);
  class_destroy(decdevClass);
  unregister_chrdev(majorNumber, DEVICE_NAME);
}

static int dev_open(struct inode *inodep, struct file *filep){
    if(numberOpens == 0){
	numberOpens = 1;
	isKeyRead = FALSE;
	printk(KERN_ALERT "decdev: Device opened");
	printk(KERN_ALERT "decdev: Encryption key: %s", encryptionKey);
	return 0;
    }
    printk("decdev: failed open attemt");
    return -1;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
  int xx;
  
  if(len != Length){
    printk("decdev: read len != strlen");
    return -EFAULT;
  }
  if(copy_to_user(buffer, encryptedMessage, Length) == 0){
    for(xx = 0; xx < 200; xx++){
      encryptedMessage[xx] = '0';
    }
    return(Length);
  }else{
    return -EFAULT;
  }
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
  int newLength = len + (16 - (len%16));
    int t, j;
    copy_from_user(encryptedMessage, buffer, len);
    encryptedMessage[newLength] = '\0';
    for(t = 0; t < 16; t++){
      encryptionKey[t] = encryptedMessage[t];
    }
    printk("decdev: write - received %d %d %s", len, strlen(encryptedMessage), encryptedMessage);
    for(t = 1; t < newLength / 16; t++){
      for(j = 0; j < 16; j++){
	encryptedMessage[(t-1)*16 + j] = encryptionKey[j] ^ encryptedMessage[t*16 + j];
      }
    }
    printk("decdev: write - received %d %d %s", len, strlen(encryptedMessage), encryptedMessage);
    Length = newLength;
    return newLength;
}

static int dev_release(struct inode *inodep, struct file *filep){
    numberOpens = 0;
    Length = 0;
    return 0;
}

module_init(decdev_init);
module_exit(decdev_exit);
