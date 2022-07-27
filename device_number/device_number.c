#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>


MODULE_LICENSE("MIT");
MODULE_AUTHOR("Ben Gabay");
MODULE_DESCRIPTION("Register Device Number LKM");

#define DEVICE_NAME "mydevice"

int DEVICE_MAJOR;

int driver_open(struct inode* device_file, struct file* instance)
{
	printk("Driver Open\n");
	return 0;
}

int driver_release(struct inode* device_file, struct file* instance)
{
	printk("Driver Close\n");
	return 0;
}

struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_release
};

int __init start_module(void)
{
	printk("Registering device...\n");
	DEVICE_MAJOR = register_chrdev(0, DEVICE_NAME, &fops);
	if (DEVICE_MAJOR >= 0)
	{
		printk("Device number registered with major: %d\n", DEVICE_MAJOR);
	}
	else {
		printk("Registering the character device failed with %d\n", DEVICE_MAJOR);
	}
	return 0;
}

void __exit exit_module(void)
{
	printk("Unregistering Device with major %d...\n", DEVICE_MAJOR);
	unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);

	// if (ret == 0)
    //{   
      //  printk("Device number unregistered\n");
    //}
    //else {
      //  printk("Unregistering the character device failed with %d\n", DEVICE_MAJOR);
    //}

}

module_init(start_module);
module_exit(exit_module);
