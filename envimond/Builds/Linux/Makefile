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

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../JUCE/modules -I /usr/include/juce
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lX11 -ldl -lpthread -lrt -lwiringPi -ljuce-static-debug -lX11 -ldl -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../JUCE/modules -I /usr/include/juce
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

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../JUCE/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Os
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -lX11 -ldl -lpthread -lrt -lwiringPi -ljuce-static-debug -lX11 -ldl -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../JUCE/modules
  TARGET := envimond
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
  $(OBJDIR)/minilzo_c0dd049d.o \
  $(OBJDIR)/EnviWiringPi_dbc4dbe0.o \
  $(OBJDIR)/EnviFlatFileStore_837642f1.o \
  $(OBJDIR)/EnviHTTPConnection_9e96a62b.o \
  $(OBJDIR)/EnviDataSource_4723edaa.o \
  $(OBJDIR)/EnviDB_4bcb0923.o \
  $(OBJDIR)/EnviDSBMP085_c301cd1c.o \
  $(OBJDIR)/EnviDSCommand_d4a3981.o \
  $(OBJDIR)/EnviDSDHT11_c287a686.o \
  $(OBJDIR)/EnviLog_249cfce9.o \
  $(OBJDIR)/EnviApplication_4adf88f5.o \
  $(OBJDIR)/EnviCLI_b2488c5.o \
  $(OBJDIR)/EnviHTTP_1c90702d.o \
  $(OBJDIR)/Envimond_e1dc027d.o \
  $(OBJDIR)/juce_core_28ba299e.o \
  $(OBJDIR)/juce_data_structures_2a02f2da.o \
  $(OBJDIR)/juce_events_a3c29712.o \

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

$(OBJDIR)/minilzo_c0dd049d.o: ../../Source/Ext/minilzo.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling minilzo.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviWiringPi_dbc4dbe0.o: ../../Source/EnviWiringPi.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviWiringPi.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviFlatFileStore_837642f1.o: ../../Source/EnviFlatFileStore.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviFlatFileStore.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviHTTPConnection_9e96a62b.o: ../../Source/EnviHTTPConnection.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviHTTPConnection.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDataSource_4723edaa.o: ../../Source/EnviDataSource.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDataSource.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDB_4bcb0923.o: ../../Source/EnviDB.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDB.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDSBMP085_c301cd1c.o: ../../Source/EnviDSBMP085.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDSBMP085.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDSCommand_d4a3981.o: ../../Source/EnviDSCommand.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDSCommand.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDSDHT11_c287a686.o: ../../Source/EnviDSDHT11.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDSDHT11.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviLog_249cfce9.o: ../../Source/EnviLog.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviLog.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviApplication_4adf88f5.o: ../../Source/EnviApplication.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviApplication.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviCLI_b2488c5.o: ../../Source/EnviCLI.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviCLI.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviHTTP_1c90702d.o: ../../Source/EnviHTTP.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviHTTP.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Envimond_e1dc027d.o: ../../Source/Envimond.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Envimond.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_28ba299e.o: ../../../../JUCE/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_2a02f2da.o: ../../../../JUCE/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_a3c29712.o: ../../../../JUCE/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
