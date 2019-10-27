OPENCM3_DIR     = $(CORE_DIR)/libopencm3
FREERTOS_DIR    = $(CORE_DIR)/freertos

include $(CORE_DIR)/boards/$(BOARD)/config.mk

OBJS   += $(CORE_DIR)/boards/$(BOARD)/init.o
OBJS   += $(CORE_DIR)/boards/$(BOARD)/tpuart.o
OBJS   += $(CORE_DIR)/ring_buffer.o
OBJS   += $(CORE_DIR)/core.o


CFLAGS   += -I$(CORE_DIR)/
CPPFLAGS += -I$(CORE_DIR)/
CPPFLAGS += -I$(CORE_DIR)/boards/$(BOARD)
CPPFLAGS += -I$(OPENCM3_DIR)/include
LDFLAGS  += -L$(OPENCM3_DIR)/lib
LDFLAGS  += -static -nostartfiles  -Xlinker -Map=output.map
LDLIBS   += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group
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


all: binary.elf binary.hex binary.bin


include $(OPENCM3_DIR)/mk/gcc-config.mk


.PHONY: clean all


flash: binary.bin
	st-flash write binary.bin $(FLASH_ADDR)

install: binary.bin
	st-flash write binary.bin $(FLASH_ADDR)


$(OPENCM3_DIR)/lib/libopencm3_stm32l4.a:
	cd $(OPENCM3_DIR); TARGETS="$(TARGETS)" make; cd -

include $(OPENCM3_DIR)/mk/genlink-rules.mk
include $(OPENCM3_DIR)/mk/gcc-rules.mk

ifdef WITH_FREERTOS
include $(FREERTOS_DIR)/freertos.mk
endif
