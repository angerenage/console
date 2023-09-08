#pragma once

class CPU; //prevent circular includes with the include at the end

#include <string.h>

#include "../../Logic.h"
#include "ALU.h"

enum Instructions : uint8_t {
	ADD, ADDcc,	//Addition
	SUB, SUBcc,	//Subtraction
	MUL,		//Multiplication
	DIV,		//Division

	AND, ANDcc,	//Bitwise AND
	OR, ORcc,	//Bitwise OR
	XOR,		//Bitwise exclusive OR
	NOT,		//Bitwise NOT
	LSL,		//Logical Shift Left
	LSR,		//Logical Shift Right
	ROL,		//Rotate Left
	ROR,		//Rotate Right

	JMP,		//Unconditional Jump
	Bcc,		//Conditional Branches
	JSR,		//Jump to Subroutine
	RTS,		//Return from Subroutine

	LD, LDcc,	//Load from memory to a register
	ST, STcc,	//Store from a register to memory
	PUSH,		//Push to stack
	POP,		//Pop from stack

	MATMUL,		//Matrix Multiplication

	NOP,		//No Operation
	HALT,		//Halts the CPU
	MOV,		//Mouve data from a register to another
	SETI,
	FETCH		//Block copy using the DMA
};

enum Conditions : uint8_t {
	EQ,	//Equal
	NE,	//Not Equal
	CS,	//Carry Set/Unsigned higher or same
	CC,	//Carry Clear/Unsigned lower
	MI,	//Negative
	PL,	//Positive or Zero
	VS,	//Overflow
	VC,	//No Overflow
	HI,	//Unsigned Higher
	LS,	//Unsigned Lower or Same
	GE,	//Greater than or Equal (signed)
	LT,	//Less Than (signed)
	GT,	//Greater Than (signed)
	LE	//Less than or Equal (signed)
};

union floatRegister {
	double d;
	float f;
};

class Core {
	public:
		Core(bool coreID, CPU *cpu);

		void fetch();
		void execute();
		bool checkCondition(uint8_t conditionCode) const;

		uint64_t getIntRegister(uint8_t reg) const;
		void setIntRegister(uint8_t reg, uint64_t value);
		floatRegister getFloatRegister(uint8_t reg) const;
		void setFloatRegister(uint8_t reg, floatRegister value);

		void extractAddress(uint32_t address, uint8_t &index, uint32_t &tag, uint8_t &offset) const;
		bool cacheLookup(uint32_t address, const uint8_t *valuePointer) const;

		uint64_t loadInt(uint32_t address, uint8_t length, bool isSigned) const;
		void storeInt(uint32_t address, uint64_t value, uint8_t length);
		floatRegister loadFloat(uint32_t address, bool isDouble) const;
		void storefloat(uint32_t address, floatRegister value, bool isDouble);

		bool isHalted();

	private:
		CPU* cpu = nullptr;
		ALU alu;

		struct Registers {
			uint32_t programCounter;

			int64_t intDataRegisters[8];
			floatRegister floatDataRegisters[8];
			double matrixRegistres[2][2][2];
			uint32_t addressRegisters[7];
			uint32_t callStackPointer;

			uint8_t flagsRegister;
			uint32_t instructionRegister;
			uint16_t interruptEnableRegister;
			bool busLockRegister;
			const bool coreIDRegister;

			Registers(bool coreID) : coreIDRegister(coreID) {}
		};
		Registers registers;

		struct L1CacheBlock {
			uint32_t tag;
			uint8_t data[64];
			bool flag = 0;
		};
		L1CacheBlock cache[32][4];

		bool halted = false;
};

#include "CPU.h" //Prevent circular includes