# pin_assem
Capture instructions will be captured and outputed into a trace file. In addition, assembly code can be inserted to source program to label the start and end of a snippet.

#### 1. ./src: holds source files of pintool
* pinAssem.cpp: captures instructions of executable
* printmacros.h: defines print macros
* util.h/util.cpp: defines some global variabled and functions
* anno.h: defines macros to hook instructions
* makefile.rules: pin makefile
* makefile: makefile

#### 2. ./Annotate: holds source codes to be instrumented
* anno.c: source code
* Makefile: makefile

#### 3. how to run?
* install Intel pin and update the 'PIN_ROOT' in src/makefile
* executes './make.sh' (or step into src to make) to compile pin source files, *.so will be created in src/obj-intel64/
* step into Annotate/ and make
* ./run.sh to instrument, output trace can be found in gen.trace/test_anno
