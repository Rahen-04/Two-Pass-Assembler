#include <bits/stdc++.h>
using namespace std;

// Registers
int32_t A, B, PC, SP;
// Memory array
int32_t memory[10000];

// Convert integer to hexadecimal
string intToHex(int i) {
    stringstream stream;
    stream << setfill('0') << setw(8) << hex << i;
    return stream.str();
}

// Retrieve operand and opcode from machine code
tuple<int32_t, int32_t> decodeInstruction(int32_t instr) {
    int32_t opcode = instr & 0xff;
    int32_t operand = (instr & 0xffffff00) >> 8;
    return make_tuple(operand, opcode);
}

// Print usage instructions
void printUsage() {
    cout << "Usage: ./emu.exe [option] file.o\n";
    cout << "Options:\n\t-trace\tshow instruction trace\n\t-before\tshow memory dump before execution\n\t-after\tshow memory dump after execution\n\t-isa\tdisplay ISA\n";
}

// Dump memory content
void dumpMemory(int limit, ofstream& trcfile) {
    cout << "Memory Dump:\n";
    trcfile << "Memory Dump:\n";
    for (int i = 0; i < limit; i++) {
        if (i % 4 == 0) {
            cout << "\n" << intToHex(i) << "\t";
            trcfile << "\n" << intToHex(i) << "\t";
        }
        cout << intToHex(memory[i]) << " ";
        trcfile << intToHex(memory[i]) << " ";
    }
    cout << endl;
}

// Print ISA
void printISA() {
    cout << "Opcode Mnemonic Operand\n";
    cout << "       data     value\n";
    cout << "0      ldc      value\n";
    cout << "1      adc      value\n";
    cout << "2      ldl      value\n";
    cout << "3      stl      value\n";
    cout << "4      ldnl     value\n";
    cout << "5      stnl     value\n";
    cout << "6      add\n";
    cout << "7      sub\n";
    cout << "8      shl\n";
    cout << "9      shr\n";
    cout << "10     adj      value\n";
    cout << "11     a2sp\n";
    cout << "12     sp2a\n";
    cout << "13     call     offset\n";
    cout << "14     return\n";
    cout << "15     brz      offset\n";
    cout << "16     brlz     offset\n";
    cout << "17     br       offset\n";
    cout << "18     HALT\n";
    cout << "       SET      value\n";
}

// Initialize opcode table
map<int, string> initOpcodeTable() {
    return {{0, "ldc"}, {1, "adc"}, {2, "ldl"}, {3, "stl"}, {4, "ldnl"},
            {5, "stnl"}, {6, "add"}, {7, "sub"}, {8, "shl"}, {9, "shr"},
            {10, "adj"}, {11, "a2sp"}, {12, "sp2a"}, {13, "call"}, {14, "return"},
            {15, "brz"}, {16, "brlz"}, {17, "br"}, {18, "HALT"}, {19, "data"}, {20, "SET"}};
}

// Execute and trace instructions
int executeInstructions(int pc_start, int limit, ofstream& trcfile, map<int, string>& opcodeTable) {
    int count = 0;
    while (true) {
        int old_pc = PC;
        int32_t operand, opcode;
        tie(operand, opcode) = decodeInstruction(memory[PC]);

        cout << "PC: " << intToHex(PC) << "\tSP: " << intToHex(SP) << "\tA: " << intToHex(A)
             << "\tB: " << intToHex(B) << "\t" << opcodeTable[opcode] << " " << operand << endl;
        trcfile << "PC: " << intToHex(PC) << "\tSP: " << intToHex(SP) << "\tA: " << intToHex(A)
                << "\tB: " << intToHex(B) << "\t" << opcodeTable[opcode] << " " << operand << endl;

        switch (opcode) {
            case 0: B = A; A = operand; break;
            case 1: A += operand; break;
            case 2: B = A; A = memory[SP + operand]; break;
            case 3: memory[SP + operand] = A; A = B; break;
            case 4: A = memory[A + operand]; break;
            case 5: memory[A + operand] = B; break;
            case 6: A += B; break;
            case 7: A = B - A; break;
            case 8: A = B << A; break;
            case 9: A = B >> A; break;
            case 10: SP += operand; break;
            case 11: SP = A; A = B; break;
            case 12: B = A; A = SP; break;
            case 13: B = A; A = PC; PC += operand; break;
            case 14: PC = A; A = B; break;
            case 15: if (A == 0) PC += operand; break;
            case 16: if (A < 0) PC += operand; break;
            case 17: PC += operand; break;
            case 18: cout << count << " instructions executed\n"; return count;
        }
        PC++;
        if (old_pc == PC) {
            cout << "Infinite loop detected\n"; break;
        }
        count++;
    }
    return count;
}

// Load instructions into memory
int loadInstructions(ifstream& infile) {
    int32_t instr_code, poc = 0;
    while (infile.read((char*)&instr_code, sizeof(instr_code))) {
        int32_t operand, opcode;
        tie(operand, opcode) = decodeInstruction(instr_code);
        if (opcode == 19 || opcode == 20) memory[poc++] = operand;
        else if (opcode >= 0 && opcode < 20) memory[poc++] = instr_code;
        else { cout << "Invalid instruction\n"; return 0; }
    }
    return poc;
}

int main(int argc, char* argv[]) {
    if (argc < 3) { printUsage(); return 0; }

    map<int, string> opcodeTable = initOpcodeTable();
    ifstream infile(argv[2], ios::out | ios::binary);
    ofstream trcfile(string(argv[2]).substr(0, string(argv[2]).find(".")) + ".trace");

    int poc = loadInstructions(infile);
    A = B = PC = 0;
    SP = sizeof(memory) / sizeof(memory[0]) - 1;

    if (string(argv[1]) == "-before") dumpMemory(poc, trcfile);
    else if (string(argv[1]) == "-trace") executeInstructions(0, poc, trcfile, opcodeTable);
    else if (string(argv[1]) == "-after") { executeInstructions(0, poc, trcfile, opcodeTable); dumpMemory(poc, trcfile); }
    else if (string(argv[1]) == "-isa") printISA();

    infile.close();
    trcfile.close();
    return 0;
}
