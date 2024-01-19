#+---------------------------------------------------------------------------
#
#  Copyright (c) 2024 Anton Gusev
#
#  File:       makefile
#
#  Contents:   makefile to build arm Cortex-M4 software with gcc
#
#----------------------------------------------------------------------------

# program name
	TARGET	= stm32f405-sdio-fatfs

# toolchain
	TOOL	= arm-none-eabi-

# program version
    VER_MAJOR   = 0
    VER_MINOR   = 1

# compile options
	OPTIMIZE        = -Og
	USE_LTO         = NO

# Select chip: 
	CHIP		= STM32F40_41xxx
	MCU			= cortex-m4
	FPU			= -mfpu=fpv4-sp-d16 -mfloat-abi=hard
	HSE_VALUE	= 25000000

# compiler defines
	DEFS		= -D$(CHIP)
	DEFS        += -DMICRO_FORMAT_DOUBLE
	DEFS		+= -DVER_MAJOR=$(VER_MAJOR)
	DEFS		+= -DVER_MINOR=$(VER_MINOR)
	DEFS		+= -DHSE_VALUE=$(HSE_VALUE)

    DEFS        += -DSTM32F4XX


	TARGET		:= $(TARGET)-$(VER_MAJOR)-$(VER_MINOR)

###########################################################
#  common part for all my cortex-m3/m4 projects
###########################################################

	BASE		= .
	CC			= $(TOOL)gcc
	CXX			= $(TOOL)g++
	LD			= $(TOOL)g++
	AS			= $(CC) -x assembler-with-cpp
	OBJCOPY		= $(TOOL)objcopy
	OBJDUMP		= $(TOOL)objdump
	SIZE		= $(TOOL)size -d
	FLASHER		= openocd
	RM			= rm -f
	CP			= cp
	MD			= mkdir


# dirs
	SRCDIR		= $(BASE)/src
	OBJDIR		= $(BASE)/obj
	EXEDIR		= $(BASE)/exe
	LSTDIR		= $(BASE)/lst
	PRJDIR		= $(BASE)/prj
	LD_SCRIPTS	= $(PRJDIR)

# files
	HEX		= $(EXEDIR)/$(TARGET).hex
	BIN		= $(EXEDIR)/$(TARGET).bin
	ELF		= $(EXEDIR)/$(TARGET).elf
	MAP		= $(LSTDIR)/$(TARGET).map
	LSS		= $(LSTDIR)/$(TARGET).lss
	OK		= $(EXEDIR)/$(TARGET).ok

# linker script (chip dependent)
	LD_SCRIPT	= $(CHIP).ld

# libs dir
	LIBSDIR     = ../lib

# scmRTOS dir
	SCMDIR      = $(LIBSDIR)/scmRTOS

# stm32tpl lib dir
	STM32TPL    = $(LIBSDIR)/stm32tpl

# source directories (all *.c, *.cpp and *.S files from this dirs are included in build)
	DIRS	:= $(SRCDIR)
	DIRS	+= $(SRCDIR)/base
	DIRS	+= $(SRCDIR)/startup
	DIRS	+= $(SRCDIR)/startup/$(CHIP)

	DIRS	+= $(STM32TPL)
	DIRS	+= $(STM32TPL)/CMSIS
	
	DIRS	+= $(SCMDIR)/core
	DIRS	+= $(SCMDIR)/port/cortex/mx-gcc
	DIRS	+= $(SCMDIR)/ext/profiler

	DIRS	+= $(LIBSDIR)/micro_format

	DIRS	+= $(LIBSDIR)/fatfs

# includes
	INCS	:= $(patsubst %, -I "%", $(DIRS))

# individual source files
	SRCS	:= 

#calc obj files list
	OBJS	:= $(SRCS)
	OBJS	+= $(wildcard $(addsuffix /*.cpp, $(DIRS)))
	OBJS	+= $(wildcard $(addsuffix /*.c, $(DIRS)))
	OBJS	+= $(wildcard $(addsuffix /*.S, $(DIRS)))
	OBJS	:= $(notdir $(OBJS))
	OBJS	:= $(OBJS:.cpp=.o)
	OBJS	:= $(OBJS:.c=.o)
	OBJS	:= $(OBJS:.S=.o)
	OBJS	:= $(patsubst %, $(OBJDIR)/%, $(OBJS))

# flags
	FLAGS	= -mcpu=$(MCU) -mthumb
	FLAGS	+= $(FPU)
	FLAGS	+= $(INCS)
	FLAGS	+= $(DEFS)
	FLAGS	+= -Wa,-adhlns=$(addprefix $(LSTDIR)/, $(notdir $(addsuffix .lst, $(basename $<))))

	AFLAGS	= $(FLAGS)

	CFLAGS	= $(FLAGS)
	CFLAGS	+= $(OPTIMIZE)
ifneq (,$(filter-out discovery defines,$(MAKECMDGOALS)))
	CFLAGS	+= -MD 
endif
	CFLAGS	+= -std=gnu99
	CFLAGS	+= -g
	CFLAGS	+= -pipe
	CFLAGS	+= -ffunction-sections -fdata-sections
	CFLAGS	+= -Wall -Wextra
	CFLAGS	+= -Wimplicit -Wcast-align -Wpointer-arith -Wredundant-decls
	CFLAGS	+= -Wshadow -Wcast-qual -Wnested-externs -pedantic

	CXXFLAGS	= $(FLAGS)
	CXXFLAGS	+= $(OPTIMIZE)
ifneq (,$(filter-out discovery defines,$(MAKECMDGOALS)))
	CXXFLAGS	+= -MD 
endif
	CXXFLAGS	+= -std=c++17
	CXXFLAGS	+= -g
	CXXFLAGS	+= -pipe
	CXXFLAGS	+= -fno-exceptions -fno-rtti
	CXXFLAGS	+= -ffunction-sections -fdata-sections
	CXXFLAGS	+= -funsigned-bitfields -fshort-enums
	CXXFLAGS	+= -Wall -Wextra
	CXXFLAGS	+= -Winline
	CXXFLAGS	+= -Wpointer-arith -Wredundant-decls
	CXXFLAGS	+= -Wshadow -Wcast-qual -Wcast-align -pedantic
	CXXFLAGS	+= -Wfloat-equal -Wlogical-op -Wduplicated-cond
#	CXXFLAGS	+= -Wshift-overflow=2
#	CXXFLAGS	+= -Wconversion

#	CXXFLAGS	+= -fsanitize=address
#	CXXFLAGS	+= -fsanitize=undefined

	LD_FLAGS	= -mcpu=$(MCU)
	LD_FLAGS	+= -mthumb
	LD_FLAGS	+= $(FPU)
	LD_FLAGS	+= -nostartfiles 
	LD_FLAGS	+= --specs=nano.specs
#	LD_FLAGS	+= -u _printf_float  
	LD_FLAGS	+= -Wl,-Map="$(MAP)",--cref
	LD_FLAGS	+= -Wl,--gc-sections
	LD_FLAGS	+= -L$(LD_SCRIPTS)
	LD_FLAGS	+= -T$(LD_SCRIPT)

ifeq ($(USE_LTO),YES)
	CFLAGS		+= -flto
	CXXFLAGS	+= -flto
	LD_FLAGS	+= -flto $(OPTIMIZE)
endif

#openocd command-line

# debug level (d0..d3)
	OPENOCD_PARAMS		= -d0

# interface and board/target settings (using the OOCD target-library here)
	OPENOCD_PARAMS		+= -f $(PRJDIR)/openocd.cfg 
	OPENOCD_PARAMS		+= -c init
	OPENOCD_PARAMS		+= -c "sleep 200"

# commands to flash-write
	OPENOCD_PARAMS_LOAD	= $(OPENOCD_PARAMS)
	OPENOCD_PARAMS_LOAD	+= -c "reset halt"
	OPENOCD_PARAMS_LOAD	+= -c "wait_halt"
	OPENOCD_PARAMS_LOAD	+= -c "sleep 200"
	OPENOCD_PARAMS_LOAD	+= -c "flash write_image erase $(ELF)"
	OPENOCD_PARAMS_LOAD	+= -c "verify_image $(ELF)"
	OPENOCD_PARAMS_LOAD	+= -c "reset run"
	OPENOCD_PARAMS_LOAD	+= -c shutdown

# commands to just reset target
	OPENOCD_PARAMS_RESET	= $(OPENOCD_PARAMS)
	OPENOCD_PARAMS_RESET	+= -c "reset run"
	OPENOCD_PARAMS_RESET	+= -c shutdown


.SILENT :

.PHONY: all dirs build clean program protect reset archive docs discovery defines check
MAKEFLAGS += --no-print-directory

############# targets

all : 
	@echo --- building $(TARGET). defines: $(DEFS)
	$(MAKE) dirs
	$(MAKE) $(ELF)
	$(MAKE) $(HEX)
	$(MAKE) $(BIN)
	$(MAKE) $(LSS)
	$(SIZE) $(ELF)
	@echo "Errors: none"

build:
	$(MAKE) clean
	$(MAKE) all

$(LSS): $(ELF) makefile
	@echo --- making asm-lst...
#	@$(OBJDUMP) -dStC $(ELF) > $(LSS)
	$(OBJDUMP) -dC $(ELF) > $(LSS)

$(ELF):	$(OBJS) $(LIBS) makefile
	@echo --- linking...
	$(LD) $(OBJS) $(LD_FLAGS) -o "$(ELF)"

$(HEX): $(ELF) makefile
	@echo --- make hex...
	$(OBJCOPY) -O ihex $(ELF) $(HEX)

$(BIN): $(ELF) makefile
	@echo --- make bin...
	$(OBJCOPY) -O binary $(ELF) $(BIN)

program: $(HEX)
	@echo "Programming with $(FLASHER)"
	$(FLASHER) $(OPENOCD_PARAMS_LOAD)

reset:
	@echo Resetting device
	$(FLASHER) $(OPENOCD_PARAMS_RESET)

VPATH := $(DIRS)

$(OBJDIR)/%.o: %.cpp makefile
	@echo --- compiling $<...
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(OBJDIR)/%.o: %.c makefile
	@echo --- compiling $<...
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/%.o: %.S makefile
	@echo --- assembling $<...
	$(AS) -c $(AFLAGS) -o $@ $<

dirs: $(OBJDIR) $(EXEDIR) $(LSTDIR)

$(OBJDIR):
	-@$(MD) $(OBJDIR)

$(EXEDIR):
	-@$(MD) $(EXEDIR)

$(LSTDIR):
	-@$(MD) $(LSTDIR)

clean:
	-@$(RM) $(OBJDIR)/*.d 2>/dev/null
	-@$(RM) $(OBJDIR)/*.o 2>/dev/null
	-@$(RM) $(LSTDIR)/*.lst 2>/dev/null
	-@$(RM) $(ELF)
	-@$(RM) $(BIN)
	-@$(RM) $(HEX)
	-@$(RM) $(LSS)
	-@$(RM) $(MAP)

#discovery target for Eclipse parser
discovery:
ifeq ($(extension),c)
	@echo -- discovery for $(CC)
	$(CC) $(INCS) $(CFLAGS) -E -P -v -dD '$(specs_file)'
else
	@echo -- discovery for $(CCX)
	$(CXX) $(INCS) $(CXXFLAGS) -E -P -v -dD '$(specs_file)'
endif
	$(RM) spec.d

defines:
	$(CC) $(CFLAGS) -dM -E - < /dev/null | sort

#do not include dependency info on some targets:
ifneq (,$(filter-out docs build clean dirs discovery,$(MAKECMDGOALS)))
  -include $(wildcard $(OBJDIR)/*.d) 
endif
