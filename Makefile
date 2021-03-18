.SUFFIXES: .o .cpp
CC = g++
VPATH = src:build:contrib/dma_loopback/sw
BIN_DIR = bin
LIB_DIR = build
CFLAGS = -g -I"$(INCLUDE)" -I"$(INCLUDE_CONTRIB)"
OBJS =  env.o messages.o mips.o mtsstream.o \
	range.o program.o \
	shell.o streams.o syms_table.o priscas_osi.o \
	ISA_desc.o ustrop.o
CLOAD_OBJS = AFU.o shell_cload.o runtime_call.o mmem.o
SHELL_MAIN = shell_entry.o
ISA_GEN_MAIN = ISA_gen.o
CLOAD_MAIN = cload_entry.o
INCLUDE = include
INCLUDE_CONTRIB = contrib/dma_loopback/sw
LIB = -L. -lmtcore
LIB_CLOAD = -lopae-c -luuid -lopae-cxx-core -lMPF-cxx -lMPF

all: $(LIB_DIR)/libmtcore.a class cload

class: $(BIN_DIR)/class

cload: $(BIN_DIR)/cload

build/libmtcore.a: $(OBJS)
	cd build; ar r libmtcore.a $(OBJS)

$(BIN_DIR)/class: $(LIB_DIR)/libmtcore.a $(SHELL_MAIN) $(INCLUDE)
	@ echo "Looking for bin directory..."
	@ if \
		ls $(BIN_DIR); \
	then \
		echo "bin directory exists"; \
	else \
		echo "bin doesn't exist, making bin..." && \
		mkdir $(BIN_DIR); \
	fi
	cd build; $(CC) $(SHELL_MAIN) $(LIB) -o ../$@

$(BIN_DIR)/cload: $(LIB_DIR)/libmtcore.a $(CLOAD_MAIN) $(CLOAD_OBJS) $(INCLUDE) 
	cd build; $(CC) $(CLOAD_MAIN) $(CLOAD_OBJS) $(LIB) $(LIB_CLOAD) -o ../$@

.cpp.o:
	$(CC) $(CFLAGS) -c $<
	mv $*.o build 
release:
	$(MAKE) all CFLAGS="-O3 -I\"$(INCLUDE)\" -I"$(INCLUDE_CONTRIB)" -DP_RELEASE"
clean:
	@if \
		rm build/*.o; \
	then \
		echo "Object files removed."; \
	else \
		echo "Object files not found. Skipping..."; \
	fi
	@if \
		rm build/*.a; \
	then \
		echo "Core library file(s) removed."; \
	else \
		echo "Core library file(s) not found. Skipping..."; \
	fi
	@if \
		rm bin/*; \
	then \
		echo "Executable files removed."; \
	else \
		echo "Executable files not found. Skipping..."; \
	fi
	cd testing/unix_test; $(MAKE) clean;
test: $(BIN_DIR)/class
	cd testing/unix_test; export TESTS=`./testobjs.sh` && $(MAKE) "P_LIB=$(OBJS)"; $(MAKE) run;
