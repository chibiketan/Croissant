
#include "stdafx.hpp"
#include "Core/Rand.hpp"
#include <stdlib.h>
#include <xmmintrin.h>

#define rotl(r,n) (((r)<<(n)) | ((r)>>((8*sizeof(r))-(n))))
#define FIRST_BIT_MASK 0x80000000
#define ALL_WITHOUT_FIRST_BIT_MASK 0x7FFFFFFF

Croissant::Core::Rand2CmrRsr::Rand2CmrRsr(void) : x(0), y(0), z(0)
{
	Reseed(0);
}

Croissant::Core::Rand2CmrRsr::Rand2CmrRsr(uint32_t seed) : x(0), y(0), z(0)
{
	Reseed(seed);
}

void Croissant::Core::Rand2CmrRsr::Reseed(uint32_t seed)
{
	x = (seed >> 16)    + 4125832013u; // upper 16 bits + offset
	y = (seed & 0xffff) +  814584116u; // lower 16 bits + offset
	z = 542;
}

uint32_t Croissant::Core::Rand2CmrRsr::GetNext()
{
	x *=  255519323u; x = rotl(x,13); // CMR, period = 4294785923 (prime)
	y *= 3166389663u; y = rotl(y,17); // CMR, period = 4294315741 (prime)
	z -= rotl(z,11);  z = rotl(z,27); // RSR, period = 253691 = 2^3*3^2*71*557
	return x ^ y ^ z;
}

// ------------------------------------------------------------ Twister

Croissant::Core::RandTwister::RandTwister(void) : m_currentState(0)
{
	Reseed(0);
}

Croissant::Core::RandTwister::RandTwister(uint32_t seed) : m_currentState(0)
{
	Reseed(seed);
}

#define RSHIFT(num, size) (num >> size)
#define LSHIFT(num, size) (num << size)

void Croissant::Core::RandTwister::Reseed(uint32_t seed)
{
	/*
	MT[0] := seed
	for i from 1 to 623 { // loop over each other element
	MT[i] := last 32 bits of(1812433253 * (MT[i-1] xor (right shift by 30 bits(MT[i-1]))) + i) // 0x6c078965
	*/
	m_currentState = TWISTER_STATES_COUNT;
	m_states[0] = seed;
	uint32_t tmp = seed;

	for (int i = 1; i < TWISTER_STATES_COUNT; ++i)
	{
		m_states[i] = tmp = (1812433253 * (tmp ^ RSHIFT(tmp, 30)) + i) & 0xFFFFFFFF;

	}
}

uint32_t Croissant::Core::RandTwister::GetNext()
{
	/*
	if index == 0 {
	generate_numbers()
	}

	int y := MT[index]
	y := y xor (right shift by 11 bits(y))
	y := y xor (left shift by 7 bits(y) and (2636928640)) // 0x9d2c5680
	y := y xor (left shift by 15 bits(y) and (4022730752)) // 0xefc60000
	y := y xor (right shift by 18 bits(y))

	index := (index + 1) mod 624
	return y
	*/
	if (m_currentState >= TWISTER_STATES_COUNT)
	{
		GenerateNumbers();
		m_currentState = 0;
	}

	uint32_t y = m_states[m_currentState++];
	y = y ^ RSHIFT(y, 11);
	y = y ^(LSHIFT(y, 7) & 0x9d2c5680);
	y = y ^(LSHIFT(y, 15) & 0xefc60000);
	y = y ^ RSHIFT(y, 18);

	return y;
}

void Croissant::Core::RandTwister::GenerateNumbers()
{
	static uint32_t magic[] = { 0x0, 0x9908b0df };
	/*
	for i from 0 to 623 {
		int y := 32nd bit of(MT[i]) + last 31 bits of(MT[(i+1) mod 624])
		MT[i] := MT[(i + 397) mod 624] xor (right shift by 1 bit(y))
		if (y mod 2) != 0 { // y is odd
			MT[i] := MT[i] xor (2567483615) // 0x9908b0df
		}
	}
	*/
	
	int diff = TWISTER_STATES_COUNT - TWISTER_HALF_STATES_COUNT;
	int i = 0;

	for (i = 0; i < diff; ++i)
	{
		uint32_t y = (m_states[i] & FIRST_BIT_MASK) | (m_states[i + 1] & ALL_WITHOUT_FIRST_BIT_MASK);
		m_states[i] = m_states[(i + TWISTER_HALF_STATES_COUNT)] ^ RSHIFT(y, 1) ^ magic[y & 0x1];
	}

	for (; i < TWISTER_STATES_COUNT - 1; ++i)
	{
		uint32_t y = (m_states[i] & FIRST_BIT_MASK) | (m_states[i + 1] & ALL_WITHOUT_FIRST_BIT_MASK);
		m_states[i] = m_states[i + (TWISTER_HALF_STATES_COUNT - TWISTER_STATES_COUNT)] ^ RSHIFT(y, 1) ^ magic[y & 0x1];
	}

	uint32_t y = (m_states[i] & FIRST_BIT_MASK) | (m_states[0] & ALL_WITHOUT_FIRST_BIT_MASK);
	m_states[i] = m_states[TWISTER_STATES_COUNT - 1] ^ RSHIFT(y, 1) ^ magic[y & 0x1];

	//for (int i = 0; i < TWISTER_STATES_COUNT; ++i)
	//{
	//	uint32_t y = (m_states[i] & FIRST_BIT_MASK) | (m_states[(i + 1)% TWISTER_STATES_COUNT] & ALL_WITHOUT_FIRST_BIT_MASK);
	//	m_states[i] = m_states[(i + 397) % TWISTER_STATES_COUNT] ^ RSHIFT(y, 1);
	//	if ((y % 2) != 0)
	//	{
	//		m_states[i] = m_states[i] ^ 0x9908b0df;
	//	}
	//}
}

// ------------------------------------------------------------ System

Croissant::Core::RandSystem::RandSystem(void)
{
	Reseed(0);
}

Croissant::Core::RandSystem::RandSystem(uint32_t seed)
{
	Reseed(seed);
}

void Croissant::Core::RandSystem::Reseed(uint32_t seed)
{
	srand(seed);
}

uint32_t Croissant::Core::RandSystem::GetNext()
{
	return (uint32_t)rand();
}
