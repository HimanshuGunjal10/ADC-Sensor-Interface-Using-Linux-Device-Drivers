#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/irq.h>
//#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <asm/unistd.h>

#include <mach/map.h>
#include <mach/regs-clock.h>
#include <mach/regs-gpio.h>

#include <plat/gpio-cfg.h>
#include <mach/gpio-bank-e.h>
#include <mach/gpio-bank-k.h>

#define DEVICE_NAME "himanshu_gpio"

static long sbc2440_leds_ioctl(struct file *filp, unsigned int cmd)
{
	switch(cmd)
	{
		unsigned tmp;
	case 1:
		tmp = readl(S3C64XX_GPEDAT);
		tmp |= (1 << 3);
	   	writel(tmp, S3C64XX_GPEDAT);
		return 0;

	case 0:
		
		tmp = readl(S3C64XX_GPEDAT);
		tmp &= ~(1 << 3);
		writel(tmp, S3C64XX_GPEDAT);
		return 0;

	default:
		return -EINVAL;
	}
}

static struct file_operations dev_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl	= sbc2440_leds_ioctl,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

static int __init dev_init(void)
{
	int ret;

	{
		unsigned tmp;
		tmp = readl(S3C64XX_GPECON);
                tmp &= ~(0xF<<12); 
		tmp = (tmp |(1<<12));
		writel(tmp, S3C64XX_GPECON);
		
		tmp = readl(S3C64XX_GPEDAT);
		tmp &= ~(1 << 3);
		writel(tmp, S3C64XX_GPEDAT);
	}

	ret = misc_register(&misc);

	printk (DEVICE_NAME"\tinitialized\n");

	return ret;
}

static void __exit dev_exit(void)
{
	misc_deregister(&misc);
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("FriendlyARM Inc.");
