#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "Core.h"

class CPU {
	public:
		CPU();

		uint8_t *load(uint32_t address);
		void store(uint32_t address, uint8_t *value, uint8_t length);

		void fetch(uint32_t dst, uint32_t src, uint64_t size);

	private:
		Core core0, core1;
};