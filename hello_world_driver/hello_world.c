#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>


MODULE_LICENSE("MIT");
MODULE_AUTHOR("Ben Gabay");
MODULE_DESCRIPTION("Hello World LKM");

static int __init HelloStart(void)
{
	printk("Hello Kernel!\n");
	return 0;
}

static void __exit HelloEnd(void)
{
	printk("Goodbye Kernel!\n");
}

module_init(HelloStart);
module_exit(HelloEnd);
