#include <linux/module.h>

static int test_init(void)
{
	printk("Hello my module\n");
	return 0;
}

static void test_exit(void)
{
	printk("Bye bye my module\n");
}

module_init(test_init);
module_exit(test_exit);