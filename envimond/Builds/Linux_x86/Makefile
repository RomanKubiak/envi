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
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_62C6ADE1=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../JuceLibraryCode/modules -I ../../Source -I ../../Source/Source -I ../../Source/Storage -I ../../Source/Network -I ../../Source/Externals -I ~/devel/JUCE
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lX11 -ldl -lpthread -lrt -lX11 -ldl -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_62C6ADE1=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../JuceLibraryCode/modules -I ../../Source -I ../../Source/Source -I ../../Source/Storage -I ../../Source/Network -I ../../Source/Externals -I ~/devel/JUCE
  TARGET := envimond
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_62C6ADE1=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../JuceLibraryCode/modules -I ../../Source -I ../../Source/Source -I ../../Source/Storage -I ../../Source/Network -I ../../Source/Externals -I ~/devel/JUCE
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Os
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -lX11 -ldl -lpthread -lrt -lX11 -ldl -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_62C6ADE1=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../JuceLibraryCode/modules -I ../../Source -I ../../Source/Source -I ../../Source/Storage -I ../../Source/Network -I ../../Source/Externals -I ~/devel/JUCE
  TARGET := envimond
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
  $(OBJDIR)/EnviData_6447626f.o \
  $(OBJDIR)/bit_ecebd8a7.o \
  $(OBJDIR)/getopt_1199f4d.o \
  $(OBJDIR)/lua_ed7df0d2.o \
  $(OBJDIR)/sqlite3_42a04441.o \
  $(OBJDIR)/EnviHTTP_b152028e.o \
  $(OBJDIR)/EnviHTTPConnection_ceb1734c.o \
  $(OBJDIR)/EnviDSLua_f364ea42.o \
  $(OBJDIR)/EnviDSTrigger_d517e982.o \
  $(OBJDIR)/EnviDSFile_a4da9d3c.o \
  $(OBJDIR)/EnviDSPCF8591_acb629f2.o \
  $(OBJDIR)/EnviDSDHT22_1d6cc3ba.o \
  $(OBJDIR)/EnviDataSource_d73fb816.o \
  $(OBJDIR)/EnviDSCommand_a6957195.o \
  $(OBJDIR)/EnviDSDHT11_1ba9d39a.o \
  $(OBJDIR)/EnviWiringPi_a6e8514c.o \
  $(OBJDIR)/EnviDB_f29e9557.o \
  $(OBJDIR)/EnviFlatFileStore_2148ce3d.o \
  $(OBJDIR)/EnviSqlite3Store_bc4d2d73.o \
  $(OBJDIR)/EnviApplication_4adf88f5.o \
  $(OBJDIR)/EnviCLI_b2488c5.o \
  $(OBJDIR)/EnviLog_249cfce9.o \
  $(OBJDIR)/Envimond_e1dc027d.o \
  $(OBJDIR)/juce_core_aff681cc.o \
  $(OBJDIR)/juce_data_structures_bdd6d488.o \
  $(OBJDIR)/juce_events_79b2840.o \

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

$(OBJDIR)/EnviData_6447626f.o: ../../Source/EnviData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/bit_ecebd8a7.o: ../../Source/Externals/bit.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling bit.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/getopt_1199f4d.o: ../../Source/Externals/getopt.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling getopt.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/lua_ed7df0d2.o: ../../Source/Externals/lua.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling lua.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/sqlite3_42a04441.o: ../../Source/Externals/sqlite3.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling sqlite3.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviHTTP_b152028e.o: ../../Source/Network/EnviHTTP.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviHTTP.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviHTTPConnection_ceb1734c.o: ../../Source/Network/EnviHTTPConnection.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviHTTPConnection.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDSLua_f364ea42.o: ../../Source/Source/EnviDSLua.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDSLua.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDSTrigger_d517e982.o: ../../Source/Source/EnviDSTrigger.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDSTrigger.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDSFile_a4da9d3c.o: ../../Source/Source/EnviDSFile.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDSFile.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDSPCF8591_acb629f2.o: ../../Source/Source/EnviDSPCF8591.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDSPCF8591.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDSDHT22_1d6cc3ba.o: ../../Source/Source/EnviDSDHT22.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDSDHT22.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDataSource_d73fb816.o: ../../Source/Source/EnviDataSource.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDataSource.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDSCommand_a6957195.o: ../../Source/Source/EnviDSCommand.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDSCommand.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDSDHT11_1ba9d39a.o: ../../Source/Source/EnviDSDHT11.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDSDHT11.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviWiringPi_a6e8514c.o: ../../Source/Source/EnviWiringPi.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviWiringPi.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviDB_f29e9557.o: ../../Source/Storage/EnviDB.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviDB.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviFlatFileStore_2148ce3d.o: ../../Source/Storage/EnviFlatFileStore.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviFlatFileStore.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviSqlite3Store_bc4d2d73.o: ../../Source/Storage/EnviSqlite3Store.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviSqlite3Store.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviApplication_4adf88f5.o: ../../Source/EnviApplication.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviApplication.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviCLI_b2488c5.o: ../../Source/EnviCLI.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviCLI.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnviLog_249cfce9.o: ../../Source/EnviLog.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnviLog.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Envimond_e1dc027d.o: ../../Source/Envimond.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Envimond.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_aff681cc.o: ../../JuceLibraryCode/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_bdd6d488.o: ../../JuceLibraryCode/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_79b2840.o: ../../JuceLibraryCode/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
