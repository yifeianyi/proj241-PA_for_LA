CROSS_COMPILE := $(LA32R)/bin/loongarch32r-linux-gnusf-
COMMON_FLAGS  := -fno-pic
LA32R_INC     := -I/home/ysyx/Desktop/la32r-toolchains/install/lib/gcc/loongarch32r-linux-gnusf/8.3.0/include
CFLAGS        += $(COMMON_FLAGS) -static 
ASFLAGS       += $(COMMON_FLAGS) -O0
