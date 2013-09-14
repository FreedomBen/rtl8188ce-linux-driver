obj-$(CONFIG_RTLWIFI) 		+= rtlwifi.o
rtlwifi-objs	:=		\
		base.o		\
		cam.o		\
		core.o		\
		stats.o		\
		debug.o		\
		efuse.o		\
		ps.o		\
		rc.o		\
		regd.o

ifneq ($(CONFIG_PCI),)
rtlwifi-objs	+= pci.o
endif

obj-$(CONFIG_RTL8188EE)		+= rtl8188ee/
obj-$(CONFIG_RTL8192CE)		+= rtl8192ce/
obj-$(CONFIG_RTL8192DE)         += rtl8192de/
obj-$(CONFIG_RTL8192SE)         += rtl8192se/
obj-$(CONFIG_RTL8723E)         += rtl8723e/

ccflags-y += -D__CHECK_ENDIAN__ 
