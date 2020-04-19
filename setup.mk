OPENCM3_DIR     = $(CORE_DIR)/libopencm3
FREERTOS_DIR     = $(CORE_DIR)/freertos

OBJS   += $(CORE_DIR)/core.o


CPPFLAGS += -I$(CORE_DIR)/
CPPFLAGS += -I$(OPENCM3_DIR)/include
LDFLAGS  += -L$(OPENCM3_DIR)/lib
LDFLAGS  += -static -nostartfiles  -Xlinker -Map=$(PROJECT).map
ifdef MINIMAL_LIBS
LDLIBS   += -Wl,--start-group -lgcc -Wl,--end-group
else
LDLIBS   += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group
endif
LDSCRIPT        = generated.$(DEVICE).ld
DEVICES_DATA    = $(OPENCM3_DIR)/ld/devices.data
LDLIBS          += -lopencm3_$(DEVICE_FAMILY)
LIBDEPS         += $(OPENCM3_DIR)/lib/libopencm3_$(DEVICE_FAMILY).a


ifdef WITH_FREERTOS
LDFLAGS   += -L$(FREERTOS_DIR)/
LDLIBS    += -lfreertos
LIBDEPS   += $(FREERTOS_DIR)/libfreertos.a
CFLAGS    += -DWITH_FREERTOS
endif


all: $(PROJECT).elf $(PROJECT).hex $(PROJECT).bin


include $(OPENCM3_DIR)/mk/gcc-config.mk
ifdef WITH_CPP
LD		:= $(PREFIX)-g++
LDFLAGS		+= --specs=nosys.specs
endif


.PHONY: clean all


flash: $(PROJECT).bin
	st-flash read backup.img 0x0803F800 0x800
	st-flash erase
	st-flash write backup.img 0x0803F800
	st-flash write $(PROJECT).bin $(FLASH_ADDR)
	rm backup.img

install: $(PROJECT).bin
	st-flash read backup.img 0x0803F800 0x800
	st-flash erase
	st-flash write backup.img 0x0803F800
	st-flash write $(PROJECT).bin $(FLASH_ADDR)
	rm backup.img


$(OPENCM3_DIR)/lib/libopencm3_stm32l4.a:
	cd $(OPENCM3_DIR); TARGETS="$(TARGETS)" make; cd -

include $(OPENCM3_DIR)/mk/genlink-rules.mk
include $(OPENCM3_DIR)/mk/gcc-rules.mk

ifdef WITH_FREERTOS
include $(FREERTOS_DIR)/freertos.mk
endif
