TARGET = Whetstone
C_SRCS += whetstone.c
CFLAGS += -O3 -fno-builtin-printf -fno-common -funroll-loops -finline-functions --param max-inline-insns-auto=20 -falign-functions=4 -falign-jumps=4 -falign-loops=4 
LDFLAGS += -lm

BSP_BASE = ../../bsp
include $(BSP_BASE)/env/common.mk
