
#include "test.h"

#include <iostream>

const std::string FizzBuzz::FIZZ = "fizz";
const std::string FizzBuzz::BUZZ = "buzz";

FizzBuzz::FizzBuzz()
{
}

void FizzBuzz::sayFizzBuzz(int limit) const
{
	for (int i=1; i<limit; ++i)
	{
		if ((i%3 == 0) && (i%5 == 0))
		{
			std::cout << FIZZ << BUZZ << std::endl;
			continue;
		}
		if (i%3 == 0)
		{
			std::cout << FIZZ << std::endl;
			continue;
		}
		if (i%5 == 0)
		{
			std::cout << BUZZ << std::endl;
			continue;
		}

		std::cout << i << std::endl;
	}
}

void FizzBuzz::sayFizz(int limit) const
{
	for (int i=1; i<limit; ++i)
	{
		if (i%3 == 0)
		{
			std::cout << FIZZ << std::endl;
			continue;
		}

		std::cout << i << std::endl;
	}
}

void FizzBuzz::sayBuzz(int limit) const
{
	for (int i=1; i<limit; ++i)
	{
		if (i%5 == 0)
		{
			std::cout << BUZZ << std::endl;
			continue;
		}

		std::cout << i << std::endl;
	}
}
