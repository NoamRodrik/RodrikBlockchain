// ///////////////////////////////////////
// sha256.h
// Copyright (c) 2014,2015 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
// Altered by Noam Rodrik
//
#ifndef __SHA256_H__
#define __SHA256_H__

#include <stdint.h>
#include <array>

namespace Hash
{
class SHA256
{
public:
	// split into 64 byte blocks (=> 512 bits), hash is 32 bytes long
	static constexpr auto BLOCK_SIZE{512 / 8};
	static constexpr auto HASH_BYTES{32};

public:
	template <typename T>
	SHA256(const T& object)
	{
		this->Reset();
		Digest(*this, object);
	}

	// return latest hash as bytes
	void FillHash(std::array<unsigned char, 2 * HASH_BYTES>& hash_container);

	// restart
	void Reset();

	// Add arbitrary number of bytes
	void CalculateDigest(const unsigned char* data, size_t numBytes);
	
private:
	// process 64 bytes
	void ProcessBlock(const void* data);

	// process everything left in the internal buffer
	void ProcessBuffer();

private:
	static constexpr auto HASH_VALUES{HASH_BYTES / 4};

private:
	// size of processed data in bytes
	uint64_t bytes_{};

	// valid bytes in m_buffer
	size_t   buffer_size_{};

	// bytes not processed yet
	uint8_t  buffer_[BLOCK_SIZE]{};
	
	// hash, stored as integers
	uint32_t hash_[HASH_VALUES]{};
};
}

#endif // __SHA256_H__