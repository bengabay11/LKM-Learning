/*
* chardev.c: Creates a read−only char device that says how many times
* you've read from the dev file
*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "mychardev"
#define BUF_LEN 80

MODULE_LICENSE("MIT");
MODULE_AUTHOR("Ben Gabay");
MODULE_DESCRIPTION("Character Device LKM");

static int Device_Major;
static int Device_Open = 0;
static char msg[BUF_LEN];
static char *msg_Ptr;

static int device_open(struct inode *inode, struct file *file)
{
    static int counter = 0;
    if (Device_Open)
    {
        return -EBUSY;
    }
    Device_Open++;
    sprintf(msg, "I already told you %d times Hello world!\n", counter++);
    msg_Ptr = msg;
    try_module_get(THIS_MODULE);
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    Device_Open--;
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
    printk(KERN_ALERT "Write operation isn't supported.\n");
    return -EINVAL;
}

static ssize_t device_read(struct file* flip, char* buffer, size_t length, loff_t* offset)
{
    int bytes_read = 0;
    if (*msg_Ptr == 0)
        return 0;
    while (length && *msg_Ptr)
    {
        put_user(*(msg_Ptr++), buffer++);
        length--;
        bytes_read++;
    }
    return bytes_read;
}

struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

static int __init start_module(void)
{
	Device_Major = register_chrdev(0, DEVICE_NAME, &fops);
	if (Device_Major < 0)
	{
		printk(KERN_ALERT "Registering char device failed with %d\n", Device_Major);
	}
	printk(KERN_INFO "The device %s assigned to major number %d\n", DEVICE_NAME, Device_Major);
	printk(KERN_INFO "to talk to the driver, create a dev file with 'mknod /dev/%s c %d 0'\n", DEVICE_NAME, Device_Major);
	printk(KERN_INFO "Try to cat and echo to the device file\n");
	return 0;
}

static void __exit exit_module(void)
{
	unregister_chrdev(Device_Major, DEVICE_NAME);
	printk("Device with major %d unregistered\n", Device_Major);
}

module_init(start_module);
module_exit(exit_module);
