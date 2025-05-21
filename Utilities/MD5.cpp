#include "stdafx.h"
#include "MD5.h"
#include <Windows.h>
#include <bcrypt.h>
#include <cassert>

MD5::MD5()
{
	NTSTATUS createAlgo = BCryptOpenAlgorithmProvider(&Algorithm,
		BCRYPT_MD5_ALGORITHM,
		NULL,
		0);
	assert(createAlgo == 0);
	(void)createAlgo;
}

MD5::~MD5()
{
	if (Algorithm)
	{
		NTSTATUS closeAlgo = BCryptCloseAlgorithmProvider(Algorithm, 0);
		assert(closeAlgo == 0);
		(void)closeAlgo;
	}
}

bool MD5::Hash(const uint8_t* buffer, size_t bufferSize, uint8_t hash[16]) const
{
	NTSTATUS hashedOkay = BCryptHash(Algorithm,
		NULL,
		0,
		(PUCHAR)buffer,
		(ULONG)bufferSize,
		(PUCHAR)hash,
		16);
	assert(hashedOkay == 0);
	return (hashedOkay == 0);
}

bool MD5::Hash(const std::string& s, uint8_t hash[16]) const
{
	return Hash(reinterpret_cast<const uint8_t*>(s.data()), s.size(), hash);
}
