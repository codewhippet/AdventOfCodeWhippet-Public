#include "stdafx.h"
#include "MD5.h"
#if _WIN32
#include <Windows.h>
#include <bcrypt.h>
#endif
#include <cassert>

MD5::MD5()
{
#if _WIN32
	NTSTATUS createAlgo = BCryptOpenAlgorithmProvider(&Algorithm,
		BCRYPT_MD5_ALGORITHM,
		NULL,
		0);
	assert(createAlgo == 0);
	(void)createAlgo;
#endif
}

MD5::~MD5()
{
#if _WIN32
	if (Algorithm)
	{
		NTSTATUS closeAlgo = BCryptCloseAlgorithmProvider(Algorithm, 0);
		assert(closeAlgo == 0);
		(void)closeAlgo;
	}
#endif
}

bool MD5::Hash(const uint8_t* buffer, size_t bufferSize, uint8_t hash[16]) const
{
#if _WIN32
	NTSTATUS hashedOkay = BCryptHash(Algorithm,
		NULL,
		0,
		(PUCHAR)buffer,
		(ULONG)bufferSize,
		(PUCHAR)hash,
		16);
	assert(hashedOkay == 0);
	return (hashedOkay == 0);
#else
	return false;
#endif
}

bool MD5::Hash(const std::string& s, uint8_t hash[16]) const
{
	return Hash(reinterpret_cast<const uint8_t*>(s.data()), s.size(), hash);
}
