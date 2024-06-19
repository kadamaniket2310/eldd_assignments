#include <linux/module.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/io.h>

#define KBD_DATA_REG 0x60
#define KBD_CONTROL_REG 0x64
#define DELAY do { mdelay(1); if (++delay > 10) break; } while(0)

struct timer_list mytimer;
int ticks;
int count = 0;

void mytimer_function(struct timer_list *ptimer)
{
	long delay =0;
	printk(KERN_INFO " %s : mytimer_function : count = %d\n", THIS_MODULE->name, count);
	count++;
	printk(KERN_INFO " %s : Reenabling keyboard starting...\n", THIS_MODULE->name);
	while(inb(KBD_CONTROL_REG) & 0x02)
		DELAY;
	outb(0xAE, KBD_CONTROL_REG);
	printk(KERN_INFO " %s : Renabling keyboard completed...\n", THIS_MODULE->name);
}

static __init int desd_init(void)
{
	long delay =0;
	int sec = 10;
	ticks = sec * HZ;		//2500

	timer_setup(&mytimer, mytimer_function, 0);
	mytimer.expires = jiffies + ticks;
	add_timer(&mytimer);
	printk(KERN_INFO " %s : Disabling keyboard starting...\n", THIS_MODULE->name);
	while(inb(KBD_CONTROL_REG) & 0x02)
		DELAY;
	outb(0xAD, KBD_CONTROL_REG);
	printk(KERN_INFO " %s : Disabling keyboard completed...\n", THIS_MODULE->name);
	

	printk(KERN_INFO " %s : Timer initialisation is done successfully\n", THIS_MODULE->name);
	return 0;
}

static __exit void desd_exit(void)
{
	del_timer_sync(&mytimer);
	printk(KERN_INFO " %s : Timer deinitialisation is done successfully\n", THIS_MODULE->name);
}

module_init(desd_init);
module_exit(desd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aniket");
MODULE_DESCRIPTION("This is keyboard disabling demo");
