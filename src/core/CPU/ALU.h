#pragma once

#include "../../Logic.h"

#define MSB 0x8000000000000000

enum Flag {
	Z = 1 << 0,	// Zero Flag
	C = 1 << 1,	// Carry Flag
	N = 1 << 2,	// Negative Flag
	V = 1 << 3	// Overflow Flag
};

class ALU {
	public:
		ALU(uint8_t &flagRegister);

		uint64_t ADD(uint64_t a, uint64_t b) const;
		uint64_t ADD(int64_t a, int64_t b) const;
		double ADD(double a, double b) const;
		float ADD(float a, float b) const;

		uint64_t SUB(uint64_t a, uint64_t b) const;
		double SUB(double a, double b) const;
		float SUB(float a, float b) const;

		uint64_t MUL(uint64_t a, uint64_t b) const;
		uint64_t MUL(int64_t a, int64_t b) const;
		double MUL(double a, double b) const;
		float MUL(float a, float b) const;

		uint64_t DIV(uint64_t a, uint64_t b) const;
		double DIV(double a, double b) const;
		float DIV(float a, float b) const;

		void MATMUL(double N[2][2], double M[2][2], double C[2][2]);

	private:
		uint8_t &flags;
};