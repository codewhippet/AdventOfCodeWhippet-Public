#pragma once

#include <stdint.h>

class MD5
{
public:

	struct Hash
	{
		uint8_t Data[16];
	};

	MD5();
	MD5(const MD5&) = delete;
	MD5(const MD5&&) = delete;
	~MD5();

	void operator==(const MD5&) = delete;
	void operator==(const MD5&&) = delete;

	bool Hash(const uint8_t* buffer, size_t bufferSize, uint8_t hash[16]) const;
	bool Hash(const std::string &s, uint8_t hash[16]) const;

private:

	void* Algorithm = 0;

};
