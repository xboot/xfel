#
# Top makefile
#

CROSS		?= ~/9tripod/f102_melis_v1.1/lichee/brandy-2.0/tools/toolchain/Xuantie-900-gcc-linux-6.6.0-glibc-x86_64-V2.10.0-rc5-20240319/bin/riscv64-unknown-linux-gnu-
NAME		:= spi

#
# System environment variable.
#
ifeq ($(OS), Windows_NT)
	HOSTOS		:= windows
else
	ifneq (,$(findstring Linux, $(shell uname -a)))
		HOSTOS	:= linux
	endif
endif

#
# Load default variables.
#
ASFLAGS		:= -g -ggdb -Wall -O3 -ffreestanding
CFLAGS		:= -g -ggdb -Wall -O3 -ffreestanding
CXXFLAGS	:= -g -ggdb -Wall -O3 -ffreestanding
LDFLAGS		:= -T link.ld -nostdlib
ARFLAGS		:= -rcs
OCFLAGS		:= -v -O binary
ODFLAGS		:=
MCFLAGS		:= -march=rv32imafdcv_zicbom_zicbop_zicboz_zicond_zihintntl_zihintpause_zawrs_zfa_zfbfmin_zfh_zba_zbb_zbc_zbs_zvfbfmin_zvfbfwma_svinval_svnapot_svpbmt_xtheadc_xtheadvdot -mabi=ilp32 -mtune=c907 -mcmodel=medany -fno-stack-protector

LIBDIRS		:=
LIBS 		:=
INCDIRS		:=
SRCDIRS		:=

#
# Add external library
#
INCDIRS		+= include \
			   include/external
SRCDIRS		+= source \
			   source/external

#
# You shouldn't need to change anything below this point.
#
AS			:= $(CROSS)gcc -x assembler-with-cpp
CC			:= $(CROSS)gcc
CXX			:= $(CROSS)g++
LD			:= $(CROSS)ld
AR			:= $(CROSS)ar
OC			:= $(CROSS)objcopy
OD			:= $(CROSS)objdump
MKDIR		:= mkdir -p
CP			:= cp -af
RM			:= rm -fr
CD			:= cd
FIND		:= find

#
# X variables
#
X_ASFLAGS	:= $(MCFLAGS) $(ASFLAGS)
X_CFLAGS	:= $(MCFLAGS) $(CFLAGS)
X_CXXFLAGS	:= $(MCFLAGS) $(CXXFLAGS)
X_LDFLAGS	:= $(LDFLAGS)
X_OCFLAGS	:= $(OCFLAGS)
X_LIBDIRS	:= $(LIBDIRS)
X_LIBS		:= $(LIBS) -lgcc

X_OUT		:= output
X_NAME		:= $(patsubst %, $(X_OUT)/%, $(NAME))
X_INCDIRS	:= $(patsubst %, -I %, $(INCDIRS))
X_SRCDIRS	:= $(patsubst %, %, $(SRCDIRS))
X_OBJDIRS	:= $(patsubst %, .obj/%, $(X_SRCDIRS))

X_SFILES	:= $(foreach dir, $(X_SRCDIRS), $(wildcard $(dir)/*.S))
X_CFILES	:= $(foreach dir, $(X_SRCDIRS), $(wildcard $(dir)/*.c))
X_CPPFILES	:= $(foreach dir, $(X_SRCDIRS), $(wildcard $(dir)/*.cpp))

X_SDEPS		:= $(patsubst %, .obj/%, $(X_SFILES:.S=.o.d))
X_CDEPS		:= $(patsubst %, .obj/%, $(X_CFILES:.c=.o.d))
X_CPPDEPS	:= $(patsubst %, .obj/%, $(X_CPPFILES:.cpp=.o.d))
X_DEPS		:= $(X_SDEPS) $(X_CDEPS) $(X_CPPDEPS)

X_SOBJS		:= $(patsubst %, .obj/%, $(X_SFILES:.S=.o))
X_COBJS		:= $(patsubst %, .obj/%, $(X_CFILES:.c=.o))
X_CPPOBJS	:= $(patsubst %, .obj/%, $(X_CPPFILES:.cpp=.o)) 
X_OBJS		:= $(X_SOBJS) $(X_COBJS) $(X_CPPOBJS)

VPATH		:= $(X_OBJDIRS)

.PHONY:	all clean
all : $(X_NAME)

$(X_NAME) : $(X_OBJS)
	@echo [LD] Linking $@.elf
	@$(CC) $(X_LDFLAGS) $(X_LIBDIRS) -Wl,--cref,-Map=$@.map $^ -o $@.elf $(X_LIBS)
	@echo [OC] Objcopying $@.bin
	@$(OC) $(X_OCFLAGS) $@.elf $@.bin

$(X_SOBJS) : .obj/%.o : %.S
	@echo [AS] $<
	@$(AS) $(X_ASFLAGS) -MD -MP -MF $@.d $(X_INCDIRS) -c $< -o $@

$(X_COBJS) : .obj/%.o : %.c
	@echo [CC] $<
	@$(CC) $(X_CFLAGS) -MD -MP -MF $@.d $(X_INCDIRS) -c $< -o $@

$(X_CPPOBJS) : .obj/%.o : %.cpp
	@echo [CXX] $<
	@$(CXX) $(X_CXXFLAGS) -MD -MP -MF $@.d $(X_INCDIRS) -c $< -o $@

clean:
	@$(RM) .obj $(X_OUT)

#
# Include the dependency files, should be place the last of makefile
#
sinclude $(shell $(MKDIR) $(X_OBJDIRS) $(X_OUT)) $(X_DEPS)
