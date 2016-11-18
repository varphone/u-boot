#
# MYZR Technology Co.,Ltd
# http://www.myzr.com.cn
# Tang Bin <tangb@myzr.com.cn>
#

obj-$(CONFIG_TARGET_MYIMX6EK200)	:= myimx6qdls.o
obj-$(CONFIG_TARGET_MYIMX6EK314)	:= myimx6qdls.o
obj-$(CONFIG_TARGET_MYIMX6EKPOB)	:= myimx6qdls.o
obj-$(CONFIG_TARGET_MYIMX6ACO)		:= myimx6qdls.o
obj-$(CONFIG_TARGET_MYIMX6EK140)	:= myimx6ul.o

extra-$(CONFIG_USE_PLUGIN) :=  plugin.bin
$(obj)/plugin.bin: $(obj)/plugin.o
	$(OBJCOPY) -O binary --gap-fill 0xff $< $@
