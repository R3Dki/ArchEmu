#include "lib/types.hpp"
#include "cpus/cpu_includes.hpp"

#include <iostream>

using namespace std;


i32 main () {
/*    cout << "ArchEmu CPU Architecture Emulator\nArchitectures/ICs:\n\
MOS: 6501, 6502\n\
Zilog: Z80, eZ80, Z180, Z280, Z380, Z800, Z8000\n\
Intel: 4004, 4040, 8008, 8080, 8085, 8086, 8088, 80186, 80188, 80286, i386" << endl;
    cout << "Instancing Intel 4004." << endl; */
    
    Intel_4040 CPU1;

    string path =".\\4004bin.bin";

    if (CPU1.LoadFileToROM(path)) return 1;
    if (CPU1.GetExecutionState() != READY) return 1;

    bool runinstantly = false;
    char checkchar = 0;

    for (u64 i = 0; i < 5000; i++) {
        CPU1.Execute();
        //cout << "Prog. Counter: " << CPU1.registers12[Intel4004::PC] << endl;
        if (!runinstantly) {
            CPU1.PrintRegisters();
            checkchar = getchar();
            if (checkchar == 'x') break;
            if (checkchar == 'c') runinstantly = !runinstantly;
            system("cls");
        }
        if (!CPU1.canExecute) break;
    }
    CPU1.PrintCPUData();
}
