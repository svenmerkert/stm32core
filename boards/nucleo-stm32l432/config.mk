TARGETS = stm32/l4
DEVICE_FAMILY = stm32l4
DEVICE  = stm32l432kc
FREERTOS_PORT = ARM_CM4F
CPPFLAGS += -MD -DSTM32L4 -DSTM32L432KC
ARCH_FLAGS := -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
FLASH_ADDR = 0x8000000

OBJS   += $(CORE_DIR)/boards/$(BOARD)/console.o
OBJS   += $(CORE_DIR)/boards/$(BOARD)/board_config.o
