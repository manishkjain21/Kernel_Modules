#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x62c440a7, "module_layout" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xab70ad05, "del_timer_sync" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0xbf258353, "init_timer_key" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x7d11c268, "jiffies" },
	{ 0xa5882aae, "mod_timer" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0x27e1a049, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "888C5FAF17068FC4B444437");
