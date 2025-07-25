# Two-Pass-Assembler
						
AUTHOR: B.Rahen
ROLL NO.: 2301CS37
Declaration of Authorship
This txt file, claims.txt, is part of the assignment of CS2102 at the 
department of Computer Science and Engg, IIT Patna . 

Files:
==========
*All the files I submitted, asm.cpp and emu.cpp and claims.txt, have my name and student id at the start, along with a declaration of authorship.

The evidence for Assembler including the output files from the eight test examples has also been uploaded. They are test1(3 output files), test2(3 output files), test3
(3 output file), test4(3 output files), test5(3 output files), test6(3 output files), MyBubbleSort(3 output files). All the test files have been tested and work properly. As for the evidence for the emulator is uploaded
with eight output files in file.TRACE format, for all the same files.

* Submission also includes Report of the project in document and pdf format to provide evidence of the functioning of both assembler and emulator

*Both of my cpp programs did not show any errors and warnings when I compiled it with g++ file.cpp

*Two programs are compilable with g++ asm.cpp -o asm and g++ emu.cpp -o emu.


=============================
Program format and structure:
=============================
1. The assembler:

    * with two data structures to store respectively labels and mnemonic table with expected operands.
    * uses a single routine for both passes.
    * detects label errors.
    * consistent and sensible formatting with sensible program structure. 
    * sensible variable, function & type names with explanatory comments.
		* advanced listing file.
		* assembles test programs.
	  * can implements and demonstrates the instruction SET.

2. The emulator :

    * with two data structures to store respectively memory, mnemonic table with expected operands
    * loads object file, 
    * by giving the instructions like "-befor/-after" , the program can produce memory dump before/after execution.
    * by giving the instruction like "-trace", the program prints the result of program executed.
    * detects errant programs.


=============================
Testing:
=============================

1. The assembler:

I have tested seven examples shown as bellow. The errors only occurred in the file test2.asm and the errors are logged and shown in terminal.

The others were tested without any errors detected and produced three output files, listing file, 
error log file and machine readable object file, for each example. 

After comparing the listing files and object file of all test files
with the implemented result given on the assignment, both of them show no difference 
from those provided.
 

1)
#Input: asm test1.asm
#Output: 
a. test1.l 
b. test1.log (without any error messages)
c. test1.o

2)
#Input:asm test2.asm
#Output: 
a. test2.l 
b. test2.log (without error messages shown as below but also in log file)
c. test2.o

error messages:
ERROR: Duplicate Label at line 4
WARNING: Incorrect label format at line 10
ERROR: Unknown Symbol as operand at line 5
ERROR: Unknown Symbol as operand at line 6
ERROR: Operand expected at line 7
ERROR: Operand not expected at line 8
ERROR: Unknown Symbol as operand at line 9
ERROR: Mnemonic not defined at line 11
ERROR: Mnemonic not defined at line 12

3)
#Input:./asm.exe test3.asm
#Output: 
a. test3.l 
b. test3.log (without any error messages)
c. test3.o

4)
#Input:./asm.exe test4.asm
#Output: 
a. test4.l 
b. test4.log (without any error messages)
c. test4.o

5)
#Input:./asm.exe test5.asm
#Output: 
a. test5.l 
b. test5.log (without any error messages)
c. test5.o

6)
#Input:./asm.exe test6.asm
#Output: 
a. test6.l 
b. test6.log (without any error messages)
c. test6.o

7)
#Input:./asm.exe bubbleSort.asm
#Output: 
a. bubbleSort.l 
b. bubbleSort.log (without any error messages)
c. bubbleSort.o


2. The emulator:

I have tested seven examples shown as bellow. The errors only occurred in the file test1.o and 
the program immediately stopped with an error message of infinite loop on the screen. 

The others were tested without any errors detected and produced excution data both on the screen and 
output trace file for each example according to the usage instruction. 

1)
#Input:./emu.exe -trace test1.o
#Output: test1.TRACE

Error message: 
Infinite loop detected
3 number of instructions executed

2)
#Input:./emu.exe -trace test3.o
#Output: test3.TRACE

4 number of instructions executed

3)
#Input:./emu.exe -trace test4.o
#Output: test4.TRACE

47653 number of instructions executed

4)
#Input:./emu.exe -trace test5.o
#Output: test5.TRACE

41 number of instructions executed

5)
#Input:./emu.exe -trace test6.o
#Output: test6.TRACE

1403 number of instructions executed

6)
#Input:./emu.exe -trace MyBubbleSort.o
#Output: MyBubbleSort.TRACE

1439 number of instructions executed
