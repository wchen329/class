.SUFFIXES: .o .cpp
CC = g++
VPATH = src:build
BIN_DIR = bin
LIB_DIR = build
CFLAGS = -g -I"$(INCLUDE)"
OBJS =  env.o format_chk.o messages.o mtsstream.o \
	range.o runtime_call.o program.o \
	shell.o streams.o syms_table.o priscas_osi.o
SHELL_MAIN = shell_entry.o
INCLUDE = include

all: build/libmtcore.a $(BIN_DIR)/class

build/libmtcore.a: $(OBJS)
	cd build; ar r libmtcore.a $(OBJS)

$(BIN_DIR)/class: $(OBJS) $(SHELL_MAIN) $(INCLUDE)
	@ echo "Looking for bin directory..."
	@ if \
		ls $(BIN_DIR); \
	then \
		echo "bin directory exists"; \
	else \
		echo "bin doesn't exist, making bin..." && \
		mkdir $(BIN_DIR); \
	fi
	cd build; $(CC) $(OBJS) $(SHELL_MAIN) -g -o ../$@

.cpp.o:
	$(CC) $(CFLAGS) -c $<
	mv $*.o build 
release:
	$(MAKE) all CFLAGS="-O3 -I\"$(INCLUDE)\" -DP_RELEASE"
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
