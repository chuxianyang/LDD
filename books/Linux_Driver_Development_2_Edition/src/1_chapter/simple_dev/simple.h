#ifndef __SIMPLE_H_INCLUDE
#define __SIMPLE_H_INCLUDE

#include <linux/ioctl.h>

/********************************************************
 * Macros to help debugging
 ********************************************************/
#define PDEBUG(fmt, args...) printk(KERN_DEBUG, "DEMO: " fmt, ##args)

#define SIMPLE_MAJOR 224
#define SIMPLE_MINOR 0

struct simple_dev {
	struct cdev cdev;
};


ssize_t simple_read(struct file *filp, char __user *buf, size_t count,
                   loff_t *f_pos);
ssize_t simple_write(struct file *filp, const char __user *buf, size_t count,
                    loff_t *f_pos);
/*
loff_t  simple_llseek(struct file *filp, loff_t off, int whence);
int     simple_ioctl(struct inode *inode, struct file *filp,
                    unsigned int cmd, unsigned long arg);
*/
#endif



