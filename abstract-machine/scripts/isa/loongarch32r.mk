CROSS_COMPILE := $(LA32R)/bin/loongarch32r-linux-gnusf-
COMMON_FLAGS  := -fno-pic -Wno-attributes
CFLAGS        += $(COMMON_FLAGS) -static 
ASFLAGS       += $(COMMON_FLAGS) -O0
