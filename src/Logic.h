#pragma once

#include <cstdint>

namespace Logic {
	uint64_t getNthBitMask(uint8_t n);
	uint64_t getBitMask(uint8_t length);

	uint64_t extractBitsMsb(uint64_t inputNumber, uint8_t msbOffset, uint8_t length);
	uint64_t extractBitsLsb(uint64_t inputNumber, uint8_t lsbOffset, uint8_t length);
}