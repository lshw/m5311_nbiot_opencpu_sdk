#created by xgl,2018/4/2

TOP_PATH = $(shell pwd)
include utils/config.mk
empty:=
space:=$(empty) 
CC := arm-none-eabi-gcc
CXX := arm-none-eabi-g++
CFLAGS := -mlittle-endian -mthumb -mcpu=cortex-m4 -fsingle-precision-constant -Wdouble-promotion -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -fno-builtin -gdwarf-2 -Os -fno-strict-aliasing -fno-common  -std=gnu99 -w
INC := -I'C:\Program Files (x86)\GNU Tools ARM Embedded\4.9 2015q3\lib\gcc\arm-none-eabi\4.9.3\include'  -I'inc\os' -I'inc\apb' -I'inc\lwip' -Isrc -I. -Iinc

SRC_DIRS := src src/http
INC +=  $(foreach dir,$(SRC_DIRS),-I$(dir)$(space) )
OBJS = $(foreach dir,$(SRC_DIRS),$(patsubst %.c,out/%.o,$(wildcard $(dir)/*.c)))
OBJS += $(foreach dir,$(SRC_DIRS),$(patsubst %.cpp,out/%.o,$(wildcard $(dir)/*.cpp)))
SRC-C-DEPS = $(foreach dir,$(SRC_DIRS),$(patsubst %.c,out/%.o.d,$(wildcard $(dir)/*.c)))
SRC-C-DEPS +=  $(foreach dir,$(SRC_DIRS),$(patsubst %.cpp,out/%.o.d,$(wildcard $(dir)/*.cpp)))
all: check $(OBJS)
	@echo "linking $(HWVER) libs"
	@arm-none-eabi-gcc -mlittle-endian -mthumb -mcpu=cortex-m4 -fsingle-precision-constant -Wdouble-promotion -mfpu=fpv4-sp-d16 -mfloat-abi=hard --specs=nano.specs -lnosys -nostartfiles -u _printf_float -u _scanf_float -Wl,-wrap=malloc -Wl,-wrap=calloc -Wl,-wrap=realloc -Wl,-wrap=free -Wl,-wrap=memcpy -Wl,-wrap=memset -Wl,-wrap=memmove -Wl,-wrap=memcmp -Wl,-wrap=strcpy -Wl,-wrap=strcmp -Wl,-wrap=strlen -Wl,-wrap=fprintf -Wl,-wrap=_printf_i -Wl,-T'lib/mt2625_flash_bring_up.ld' -Wl,--gc-sections -u md_commit_id_str -Wl,--whole-archive -Wl,--start-group lib/$(strip $(HWVER))obj.a lib/libhal_protected_CM4_GCC.a lib/$(strip $(HWVER))custom_lib_md.a lib/librsdl.a   lib/lib_md.a lib/libtool_authentication_CM4_GCC.a   -Wl,--end-group -Wl,--no-whole-archive $(OBJS) -lm   -o firmware\$(strip $(HWVER))\nbiot_m2m_demo.elf
	@arm-none-eabi-objcopy -O binary firmware\$(strip $(HWVER))\nbiot_m2m_demo.elf firmware\$(strip $(HWVER))\nbiot_m2m_demo.bin
check:
	@do_check.bat $(subst /,\,$(SRC_DIRS))
out/%.o:%.c 
	@echo 'compile $<...'''
	@$(CC) $(CFLAGS) $(INC)  -c $< -o $@ -MD -MF $(dir $@)$(notdir $@).d -MT $@
out/%.o:%.cpp 
	@echo 'compile $<...'''
	@$(CXX) $(CFLAGS) $(INC) -c $< -o $@ -MD -MF $(dir $@)$(notdir $@).d -MT $@	
clean:
	@clean.bat $(subst /,\,$(OBJS))
	@clean.bat $(subst /,\,$(SRC-C-DEPS))
	@echo clean ends
    
-include $(SRC-C-DEPS)