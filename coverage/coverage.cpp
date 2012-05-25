
#include "pin.h"

#include <fstream>
#include <iostream>
#include <map>
#include <set>


// path prefix tool option
KNOB<string> KnobPathPrefix(KNOB_MODE_WRITEONCE, "pintool", "p", "", "path prefix");
KNOB<string> KnobApplicationName(KNOB_MODE_WRITEONCE, "pintool", "n", "app", "application name");


// file information
typedef struct FileInfo
{
    UINT32          _instructions;  // number of instructions in file (inside routines)
    UINT32          _executed;      // number of actually executed instructions

} FILE_INFO;

// routine information
typedef struct RtnInfo
{
    ADDRINT         _address;       // address of routine in image

    set<ADDRINT>    _uniqueInsAddr; // set of all executed instruction addresses

    UINT32          _instructions;  // number of instructions in routine

    string          _name;          // routine name
    string          _file;          // source code file name

    // linked list
    struct RtnInfo* _next;

} RTN_INFO;

// linked list of routine information
RTN_INFO* RtnInfoList = 0;


// instrumentation function -> save executed instruction address
VOID instructionExecuted(RTN_INFO* rtnInfo, ADDRINT address)
{
    rtnInfo->_uniqueInsAddr.insert(address);
}

// instruments routine
VOID instrumentRoutine(RTN rtn, string name, string file)
{
    // allocate information block
    RTN_INFO* ri = new RTN_INFO;

    // initialize routine information
    ri->_address = RTN_Address(rtn);

    ri->_instructions = RTN_NumIns(rtn);

    ri->_name = name;
    ri->_file = file;

    // add to list of routine information
    ri->_next = RtnInfoList;
    RtnInfoList = ri;

    // instrument each instruction
    for (INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins))
    {
        // skip noops
        if (!INS_IsNop(ins))
        {
            // increment executed instruction count
            INS_InsertCall(
                ins,
                IPOINT_BEFORE,
                (AFUNPTR)instructionExecuted,
                IARG_PTR,
                ri,
                IARG_ADDRINT,
                INS_Address(ins),
                IARG_END
            );
        }
        else
        {
            // adapt instruction count
            ri->_instructions--;
        }
    }
}

// load image event -> instrument contained routines
VOID imageLoadingCB(IMG img, VOID* v)
{
    // get path prefix
    string pathPrefix = KnobPathPrefix.Value();

    for (SEC sec = IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec))
    { 
        for (RTN rtn = SEC_RtnHead(sec); RTN_Valid(rtn); rtn = RTN_Next(rtn))
        {
            RTN_Open(rtn);

            // get symbol information for routine
            INT32 column = 0;
            INT32 line = 0;
            string fileName = "";

            PIN_GetSourceLocation(RTN_Address(rtn), &column, &line, &fileName);

            // source code available?
            if ((fileName != "") && ((pathPrefix == "") || (fileName.find(pathPrefix) == 0)))
            {
                instrumentRoutine(rtn, RTN_Name(rtn), fileName);
            }

            RTN_Close(rtn);
        }
    }
}

// called when the application exits
// prints the routine information
VOID applicationExitCB(INT32 code, VOID* v)
{
    map<string, FILE_INFO> fileInformation;

    string routineInfoFileName = KnobApplicationName.Value() + "_routines.csv";
    string fileInfoFileName = KnobApplicationName.Value() + "_files.csv";

    // save routine based coverage information
    ofstream routineCoverage(routineInfoFileName.c_str());

    routineCoverage << "File;Routine;Instructions SUM;Instructions EXEC;Coverage;Instructions SIGN" << endl;

    for (RTN_INFO* ri = RtnInfoList; ri; ri = ri->_next)
    {
        // basic information
        routineCoverage << ri->_file << ";"
                        << ri->_name << ";"
                        << ri->_instructions << ";"
                        << ri->_uniqueInsAddr.size() << ";"
                        << ((ri->_uniqueInsAddr.size()*10000) / ri->_instructions) / 100 << "."
                        << ((ri->_uniqueInsAddr.size()*10000) / ri->_instructions) % 100 << ";";

        // instruction signature
        routineCoverage << "\"";

        set<ADDRINT>::iterator it;
        for(it = ri->_uniqueInsAddr.begin(); it != ri->_uniqueInsAddr.end(); it++)
        {
            routineCoverage << ((*it) - ri->_address) << " ";
        }

        routineCoverage << "\"" << endl;

        // aggregate file information
        FILE_INFO& fi = fileInformation[ri->_file];
        fi._instructions += ri->_instructions;
        fi._executed += ri->_uniqueInsAddr.size();
    }

    // save file based coverage information
    ofstream fileCoverage(fileInfoFileName.c_str());

    fileCoverage << "File;Instructions SUM;Instructions EXEC;Coverage" << endl;

    map<string, FILE_INFO>::iterator it;
    for(it = fileInformation.begin(); it != fileInformation.end(); it++)
    {
        fileCoverage << (*it).first << ";";

        FILE_INFO fi = (*it).second;

        fileCoverage << fi._instructions << ";"
                     << fi._executed << ";"
                     << ((fi._executed*10000) / fi._instructions) / 100 << "."
                     << ((fi._executed*10000) / fi._instructions) % 100 << endl;
    }

}

INT32 usage()
{
    cerr << "This Pintool determines routine based coverage information" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

int main(int argc, char* argv[])
{
    // initialize symbols, required for routine instrumentation
    PIN_InitSymbols();

    // initialize pin
    if (PIN_Init(argc, argv))
    {
        return usage();
    }

    // register callback for image load events
    IMG_AddInstrumentFunction(imageLoadingCB, 0);
    // register finish function
    PIN_AddFiniFunction(applicationExitCB, 0);

    // start the program, never returns
    PIN_StartProgram();
    
    return 0;
}
