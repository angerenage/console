#include "ALU.h"

ALU::ALU(uint8_t &flagRegister) :
	flags(flagRegister)
{
}

uint64_t ALU::ADD(uint64_t a, uint64_t b) const
{
	flags &= 0;
	uint64_t result = a + b;

	// Updating flags
	if (result == 0) flags |= Z;
	if ((int64_t)result < 0) flags |= N;
	if (result < a) flags |= C;
	if ((a & MSB) == (b & MSB) && (result & MSB) != (a & MSB)) {
		flags |= V;
	}

	return result;
}

uint64_t ALU::ADD(int64_t a, int64_t b) const
{
	flags &= 0;
    int64_t result = a + b;

    // Updating flags
    if (result == 0) flags |= Z;
    if (result < 0) flags |= N;
    if (((a > 0) && (b > 0) && (result <= 0)) || 
        ((a < 0) && (b < 0) && (result >= 0))) {
        flags |= V;
    }

    return (uint64_t)result;
}

double ALU::ADD(double a, double b) const
{
	flags &= 0;
	double result = a + b;

	// Updating flags
	if (result == 0.0) flags |= Z;
    if (result < 0.0) flags |= N;

	return result;
}

float ALU::ADD(float a, float b) const
{
	flags &= 0;
	float result = a + b;

	// Updating flags
	if (result == 0.0) flags |= Z;
    if (result < 0.0) flags |= N;

	return result;
}

uint64_t ALU::SUB(uint64_t a, uint64_t b) const
{
	flags &= 0;
	uint64_t result = a - b;

    // Updating flags
    if (result == 0) flags |= Z;
    if ((int64_t)result < 0) flags |= N;
    if (a < b) flags |= C;
    if ((a & MSB) != (b & MSB) && (result & MSB) != (a & MSB)) {
        flags |= V;
    }

    return result;
}

double ALU::SUB(double a, double b) const
{
	flags &= 0;
	double result = a - b;

    // Updating flags
    if (result == 0.0) flags |= Z;
    if (result < 0.0) flags |= N;

    return result;
}

float ALU::SUB(float a, float b) const
{
	flags &= 0;
	float result = a - b;

    // Updating flags
    if (result == 0.0) flags |= Z;
    if (result < 0.0) flags |= N;

    return result;
}

uint64_t ALU::MUL(uint64_t a, uint64_t b) const
{
	flags &= 0;
    uint64_t result = a * b;

    // Updating flags
    if (result == 0) flags |= Z;
    if ((int64_t)result < 0) flags |= N;
    if (a != 0 && result / a != b) flags |= V;

    return result;
}

uint64_t ALU::MUL(int64_t a, int64_t b) const
{
	flags &= 0;
    int64_t result = a * b;

    // Updating flags
    if (result == 0) flags |= Z;
    if (result < 0) flags |= N;
    if (a != 0 && result / a != b) flags |= V;

    return (uint64_t)result;
}

double ALU::MUL(double a, double b) const
{
	flags &= 0;
    double result = a * b;

    // Updating flags
    if (result == 0.0) flags |= Z;
    if (result < 0.0) flags |= N;

    return result;
}

float ALU::MUL(float a, float b) const
{
	flags &= 0;
    float result = a * b;

    // Updating flags
    if (result == 0.0) flags |= Z;
    if (result < 0.0) flags |= N;

    return result;
}

uint64_t ALU::DIV(uint64_t a, uint64_t b) const
{
	if (b == 0) {
        // Handle division by zero, if necessary
        return 0;
    }

	flags &= 0;
    uint64_t result = a / b;

    // Updating flags
    if (result == 0) flags |= Z;
    if ((int64_t)result < 0) flags |= N;

    return result;
}

double ALU::DIV(double a, double b) const
{
    if (b == 0.0) {
        // Handle division by zero, if necessary
        return 0.0;
    }

	flags &= 0;
    double result = a / b;

    // Updating flags
    if (result == 0.0) flags |= Z;
    if (result < 0.0) flags |= N;

    return result;
}

float ALU::DIV(float a, float b) const
{
    if (b == 0.0) {
        // Handle division by zero, if necessary
        return 0.0;
    }

	flags &= 0;
    float result = a / b;

    // Updating flags
    if (result == 0.0) flags |= Z;
    if (result < 0.0) flags |= N;

    return result;
}

void ALU::MATMUL(double N[2][2], double M[2][2], double C[2][2])
{
	double R[2][2]; //Can surely be optimized but stay like this for now
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			double sum = 0.0f;
			for (int k = 0; k < 2; k++) {
				sum += N[i][k] * M[k][j];
			}
			R[i][j] = sum;
		}
	}

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			C[i][j] = R[i][j];
		}
	}
}