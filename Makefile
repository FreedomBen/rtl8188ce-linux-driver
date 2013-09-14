
CC = gcc
KVER  := $(shell uname -r)
KSRC := /lib/modules/$(KVER)/build
MODDESTDIR := /lib/modules/$(KVER)/kernel/drivers/net/wireless/rtlwifi
FIRMWAREDIR := /lib/firmware/
PWD := $(shell pwd)
CLR_MODULE_FILES := *.mod.c *.mod *.o .*.cmd *.ko *~ .tmp_versions* modules.order Module.symvers
SYMBOL_FILE := Module.symvers

EXTRA_CFLAGS += -O2
obj-m := rtlwifi.o
PCI_MAIN_OBJS	:= base.o	\
		rc.o	\
		debug.o	\
		regd.o	\
		efuse.o	\
		cam.o	\
		ps.o	\
		core.o	\
		stats.o	\
		pci.o	\

rtlwifi-objs += $(PCI_MAIN_OBJS)

all: 
	$(MAKE) -C $(KSRC) M=$(PWD) modules
	@cp $(SYMBOL_FILE) rtl8192ce/
	@make -C rtl8192ce/
	@cp $(SYMBOL_FILE) rtl8192se/
	@make -C rtl8192se/
	@cp $(SYMBOL_FILE) rtl8192de/
	@make -C rtl8192de/
	@cp $(SYMBOL_FILE) rtl8723e/
	@make -C rtl8723e/
	@cp $(SYMBOL_FILE) rtl8188ee/
	@make -C rtl8188ee/
install: all
	find /lib/modules/$(shell uname -r) -name "r8192se_*.ko" -exec rm {} \;
	find /lib/modules/$(shell uname -r) -name "r8192ce_*.ko" -exec rm {} \;
	find /lib/modules/$(shell uname -r) -name "r8723e_*.ko" -exec rm {} \;
	find /lib/modules/$(shell uname -r) -name "r8188ee_*.ko" -exec rm {} \;
	@rm -fr $(FIRMWAREDIR)/`uname -r`/rtlwifi

	$(shell rm -fr $(MODDESTDIR))
	$(shell mkdir $(MODDESTDIR))
	$(shell mkdir $(MODDESTDIR)/rtl8192se)
	$(shell mkdir $(MODDESTDIR)/rtl8192ce)
	$(shell mkdir $(MODDESTDIR)/rtl8192de)
	$(shell mkdir $(MODDESTDIR)/rtl8723e)
	$(shell mkdir $(MODDESTDIR)/rtl8188ee)
	@install -p -m 644 rtlwifi.ko $(MODDESTDIR)	
	@install -p -m 644 ./rtl8192se/rtl8192se.ko $(MODDESTDIR)/rtl8192se
	@install -p -m 644 ./rtl8192ce/rtl8192ce.ko $(MODDESTDIR)/rtl8192ce
	@install -p -m 644 ./rtl8192de/rtl8192de.ko $(MODDESTDIR)/rtl8192de
	@install -p -m 644 ./rtl8723e/rtl8723e.ko $(MODDESTDIR)/rtl8723e
	@install -p -m 644 ./rtl8188ee/rtl8188ee.ko $(MODDESTDIR)/rtl8188ee
	
	@depmod -a

	@#copy firmware img to target fold
	@#$(shell [ -d "$(FIRMWAREDIR)/`uname -r`" ] && cp -fr firmware/rtlwifi/ $(FIRMWAREDIR)/`uname -r`/.)
	@#$(shell [ ! -d "$(FIRMWAREDIR)/`uname -r`" ] && cp -fr firmware/rtlwifi/ $(FIRMWAREDIR)/.)
	@cp -fr firmware/rtlwifi/ $(FIRMWAREDIR)/

uninstall:
	$(shell [ -d "$(MODDESTDIR)" ] && rm -fr $(MODDESTDIR))
	
	@depmod -a
	
	@#delete the firmware img
	@rm -fr /lib/firmware/rtlwifi/
	@rm -fr /lib/firmware/`uname -r`/rtlwifi/

clean:
	rm -fr *.mod.c *.mod *.o .*.cmd *.ko *~
	rm -fr .tmp_versions
	rm -fr Modules.symvers
	rm -fr Module.symvers
	rm -fr Module.markers
	rm -fr modules.order
	rm -fr tags
	@find -name "tags" -exec rm {} \;
	@rm -fr $(CLR_MODULE_FILES)
	@make -C rtl8192ce/ clean
	@make -C rtl8192se/ clean
	@make -C rtl8192de/ clean
	@make -C rtl8723e/ clean
	@make -C rtl8188ee/ clean
