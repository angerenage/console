#include "CPU.h"

CPU::CPU() :
	core0(0, this), core1(1, this)
{
	
}

uint8_t *CPU::load(uint32_t address)
{
	return nullptr;
}

void CPU::store(uint32_t address, uint8_t *value, uint8_t length)
{
	
}

void CPU::fetch(uint32_t dst, uint32_t src, uint64_t size)
{

}