#ifndef RAND_H_
#define RAND_H_
#pragma once

#include <stdint.h>
#include "Engine.hpp"

#define TWISTER_STATES_COUNT 624
#define TWISTER_HALF_STATES_COUNT 397

namespace Croissant
{
	namespace Core
	{
		// thanks to http://drdobbs.com/tools/229625477?pgno=1
		class ENGINE_API Rand2CmrRsr
		{
		public:
			Rand2CmrRsr();
			Rand2CmrRsr(uint32_t seed);
			//Rand(const Rand& o);
			void Reseed(uint32_t seed);
			uint32_t GetNext();

		private:
			uint32_t x;
			uint32_t y;
			uint32_t z;
		};


		// thanks to http://en.wikipedia.org/wiki/Mersenne_twister
		class ENGINE_API RandTwister
		{
		public:
			RandTwister();
			RandTwister(uint32_t seed);
			//Rand(const Rand& o);
			void Reseed(uint32_t seed);
			uint32_t GetNext();

		private:
			void GenerateNumbers();

			uint32_t m_currentState;
			uint32_t m_states[TWISTER_STATES_COUNT];
		};

		// for test purpose
		class ENGINE_API RandSystem
		{
		public:
			RandSystem();
			RandSystem(uint32_t seed);
			//Rand(const Rand& o);
			void Reseed(uint32_t seed);
			uint32_t GetNext();
		};



		//typedef RandTwister Rand;
		typedef Rand2CmrRsr Rand;
	}
}

#endif
