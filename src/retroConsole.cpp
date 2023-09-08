#include <iostream>

#include "core/CPU/CPU.h"

int main() {
	static_assert(sizeof(float) == 4, "float must be 32 bits");
	static_assert(sizeof(double) == 8, "double must be 64 bits");
	
	//CPU cpu();

	return 0;
}