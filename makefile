# "I Have No Tomatoes" Makefile (GNU/Linux)
# Run 'make DEBUGMODE=1' if you need to debug the game.
# Or run 'make PROFILE=1' if you need to profile the game.

.PHONY: all clean veryclean rebuild compress

TARGET = ./tomatoes

ARCH_NAME := $(shell '$(TRGT)gcc' -dumpmachine)

CROSS?=

CC   = $(CROSS)gcc
CXX  = $(CROSS)g++
AS   = $(CROSS)gcc -x assembler-with-cpp

LD   = $(CROSS)g++
AR   = $(CROSS)ar rvc

RM= rm --force --verbose
UPX = upx --best

PACKAGES= sdl2 SDL2_image SDL2_mixer gl glu
PKGCONFIG= pkg-config

PKG_CONFIG_CFLAGS=$(shell pkg-config --cflags $(PACKAGES))
PKG_CONFIG_LDFLAGS=$(shell pkg-config --libs-only-L $(PACKAGES))
PKG_CONFIG_LIBS=$(shell pkg-config --libs-only-l $(PACKAGES))

CFLAGS= \
	-Wall \
	-fwrapv \
	-fstack-protector-strong \
	-Wall \
	-Wformat \
	-Werror=format-security \
	-Wdate-time \
	-D_FORTIFY_SOURCE=2 \
	-fPIC

LDFLAGS= \
	-Wl,-O1 \
	-Wl,-Bsymbolic-functions \
	-Wl,-z,relro \
	-Wl,--as-needed \
	-Wl,--no-undefined \
	-Wl,--no-allow-shlib-undefined \
	-Wl,-Bsymbolic-functions \
	-Wl,--dynamic-list-cpp-new \
	-Wl,--dynamic-list-cpp-typeinfo

CSTD=-std=gnu17
CPPSTD=-std=gnu++17

OPTS= -O2 -g

DEFS= \
	-DDEBUG \
	-D_LARGEFILE64_SOURCE \
	-D_FILE_OFFSET_BITS=64 \
	-DGL_GLEXT_PROTOTYPES

INCS= \
	-Iinclude

LIBS= \
	-lm





# Directory defines, you can use these defaults or adjust them if
# necessary. Remember to include the trailing /

# MPK directory (where 'tomatoes.mpk' is), default: ./
MPKDIR = ./

# Music directory (where the music files are), default: ./music/
MUSICDIR = ./music/

# Hiscore directory (where the hiscores are written to), default: ./
# We need read/write access!
HISCOREDIR = ./

# Config directory (where the 'config.cfg' is), default: ./
# We need read/write access!
CONFIGDIR = ./

# Override directory (unused at the moment), default: ./data/
OVERRIDEDIR = ./data/


DIR_DEFINES = -DLINUX -DMPK_DIR=\"$(MPKDIR)\" -DMUSIC_DIR=\"$(MUSICDIR)\" -DHISCORE_DIR=\"$(HISCOREDIR)\" -DCONFIG_DIR=\"$(CONFIGDIR)\" -DOVERRIDE_DIR=\"$(OVERRIDEDIR)\"

# Debugmode stuff
ifdef DEBUGMODE
CFLAGS = $(PKG_CONFIG_CFLAGS) -MMD -g3 -W -Wall -DDEBUGMODE
LDFLAGS = $(PKG_CONFIG_LDFLAGS) $(PKG_CONFIG_LIBS)
else
ifdef PROFILE
CFLAGS = $(PKG_CONFIG_CFLAGS) -MMD -g3 -O3 -Wall -pg
LDFLAGS = $(PKG_CONFIG_LDFLAGS) $(PKG_CONFIG_LIBS) -pg
else
endif
endif

# Source and object files
SOURCES = $(sort $(wildcard src/*.cpp))
OBJS = $(SOURCES:.cpp=.o)
OBJS := $(subst src/,obj/,$(OBJS))

# Include directories
INCLUDES = -I./include


# Targets
all: $(TARGET)

# Check dependancies
DEPS = $(subst .o,.d,$(OBJS))
-include $(DEPS)


$(TARGET): $(OBJS)
	@mkdir -p "$(dir $@)"
	$(LD) $(CPPSTD) $(CSTD) $(LDFLAGS) $(PKG_CONFIG_LDFLAGS) \
		-o $@ $^ $(LIBS) $(PKG_CONFIG_LIBS)

obj/%.o: src/%.cpp
	@mkdir -p "$(dir $@)"
	$(CXX) $(CPPSTD) $(OPTS) -o $@ -c $< \
		$(DEFS) $(DIR_DEFINES) $(INCS) $(CFLAGS) $(PKG_CONFIG_CFLAGS)

obj/%.o: src/%.c
	@mkdir -p "$(dir $@)"
	$(CC) $(CSTD) $(OPTS) -o $@ -c $< \
		$(DEFS) $(DIR_DEFINES) $(INCS) $(CFLAGS) $(PKG_CONFIG_CFLAGS)


clean:
	rm -f $(OBJS) $(TARGET)

veryclean:
	rm -f $(OBJS) $(TARGET) $(DEPS)

rebuild: veryclean all

# Compress the exe with UPX
compress: $(TARGET)
	$(UPX) $(TARGET)
