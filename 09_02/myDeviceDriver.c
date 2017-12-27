#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>

/*** このデバイスに関する情報 ***/
MODULE_LICENSE("Dual BSD/GPL");
#define DRIVER_NAME "MyDevice"				/* /proc/devices等で表示されるデバイス名 */

#define GPIO_PIN_LED 4
#define GPIO_PIN_BTN 17

static irqreturn_t mydevice_gpio_intr(int irq, void *dev_id)
{
	printk("mydevice_gpio_intr\n");

	int btn;
	btn = gpio_get_value(GPIO_PIN_BTN);
	printk("button = %d\n", btn);
	return IRQ_HANDLED;
}

/* ロード(insmod)時に呼ばれる関数 */
static int mydevice_init(void)
{
	printk("mydevice_init\n");

	/* LED用のGPIO4を出力にする。初期値は1(High) */
	gpio_direction_output(GPIO_PIN_LED, 1);
	/* LED用のGPIO4に0(Low)を出力にする */
	gpio_set_value(GPIO_PIN_LED, 0);

	/* ボタン用のGPIO17を入力にする */
	gpio_direction_input(GPIO_PIN_BTN);

	/* ボタン用のGPIO17の割り込み番号を取得する */
	int irq = gpio_to_irq(GPIO_PIN_BTN);
	printk("gpio_to_irq = %d\n", irq);

	/* ボタン用のGPIO17の割り込みハンドラを登録する */
	if (request_irq(irq, (void*)mydevice_gpio_intr, IRQF_SHARED | IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "mydevice_gpio_intr", (void*)mydevice_gpio_intr) < 0) {
		printk(KERN_ERR "request_irq\n");
		return -1;
	}

	return 0;
}

/* アンロード(rmmod)時に呼ばれる関数 */
static void mydevice_exit(void)
{
	printk("mydevice_exit\n");
	int irq = gpio_to_irq(GPIO_PIN_BTN);
	free_irq(irq, (void*)mydevice_gpio_intr);
}

module_init(mydevice_init);
module_exit(mydevice_exit);
