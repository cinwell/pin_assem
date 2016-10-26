#include "pin.H"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include "anno.h"
#include "printmacros.h"
#include "util.h"

typedef unsigned long long LARGEINT;

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "output", "default", "specify trace filename");

static ADDRINT InsCount = 0;
std::fstream TraceFile;
std::fstream MemFile;
PIN_LOCK lock;

static INT32 Usage() {
	ERROR ("Anyway, this is a mistake!!!");
	ERROR (KNOB_BASE::StringKnobSummary());
	return -1;
}

VOID record_ins(const THREADID threadid, const ADDRINT opCode, const ADDRINT numops, const string op_str) {
	auto ins_op = OPCODE_StringShort(opCode);
    AX_LOG_ASSEM(TraceFile, InsCount << "-" << ins_op << op_str);
}

static VOID handleHook(THREADID threadIndex, ADDRINT cmd, ADDRINT arg0, ADDRINT arg1)
{
   if(cmd == APPROX_CMD){//come across a APPROX hook
        ADDRINT approxId = arg0;//get the hook ID
        if(arg1 == _START){//a START
            AX_LOG_ASSEM(TraceFile, "Hook [" << approxId << "] starting ...");
            ERROR ("====> Hook [" << approxId << "] starting ...");
        }
        if(arg1 == _END){//a END
            AX_LOG_ASSEM(TraceFile, "Hook [" << approxId << "] ending ...");
            ERROR ("====> Hook [" << approxId << "] ending ...");
        }

   }
}

VOID Instruction(INS ins, VOID *v){
	/**
 	 * INS_InsertCall(INS ins, IPOINT action, AFUNPTR funptr, ...)
 	 *
 	 * insert a call to 'docount' relative to instruction 'ins'
 	 *
 	 * ins: instruction to instrument
 	 * action: specifies before/after, etc. IPOINT_BEFORE is always valid for all instructions.
 	 * funptr: insert a call to funptr.
 	 * ...: list of arguments to pass funptr, terminated with IARG_END
 	 */ 
	//INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)stat_ins, IARG_END);
	InsCount ++;

	//string ins_cat = CATEGORY_StringShort(INS_Category(ins));
	int num_ops = INS_OperandCount(ins);                                //total #operands
    int memOperands = INS_MemoryOperandCount(ins);                      //#mem_operands
    
    string op_string = "";                                              //string to record all operands
    stringstream sstm;                                                  //string stream

    int num_mems = 0;
	for(int ii=0; ii<num_ops; ii++){                                    //iterate each operand
        if(INS_OperandIsImmediate(ins, ii)){                            //immediate
            auto value = INS_OperandImmediate(ins, ii);
            sstm.str("");                                               //empty
            sstm << "$" << value;
            op_string += " " + sstm.str();
        } else if(INS_OperandIsReg(ins, ii)){                           //register
           auto reg = REG_StringShort(INS_OperandReg(ins, ii)); 
           sstm.str("");
           sstm << "%" << reg;
           op_string += " " + sstm.str();
        } else if(INS_OperandIsMemory(ins, ii) && memOperands>0){       //memory
            string mem_type = "memXX";
            if(INS_MemoryOperandIsRead(ins, num_mems)) {
                mem_type = "memR";
            } else if(INS_MemoryOperandIsWritten(ins, num_mems)) {
                mem_type = "memW";
            }

            if(INS_MemoryOperandIsRead(ins, num_mems) && INS_MemoryOperandIsWritten(ins, num_mems)) {
                mem_type = "memRW";
            }

            ++ num_mems;
            op_string += " " + mem_type;
        //true if this operand is a memory reference,
        //Note: this does not include LEA operands.
        } else if(INS_OperandIsMemory(ins, ii) && memOperands==0){      //NOP
            assert(INS_IsNop(ins));
        } else {
            //TRUE if memory operand uses predefined base register and this register can not be changed 
            //Example: movs ds:(esi), es:(edi) There are two fixed operands
            string other_type = "";
            if(INS_OperandIsFixedMemop(ins, ii))
                other_type = "FM";
            //true if this operand is a displacement (e.g. branch offset)
            else if(INS_OperandIsBranchDisplacement(ins, ii))
                other_type = "BD";
            //true if this operand is implied by the opcode (e.g. the stack write in a push instruction)
            else if(INS_OperandIsImplicit(ins, ii))
                other_type = "IM";
            else {
                assert(INS_IsLea(ins));
                other_type = "lea";
            }
            op_string += " " + other_type;
        }
    }

    assert(num_mems == memOperands);
    assert(num_ops <= 6);

    //record ins
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)record_ins,
            IARG_THREAD_ID,
            IARG_UINT32,
            INS_Opcode(ins),
            IARG_UINT32, num_ops,
            IARG_PTR, new string(op_string),
            IARG_END);

    if (INS_IsXchg(ins) && INS_OperandReg(ins, 0)==REG_BX && INS_OperandReg(ins, 1)==REG_BX)
    {
        //INS_InsertPredictedCall() is used to call analysis functions.
        //This API function prevents pollution of the memory analysis by calling an analysis function only if a particular
        //instruction is actualy executed, i.e., only if the instruction is executed.
        INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)handleHook, IARG_THREAD_ID, IARG_REG_VALUE, REG_GAX,
        #ifdef TARGET_IA32
        IARG_REG_VALUE, REG_GDX,
        #else
        IARG_REG_VALUE, REG_GBX,
        #endif
        IARG_REG_VALUE, REG_GCX, IARG_END);
    }
}

VOID Fini(INT32 code, VOID *v) {
	PRINT_SCN ("All applications are at the end" << "...");
    #ifdef LOG_ASSEM
     TraceFile.close();
    #endif
    MemFile.close();
}

int main(int argc, char *argv[]) {
	//Initialize symbol table code. Pin does not read symbols unless this is called
	PIN_InitSymbols();

	//initialize Pin system
	if(PIN_Init(argc,argv)) {
		return Usage();
	}

    string filename = KnobOutputFile.Value();

    //file to record all instructions
    #ifdef LOG_ASSEM
     //TraceFile.open(filename, ios::out);
     AxOpenFile(TraceFile, filename);
     if (TraceFile.is_open()) {
        PRINT_SCN(filename << " : Start to make trace at instruction #" << InsCount);
     } else {
        PRINT_SCN("cannot open");
        return -1;
     }
    #endif
    //file to record all memory accesses
    MemFile.open("mem.txt", ios::out);

	//add a function used to instrument at instruction granularity 
	INS_AddInstrumentFunction(Instruction, 0);
	//call 'Fini' immediately before the application exits
	PIN_AddFiniFunction(Fini, 0);

	//starts executing the application
	PIN_StartProgram();

	return 0;
}
