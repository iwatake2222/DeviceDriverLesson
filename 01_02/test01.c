#include <linux/module.h>

extern void test2_func(void);

static int test_init(void)
{
	printk("My module is installed\n");
	test2_func();
	return 0;
}

static void test_exit(void)
{
	printk("My module is removed\n");
}

module_init(test_init);
module_exit(test_exit);
