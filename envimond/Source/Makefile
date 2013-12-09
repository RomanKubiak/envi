# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := /usr/local/bin/

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=armv6
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_EAB53354=1" -I /usr/include -I /usr/include/freetype2 -I ../JuceLibraryCode -I ../../../JUCE/modules -I /usr/include/juce
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lX11 -ldl -lpthread -lrt -lwiringPi -ljuce-static-debug -lX11 -ldl -lrt -lsqlite3 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_EAB53354=1" -I /usr/include -I /usr/include/freetype2 -I ../JuceLibraryCode -I ../../../JUCE/modules -I /usr/include/juce
  TARGET := envimond
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=armv6
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_EAB53354=1" -I /usr/include -I /usr/include/freetype2 -I ../JuceLibraryCode -I ../../../JUCE/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Os
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -lX11 -ldl -lpthread -lrt -lwiringPi -ljuce-static-debug -lX11 -ldl -lrt -lsqlite3 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_EAB53354=1" -I /usr/include -I /usr/include/freetype2 -I ../JuceLibraryCode -I ../../../JUCE/modules
  TARGET := envimond
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
  $(OBJDIR)/minilzo_667709d1.o \
  $(OBJDIR)/EnviWiringPi_1a485a2c.o \
  $(OBJDIR)/EnviFlatFileStore_6321d425.o \
  $(OBJDIR)/EnviHTTPConnection_b45d3b77.o \
  $(OBJDIR)/EnviDataSource_f2c108f6.o \
  $(OBJDIR)/EnviDB_992e306f.o \
  $(OBJDIR)/EnviDSBMP085_1854b68.o \
  $(OBJDIR)/EnviDSCommand_9f3684b5.o \
  $(OBJDIR)/EnviDSDHT11_69b52eba.o \
  $(OBJDIR)/EnviLog_839ebf1d.o \
  $(OBJDIR)/EnviApplication_12e5d729.o \
  $(OBJDIR)/EnviCLI_6a264af9.o \
  $(OBJDIR)/EnviHTTP_9dc6f479.o \
  $(OBJDIR)/Envimond_631286c9.o \
  $(OBJDIR)/juce_core_9d6b33ad.o \
  $(OBJDIR)/juce_data_structures_dcd877a9.o \
  $(OBJDIR)/juce_events_e5b201a1.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking envimond
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning envimond
	-@rm -f $(OUTDIR)/$(TARGET)
	-@rm -rf $(OBJDIR)/*
	-@rm -rf $(OBJDIR)

strip:
	@echo Stripping envimond
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/minilzo_667709d1.o: Ext/minilzo.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling minilzo.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviWiringPi_1a485a2c.o: EnviWiringPi.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviWiringPi.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviFlatFileStore_6321d425.o: EnviFlatFileStore.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviFlatFileStore.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviHTTPConnection_b45d3b77.o: EnviHTTPConnection.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviHTTPConnection.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDataSource_f2c108f6.o: EnviDataSource.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDataSource.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDB_992e306f.o: EnviDB.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDB.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDSBMP085_1854b68.o: EnviDSBMP085.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDSBMP085.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDSCommand_9f3684b5.o: EnviDSCommand.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDSCommand.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDSDHT11_69b52eba.o: EnviDSDHT11.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDSDHT11.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviLog_839ebf1d.o: EnviLog.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviLog.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviApplication_12e5d729.o: EnviApplication.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviApplication.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviCLI_6a264af9.o: EnviCLI.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviCLI.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviHTTP_9dc6f479.o: EnviHTTP.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviHTTP.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Envimond_631286c9.o: Envimond.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Envimond.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_9d6b33ad.o: ../../../JUCE/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_dcd877a9.o: ../../../JUCE/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_e5b201a1.o: ../../../JUCE/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
