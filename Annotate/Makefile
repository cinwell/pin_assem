#ifneq ($(CC), gcc)
$(info $(CC))
CC = gcc #/usr/local/gcc-4.8.2/bin/g++
#endif

CFLAGS = -Wall -g
OPTFLAGS = -O1

DEBUG = 1

# *** CONDITIONAL DIRECTIVE ***
ifdef DEBUG
ifeq ($(DEBUG), 1)
OPTFLAGS=-O0 -g
endif
endif

CFLAGS += $(OPTFLAGS)
$(info $(CFLAGS))

# *** MULTIPLE MARGETS ***
# first target
EXE1 = ANNO
# second target
#EXE2 = type
# all targets
EXES = $(EXE1) #$(EXE2)
$(info $(EXES))

# a trick line without command line
# target 'all' depends on multiple targets
# thus, if one of these targets is not up-to-date, 'all' will be generated in turn causes all programs compiled
# do as follow
#  get all targets: make all
#  get specific target: make xx
all: $(EXES)

$(EXE1) : anno.o
#$(EXE2) : type.o

# *** string function ***
# get all c files: x.c y.c
SRC = $(wildcard *.c)
# *** filename function ***
# $(basename names...): extract the suffix of each file name in 'names'
# $(addsuffix suffix, names): 'suffix' is appended to the end of each individual name
# 	x.o, y.o
OBJ = $(addsuffix .o, $(basename $(SRC)))

$(EXES) : 
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Link $@ successfully"

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<
	@echo "Compile $@ successfully"

# autogenerate dependency files, and include to achieve the effect that once header file is updated, remake
-include $(OBJ:.o=.dep)
%.dep : %.c
	$(CC) -M $(CFLAGS) $< > $@

clean:
	rm -f *.o $(EXES) *.dep

