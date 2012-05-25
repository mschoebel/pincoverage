
#pragma once

#include <string>

class FizzBuzz
{
public:

	static const std::string FIZZ;
	static const std::string BUZZ;

	FizzBuzz();

	void sayFizzBuzz(int limit) const;

	void sayFizz(int limit) const;
	void sayBuzz(int limit) const;
};
