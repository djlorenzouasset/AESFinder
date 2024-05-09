#pragma once
#include "globals.h"

struct FAESKey
{
private:
	static constexpr uint32 KeySize = 32;

public:
	uint8 KeyBytes[KeySize];

	FAESKey() : KeyBytes{ 0 }
	{
	}

	/*
	Converts a FAESKey to std::string.
	Copied from https://github.com/ramok0/PlataniumV3/blob/main/PlataniumV3/src/aeskey.cpp#L3 
	*/
	inline std::string ToString() const
	{
		std::stringstream s;
		s << "0x" << std::hex << std::uppercase << std::setfill('0');

		for (int i = 0; i < KeySize; i++)
		{
			s << std::setw(2) << static_cast<int>(KeyBytes[i]);
		}

		return s.str();
	}
};