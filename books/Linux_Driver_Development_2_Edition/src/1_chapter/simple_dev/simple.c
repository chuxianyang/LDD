#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/cdev.h>
#include <linux/version.h>
#include <linux/vmalloc.h>
#include <linux/ctype.h>
#include <linux/pagemap.h>

#include <linux/slab.h>
#include "simple.h"

struct simple_dev *simple_device;
static unsigned char simple_inc = 0;
static unsigned char demobuffer[256];

static void __exit simple_cleanup_module(void);


int simple_open(struct inode *inode, struct file *filp)
{
	struct simple_dev *dev;
	if(simple_inc>0)
		return -ERESTARTSYS;
		
	simple_inc++;
	dev = container_of(inode->i_cdev, struct simple_dev, cdev); //find cdev
	filp->private_data = dev;
	return 0;
}

int simple_release(struct inode *inode, struct file *filp)
{
	simple_inc--;
	return 0;
}

ssize_t simple_read(struct file *filp, char __user *buf, size_t count,loff_t *f_pos)
{
	if (copy_to_user(buf, demobuffer, count))
	   count=-EFAULT;

	return count;
}

ssize_t simple_write(struct file *filp, const char __user *buf, size_t count,loff_t *f_pos)
{
	if (copy_from_user(demobuffer+*f_pos, buf, count))
	{
		count = -EFAULT;
	}
	return count;
}

struct file_operations simple_fops =
{
	.owner =    THIS_MODULE,
	.read =     simple_read,
	.write =    simple_write,
	.open =     simple_open,
	.release =  simple_release,
};


/*******************************************************
                MODULE ROUTINE
*******************************************************/
static int __init simple_setup_module(void)
{
	int result;
	dev_t dev = 0;

	dev = MKDEV(SIMPLE_MAJOR, SIMPLE_MINOR);
	result = register_chrdev_region(dev, 1, "DEMO");
	if (result < 0) {
		printk(KERN_WARNING "DEMO: can't get major %d\n", SIMPLE_MAJOR);
		return result;
	}

	simple_device = kmalloc(sizeof(struct simple_dev), GFP_KERNEL);
	if (!simple_device)
	{
		result = -ENOMEM;
		goto fail;
	}
	memset(simple_device, 0, sizeof(struct simple_dev));
	cdev_init(&simple_device->cdev, &simple_fops);

	simple_device->cdev.owner = THIS_MODULE;
	simple_device->cdev.ops = &simple_fops;

	result = cdev_add (&simple_device->cdev, dev, 1);
	if(result)
	{
		printk(KERN_NOTICE "Error %d adding DEMO\n", result);
		goto fail;
	}
	return 0;

fail:
	simple_cleanup_module();
	return result;	
	
}

static void __exit simple_cleanup_module(void)
{
	dev_t devno = MKDEV(SIMPLE_MAJOR, SIMPLE_MINOR);
	if (simple_device) {
		cdev_del(&simple_device->cdev);
		kfree(simple_device);
	}
	unregister_chrdev_region(devno,1);
}

module_init(simple_setup_module);
module_exit(simple_cleanup_module);

/*  Driver Information  */
#define DRIVER_VERSION  "2.0.0"
#define DRIVER_AUTHOR   "xianyangchu_chic@163.com"
#define DRIVER_DESC     "Linux \"cdev\" module for LDD-LinuxDeviceDrivers devices"
#define DRIVER_LICENSE  "GPL"


/*  Kernel Module Information   */
MODULE_VERSION(DRIVER_VERSION);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE(DRIVER_LICENSE);





