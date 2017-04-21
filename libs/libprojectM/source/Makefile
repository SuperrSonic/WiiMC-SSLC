#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

ifeq ($(PLATFORM),wii)
include $(DEVKITPPC)/wii_rules
endif

ifeq ($(PLATFORM),cube)
include $(DEVKITPPC)/gamecube_rules
endif

#---------------------------------------------------------------------------------
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
BUILD		?=	wii_release
SOURCES		:=	. GL MilkdropPresetFactory NativePresetFactory Renderer Renderer/SOIL
INCLUDES	:=	. ../include ../include/projectM GL MilkdropPresetFactory NativePresetFactory Renderer Renderer/SOIL
LIBDIR		:=	../lib

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------

PROJMFLAGS	=	-DUSE_THREADS -DSTBI_NO_DDS -DDISABLE_NATIVE_PRESETS 

ifeq ($(PLATFORM),wii)
CFLAGS		=	-ffast-math -O3 -pipe -mrvl -mcpu=750 -meabi -mhard-float -Wall $(MACHDEP) $(INCLUDE) -DGEKKO $(PROJMFLAGS)
CXXFLAGS	=	$(CFLAGS)
#ASFLAGS		:=	-g
endif

ifeq ($(PLATFORM),cube)
CFLAGS		=	-g -O2 -Wall $(MACHDEP) $(INCLUDE) -DGEKKO $(PROJMFLAGS)
CXXFLAGS	=	$(CFLAGS)
ASFLAGS		:=	-g
endif


ifneq ($(BUILD),debug)
export PROJMBIN	:=	$(LIBDIR)/$(PLATFORM)/libprojectM.a
else
export PROJMBIN	:=	$(LIBDIR)/$(PLATFORM)/libprojectMd.a
CFLAGS		:=	-DDEBUG
endif

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS		:=

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS		:=

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir))

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))


export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
			$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD) \
			-I$(LIBOGC_INC)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
			-L$(LIBOGC_LIB)

.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr wii_debug wii_release cube_debug cube_release $(LIBDIR)

all: $(PROJMBIN)

install:
	@[ -d $(DEVKITPRO)/libogc/include/projectM ] || mkdir -p $(DEVKITPRO)/libogc/include/projectM
	cp ../include/projectM/*.hpp $(DEVKITPRO)/libogc/include/projectM
	cp ../include/*.hpp $(DEVKITPRO)/libogc/include
	cp ../lib/wii/libprojectM.a $(DEVKITPRO)/libogc/lib/wii
	cp ../lib/cube/libprojectM.a $(DEVKITPRO)/libogc/lib/cube

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(PROJMBIN): $(OFILES) $(LIBDIR)/$(PLATFORM)
	@rm -f "../$(PROJMBIN)"
	@$(AR) rcs "../$(PROJMBIN)" $(OFILES)
	@echo built ... $(notdir $@)

$(LIBDIR)/$(PLATFORM):
	mkdir -p ../$(LIBDIR)/$(PLATFORM)

-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------

