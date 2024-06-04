#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define NUM_MINORS 1
#define DEVICE_NAME "fake_char_dev"
#define DEVICE_CLASS "dev_class"

typedef struct command {
    uint32_t Op;
    int32_t A;
    int32_t B;
    int32_t Result;
} command_t;

static struct class *dev_class = NULL;
static struct cdev dev_cdev;
static dev_t dev_num;
static command_t cmd;

static ssize_t my_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos) {
    if(count != sizeof(command_t)) {
        return -EINVAL;
    }
    if(copy_from_user(&cmd, buf, count)) { 
        return -EFAULT;
    }
    if(cmd.Op == 1) {
        cmd.Result = cmd.A + cmd.B;
    } else if(cmd.Op == 2) {
        cmd.Result = cmd.A - cmd.B;
    } else {
        return -EINVAL;
    }
    return sizeof(command_t); 
}

static ssize_t my_read(struct file *filp, char __user *buf, size_t count, loff_t *pos) {
    if(count < sizeof(command_t)) {
        return -EINVAL;
    }
    if(copy_to_user(buf, &cmd, count)) { 
        return -EFAULT;
    }
    return sizeof(command_t); 

}

static int my_open(struct inode *inode, struct file *file) {
    return 0;
}

static int my_release(struct inode *inode, struct file *file) {
    return 0;
}

static struct file_operations dev_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write, 
    .open = my_open,
    .release = my_release
}; 

static int __init my_init(void) {
    int ret;
    ret = alloc_chrdev_region(&dev_num, 0, NUM_MINORS, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ALERT "FakeCharDev failed to allocate major number\n");
        return ret;
    }
    dev_class = class_create(DEVICE_CLASS);
    if(IS_ERR(dev_class)) {
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "Failed to registed device class\n");
        return PTR_ERR(dev_class);
    }
    cdev_init(&dev_cdev, &dev_fops); 
    dev_cdev.owner = THIS_MODULE;
    dev_t curr_dev = MKDEV(MAJOR(dev_num), MINOR(dev_num));
    cdev_add(&dev_cdev, curr_dev, NUM_MINORS);
    device_create(dev_class, NULL, curr_dev, NULL, DEVICE_NAME"0");
    return 0;
}

static void __exit my_exit(void) {
    device_destroy(dev_class, MKDEV(MAJOR(dev_num), MINOR(dev_num))); 
    cdev_del(&dev_cdev);
    class_unregister(dev_class);
    class_destroy(dev_class);
    unregister_chrdev_region(dev_num, 1);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Atindra");
MODULE_DESCRIPTION("FakeCharDev Linux device driver");