#include "Logic.h"

namespace Logic {
	uint64_t getNthBitMask(uint8_t n) {
		return 1i64 << n;
	}

	uint64_t getBitMask(uint8_t length) {
		return ~((uint64_t)-1 << (uint64_t)length);
	}

	uint64_t extractBitsMsb(uint64_t inputNumber, uint8_t msbOffset, uint8_t length) {
		return (inputNumber >> (sizeof(inputNumber) - msbOffset)) & getBitMask(length);
	}

	uint64_t extractBitsLsb(uint64_t inputNumber, uint8_t lsbOffset, uint8_t length) {
		return (inputNumber >> lsbOffset) & getBitMask(length);
	}
}