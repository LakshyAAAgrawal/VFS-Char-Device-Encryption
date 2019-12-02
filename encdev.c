#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

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

static int __init encdev_init(void){
  int xx;
    numberOpens = 0;
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    encryptedMessage = kmalloc(200, GFP_KERNEL);
    Length = 0;
    for(xx = 0; xx < 200; xx++){
      encryptedMessage[xx] = '0';
    }
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
	return PTR_ERR(encdev);
    }
    printk(KERN_ALERT "encdev: Module initialized");
    return 0;
}

static void __exit encdev_exit(void){
  kfree(encryptedMessage);
  device_destroy(encdevClass, MKDEV(majorNumber, 0));
  class_unregister(encdevClass);
  class_destroy(encdevClass);
  unregister_chrdev(majorNumber, DEVICE_NAME);
}

static char * genEncryptionKey(void){
    return "1234567890123456";
}

static int dev_open(struct inode *inodep, struct file *filep){
    if(numberOpens == 0){
	encryptionKey = genEncryptionKey();
	numberOpens = 1;
	isKeyRead = FALSE;
	printk(KERN_ALERT "encdev: Device opened");
	printk(KERN_ALERT "encdev: Encryption key: %s", encryptionKey);
	return 0;
    }
    printk("encdev: failed open attemt");
    return -1;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
  int xx;
    if(isKeyRead){
	if(len != Length){
	    printk("encdev: read len != strlen");
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
    }else{
	if(len != 16){
	    return -EFAULT;
	}
	if(copy_to_user(buffer, encryptionKey, 16) == 0){
	    isKeyRead = TRUE;
	    return(16);
	}else{
	    return -EFAULT;
	}
    }
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
    int newLength = len + (16 - (len%16));
    int t, j;
    copy_from_user(encryptedMessage, buffer, len);
    encryptedMessage[newLength] = '\0';
    for(t = 0; t < newLength / 16; t++){
      for(j = 0; j < 16; j++){
	encryptedMessage[t*16 + j] = encryptionKey[j] ^ encryptedMessage[t*16 + j];
      }
    }
    printk("encdev: write - received %d %d %s", len, strlen(encryptedMessage), encryptedMessage);
    Length = newLength;
    return newLength;
}

static int dev_release(struct inode *inodep, struct file *filep){
    numberOpens = 0;
    Length = 0;
    return 0;
}

module_init(encdev_init);
module_exit(encdev_exit);
