#include "Core.h"

//Define a core with its ID
Core::Core(bool coreID, CPU *cpu) :
	registers(coreID), cpu(cpu), alu(registers.flagsRegister)
{
	
}

//Fetch the next instruction and update the program counter
void Core::fetch()
{
	registers.instructionRegister = (uint32_t)loadInt(registers.programCounter++, 0b10, false);
}

//Executes an instruction stored in an unsigned integer
void Core::execute() {
	uint32_t instruction = registers.instructionRegister;

	uint8_t opcode = (uint8_t)Logic::extractBitsMsb(instruction, 0, 5);
	if (opcode == ADDcc || //If instruction is conditional
		opcode == SUBcc ||
		opcode == ANDcc ||
		opcode == ORcc  ||
		opcode == Bcc   ||
		opcode == STcc  ||
		opcode == LDcc
	) {
		if (checkCondition((uint8_t)Logic::extractBitsLsb(instruction, 0, 4))) return;
		opcode--; //The unconditional instruction is always at the previous index in the enum
	}

	uint8_t reg1 = (uint8_t)Logic::extractBitsMsb(instruction, 5, 5); //Get every register address
	uint8_t reg2 = (uint8_t)Logic::extractBitsMsb(instruction, 10, 5);
	uint8_t reg3 = (uint8_t)Logic::extractBitsMsb(instruction, 15, 5);

	bool isFloat = instruction & Logic::getNthBitMask(5); //Get every flags and values
	bool isSigned = instruction & Logic::getNthBitMask(6);
	uint8_t dataSize = (uint8_t)Logic::extractBitsLsb(instruction, 7, 2);

	switch (opcode) {
		//Basic Arithmetic Operations
		case ADD:
			if (!isFloat && isSigned) setIntRegister(reg1, alu.ADD((int64_t)getIntRegister(reg2), (int64_t)getIntRegister(reg3)));
			else if (!isFloat) setIntRegister(reg1, alu.ADD(getIntRegister(reg2), getIntRegister(reg3)));
			else {
				if (dataSize == 0b11) registers.floatDataRegisters[reg1].d = alu.ADD(getFloatRegister(reg2).d, getFloatRegister(reg3).d);
				else registers.floatDataRegisters[reg1].f = alu.ADD(getFloatRegister(reg2).f, getFloatRegister(reg3).f);
			}
			break;

		case SUB:
			if (!isFloat) setIntRegister(reg1, alu.SUB(getIntRegister(reg2), getIntRegister(reg3)));
			else {
				if (dataSize == 0b11) registers.floatDataRegisters[reg1].d = alu.SUB(getFloatRegister(reg2).d, getFloatRegister(reg3).d);
				else registers.floatDataRegisters[reg1].f = alu.SUB(getFloatRegister(reg2).f, getFloatRegister(reg3).f);
			}
			break;

		case MUL:
			if (!isFloat && isSigned) setIntRegister(reg1, alu.MUL((int64_t)getIntRegister(reg2), (int64_t)getIntRegister(reg3)));
			else if (!isFloat) setIntRegister(reg1, alu.MUL(getIntRegister(reg2), getIntRegister(reg3)));
			else {
				if (dataSize == 0b11) registers.floatDataRegisters[reg1].d = alu.MUL(getFloatRegister(reg2).d, getFloatRegister(reg3).d);
				else registers.floatDataRegisters[reg1].f = alu.MUL(getFloatRegister(reg2).f, getFloatRegister(reg3).f);
			}
			break;

		case DIV:
			if (!isFloat) setIntRegister(reg1, alu.DIV(getIntRegister(reg2), getIntRegister(reg3)));
			else {
				if (dataSize == 0b11) registers.floatDataRegisters[reg1].d = alu.DIV(getFloatRegister(reg2).d, getFloatRegister(reg3).d);
				else registers.floatDataRegisters[reg1].f = alu.DIV(getFloatRegister(reg2).f, getFloatRegister(reg3).f);
			}
			break;

		//Logic Operations
		case AND:
			setIntRegister(reg1, getIntRegister(reg2) & getIntRegister(reg3));
			break;

		case OR:
			setIntRegister(reg1, getIntRegister(reg2) | getIntRegister(reg3));
			break;

		case XOR:
			setIntRegister(reg1, getIntRegister(reg2) ^ getIntRegister(reg3));
			break;

		case NOT:
			setIntRegister(reg1, ~getIntRegister(reg2));
			break;

		case LSL:
			if (!isSigned) setIntRegister(reg1, getIntRegister(reg2) << getIntRegister(reg3));
			break;

		case LSR:
			setIntRegister(reg1, getIntRegister(reg2) >> getIntRegister(reg3));
			break;

		case ROL: {
			uint64_t d = getIntRegister(reg3) % (sizeof(int64_t) * 8);
			setIntRegister(reg1, (getIntRegister(reg2) << d) | (getIntRegister(reg2) >> ((sizeof(int64_t) * 8) - d)));
			break; }

		case ROR: {
			uint64_t d = getIntRegister(reg3) % (sizeof(int64_t) * 8);
			setIntRegister(reg1, (getIntRegister(reg2) >> d) | (getIntRegister(reg2) << ((sizeof(int64_t) * 8) - d)));
			break; }

		//Flow Control
		case JMP:
			registers.programCounter = registers.addressRegisters[reg1];
			break;

		case JSR:
			registers.callStackPointer -= sizeof(uint32_t);
			storeInt(registers.callStackPointer, registers.programCounter, 0b10);
			registers.programCounter = registers.addressRegisters[reg1];
			break;

		case RTS:
			registers.programCounter = (uint32_t)loadInt(registers.callStackPointer, 0b10, false);
			registers.callStackPointer += sizeof(uint32_t);
			break;

		//Memory Access
		case LD:
			if (isFloat) setFloatRegister(reg2, loadFloat(registers.addressRegisters[reg1], dataSize == 0b11));
			else setIntRegister(reg2, loadInt(registers.addressRegisters[reg1], dataSize, isSigned));
			break;

		case ST:
			if (isFloat) storefloat(registers.addressRegisters[reg1], getFloatRegister(reg2), dataSize == 0b11);
			else storeInt(registers.addressRegisters[reg1], getIntRegister(reg2), dataSize);
			break;

		case PUSH:
			registers.addressRegisters[6] -= sizeof(uint64_t);
			if (isFloat) storefloat(registers.addressRegisters[reg1], getFloatRegister(reg2), dataSize == 0b11);
			else storeInt(registers.addressRegisters[reg1], getIntRegister(reg2), dataSize);
			break;

		case POP:
			if (isFloat) setFloatRegister(reg2, loadFloat(registers.addressRegisters[reg1], dataSize == 0b11));
			else setIntRegister(reg2, loadInt(registers.addressRegisters[reg1], dataSize, isSigned));
			registers.addressRegisters[6] += sizeof(uint64_t);
			break;
		
		//Special Operations
		case MATMUL:
			alu.MATMUL(registers.matrixRegistres[0], registers.matrixRegistres[1], registers.matrixRegistres[0]);
			break;

		//Miscellaneous
		case NOP:
			break;

		case HALT:
			halted = true;
			break;

		case MOV:
			if (!isFloat) setIntRegister(reg1, getIntRegister(reg2));
			else setFloatRegister(reg1, getFloatRegister(reg2));
			break;

		case SETI:
			setIntRegister(reg1, instruction & Logic::getBitMask(22));
			break;

		case FETCH:
			cpu->fetch(registers.addressRegisters[reg1], registers.addressRegisters[reg2], getIntRegister(reg3));
			break;
	}
}

//Returns true if the condition given in parameter is respected otherwise false
bool Core::checkCondition(uint8_t conditionCode) const
{
	uint8_t flags = registers.flagsRegister;
	switch (conditionCode) {
		case EQ: return (flags & Z) != 0;
		case NE: return (flags & Z) == 0;
		case CS: return (flags & C) != 0;
		case CC: return (flags & C) == 0;
		case MI: return (flags & N) != 0;
		case PL: return (flags & N) == 0;
		case VS: return (flags & V) != 0;
		case VC: return (flags & V) == 0;
		case HI: return ((flags & C) != 0) && ((flags & Z) == 0);
		case LS: return ((flags & C) == 0) || ((flags & Z) != 0);
		case GE: return ((flags & N) >> 2) == ((flags & V) >> 3);
		case LT: return ((flags & N) >> 2) != ((flags & V) >> 3);
		case GT: return ((flags & Z) == 0) && (((flags & N) >> 2) == ((flags & V) >> 3));
		case LE: return ((flags & Z) != 0) || (((flags & N) >> 2) != ((flags & V) >> 3));
		default: return false;
	}
}

//Return the contents of a integer register from an address
uint64_t Core::getIntRegister(uint8_t reg) const {
	if (reg < 8) {
		return registers.intDataRegisters[reg];
	}
	else if (reg < 15) {
		return registers.addressRegisters[reg - 8];
	}
	else {
		return registers.interruptEnableRegister;
	}
}

//Update the contents of a integer register from an address
void Core::setIntRegister(uint8_t reg, uint64_t value) {
	if (reg < 8) {
		registers.intDataRegisters[reg] = value;
	}
	else if (reg < 15) {
		registers.addressRegisters[reg - 8] = (uint32_t)value;
	}
	else {
		registers.interruptEnableRegister = (uint16_t)value;
	}
}

//Return the contents of a floating register from an address
floatRegister Core::getFloatRegister(uint8_t reg) const {
	if (reg < 8) {
		return registers.floatDataRegisters[reg];
	}
	throw 1;
}

//Update the contents of a floating register from an address
void Core::setFloatRegister(uint8_t reg, floatRegister value) {
	if (reg < 8) {
		registers.floatDataRegisters[reg] = value;
	}
	throw 2;
}

//Function to extract index, tag, and offset from an address
void Core::extractAddress(uint32_t address, uint8_t &index, uint32_t &tag, uint8_t &offset) const {
	// Depending on your layout you'll need to change this
	offset = address & 0x3F; // 6 bits for the offset within a 64-byte block
	index = (address >> 6) & 0x7F; // 7 bits for index
	tag = address >> 13; // Rest of the bits are for the tag
}

//Returns whether an address is cached and a pointer to its first byte
bool Core::cacheLookup(uint32_t address, const uint8_t *valuePointer) const {
	uint8_t index, offset;
	uint32_t tag;
	extractAddress(address, index, tag, offset);

	for (int i = 0; i < 4; ++i) {
		if (cache[index][i].flag && cache[index][i].tag == tag) {
			// Cache hit
			valuePointer = &(cache[index][i].data[offset]);
			return true;
		}
	}

	// Cache miss
	return false;
}

//Load an int from memory or cache
uint64_t Core::loadInt(uint32_t address, uint8_t length, bool isSigned) const
{
	uint8_t *a = nullptr;
	if (!cacheLookup(address, a)) a = cpu->load(address);

	if (!isSigned) {
		uint64_t t;
		memcpy(&t, a, sizeof(uint8_t) * (length + 1));
		return t;
	}

	switch (length) { //Preserve sign bit
		case 0: {
			return (uint64_t)(int64_t)*a;
		}

		case 1: {
			int16_t t;
			memcpy(&t, a, sizeof(t));
			return (uint64_t)(int64_t)t;
		}
		
		case 2: {
			int32_t t;
			memcpy(&t, a, sizeof(t));
			return (uint64_t)(int64_t)t;
		}
		
		default: {
			int64_t t;
			memcpy(&t, a, sizeof(t));
			return (uint64_t)t;
		}
	}
}

//Load a float from memory or cache
floatRegister Core::loadFloat(uint32_t address, bool isDouble) const
{
	floatRegister r;
	uint8_t *a = nullptr;
	if (!cacheLookup(address, a)) a = cpu->load(address);

	if (isDouble) {
		double t;
		memcpy(&t, a, sizeof(double));
		r.d = t;
		return r;
	}

	float t;
	memcpy(&t, a, sizeof(float));
	r.f = t;
	return r;
}

//Store an int in memory or cache
void Core::storeInt(uint32_t address, uint64_t value, uint8_t length)
{
	uint8_t size = sizeof(uint8_t) * (length + 1);
	uint8_t *a = nullptr;
	if (cacheLookup(address, a)) {
		memcpy(a, &value, size);
	}
	
	a = new uint8_t[size];

	memcpy(a, &value, size);
	cpu->store(address, a, size);

	delete[] a;
}

//Store a float in memory or cache
void Core::storefloat(uint32_t address, floatRegister value, bool isDouble)
{
	uint8_t *a = nullptr;
	if (cacheLookup(address, a)) {
		if (isDouble) {
			memcpy(a, &value.d, sizeof(double));
		}
		else {
			memcpy(a, &value.f, sizeof(float));
		}
	}

	uint8_t size = isDouble ? sizeof(uint64_t) : sizeof(uint32_t);
	a = new uint8_t[size];

	if (isDouble) {
		memcpy(a, &value.d, sizeof(double));
	} else {
		memcpy(a, &value.f, sizeof(float));
	}
	cpu->store(address, a, size);

	delete[] a;
}

// Returns whether the core is halted or not
bool Core::isHalted()
{
	return halted;
}
