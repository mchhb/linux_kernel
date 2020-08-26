#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/of_gpio.h>
#include <linux/semaphore.h>

#define KEY_CNT		1
#define KEY_NAME	"key"

#define KEY0VALUE	0xf0
#define INVAKEY		0X00

/*   gpio设备结构体		*/ 
struct key_dev{
	dev_t devid;
	struct cdev cdev;
	int major;
	int minor;
	struct class *class;
	struct device *device;
	struct device_node *nd;
	int key_gpio;
	atomic_t keyvalue;
};


struct key_dev key;

static int keyio_init(void)
{
	int ret = 0;
	/*设置KEY所使用的GPIO*/
	/*1、获取设备节点：key*/
	
	key.nd = of_find_node_by_path("/key");
	if(key.nd == NULL)
	{
		printk("key node not find!\r\n");
		return -EFAULT;
	}
	else
	{
		printk("key node find!\r\n");
	}
	
	/*2、获取设备树中的gpio属性，得到KEY所使用的KEY编号*/
	
	key.key_gpio = of_get_named_gpio(key.nd,"key", 0);
	if(key.key_gpio < 0)
	{
		printk("can't get key\r\n");
		return -EFAULT;
	}
	printk("key-gpio num = %d \r\n",key.key_gpio);
	
	gpio_request(key.key_gpio,"key0");

	ret = gpio_direction_input(key.key_gpio);
	if(ret < 0)
	{
		printk("can't set gpio!\r\n");
		return -EFAULT;
	}	

}


static int key_open(struct inode *inode, struct file *filp)
{
	int ret = 0;

	filp->private_data = &key;/*设置私有数据*/
	
	ret = keyio_init();

	if(ret < 0)
	{
		return ret;
	}

	return 0;
}

static ssize_t key_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
	int ret = 0;

	unsigned char value;

	struct key_dev *dev = filp->private_data;

	if(gpio_get_value(dev->key_gpio) == 0)
	{
		while(!gpio_get_value(dev->key_gpio));
		atomic_set(&dev->keyvalue,KEY0VALUE);
	}
	else
	{
		atomic_set(&dev->keyvalue,INVAKEY);
	}

	value = atomic_read(&dev->keyvalue);

	ret = copy_to_user(buf,&value,sizeof(value));

	return 0;
}

static ssize_t key_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
	return 0;
}

static int key_release(struct inode *inode, struct file *filp)
{
	return 0;
}

/* 设备操作函数 */
static struct file_operations key_fops = {
	.owner = THIS_MODULE,
	.open = key_open,
	.read = key_read,
	.write = key_write,
	.release = key_release,
};
/*	驱动函数入口	*/

static int __init key_gpio_init(void)
{
	//int ret = 0;

	atomic_set(&key.keyvalue, INVAKEY);

	/* 1、注册字符设备驱动	*/
	key.major = 0;
	if(key.major)
	{
		/*	给定主设备号	*/
		key.devid = MKDEV(key.major, 0);
		register_chrdev_region(key.devid, KEY_CNT,KEY_NAME);
	}
	else
	{
		alloc_chrdev_region(&key.devid, 0, KEY_CNT, KEY_NAME);/* 申请设备号 */
		key.major = MAJOR(key.major);
		key.minor = MINOR(key.minor);
	}
	printk("gpio key major = %d,minor = %d\r\n",key.major,key.minor);

	/* 2、初始化cdev*/
	key.cdev.owner = THIS_MODULE;
	cdev_init(&key.cdev, &key_fops);

	/*3、添加一个cdev*/
	cdev_add(&key.cdev, key.devid, KEY_CNT);

	/*4、创建类*/
	key.class = class_create(THIS_MODULE, KEY_NAME);
	if(IS_ERR(key.class))
	{
		return PTR_ERR(key.class);
	}

	/*5、创建设备*/
	key.device = device_create(key.class, NULL, key.devid, NULL, KEY_NAME);
	if(IS_ERR(key.device))
	{
		return PTR_ERR(key.device);
	}

	return 0;
}

/*	驱动函数出口	*/

static void __exit key_gpio_exit(void)
{
	/*注销字符设备驱动*/
	cdev_del(&key.cdev);/*删除cdev*/
	unregister_chrdev_region(key.devid, KEY_CNT);/*注销*/

	device_destroy(key.class, key.devid);
	class_destroy(key.class);
}


module_init(key_gpio_init);
module_exit(key_gpio_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("HHB");