#ifndef __RODRIK_BLOCK_CHAIN_H__
#define __RODRIK_BLOCK_CHAIN_H__

#include <Tools/Hash/SHA256.h>
#include <sstream>
#include <time.h>
#include <string>
#include <array>

namespace Hash
{
class SHA256;
}

template <typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const std::array<T, N>& arr)
{
	for (const auto& character : arr)
	{
		os << character;
	}

	return os;
}

namespace Core
{
class Block
{
public:
	Block(std::string data);
	Block(const Block& other);
	Block(Block&& other) noexcept;
	~Block() = default;

public:
	Block& operator=(const Block& other);
	Block& operator=(Block&& other) noexcept;

public:
	inline auto& GetHash() { return this->hash_; }
	inline const auto& GetHash() const { return this->hash_; }
	inline auto& GetPreviousHash() { return this->block_contents_.previous_hash_; }
	inline const auto& GetPreviousHash() const { return this->block_contents_.previous_hash_; }
	inline auto& GetData() { return this->block_contents_.data_; }
	inline const auto& GetData() const { return this->block_contents_.data_; }
	inline const auto& GetBlockContents() const { return this->block_contents_; }
	void Mine(std::size_t difficulty);

private:
	struct BlockContents
	{
	public:
		BlockContents(std::string data);
		BlockContents(const BlockContents& other);
		BlockContents(BlockContents&& other) noexcept;
		~BlockContents() = default;

	public:
		BlockContents& operator=(const BlockContents& other);
		BlockContents& operator=(BlockContents&& other) noexcept;

	public:
		std::array<int, 32 / sizeof(int)> random_{};
		std::array<unsigned char, 2 * 32> previous_hash_{};
		std::string						  data_{};
		time_t							  time_{};
		std::uint64_t					  proof_of_work_{};
	} block_contents_;

	std::array<unsigned char, 2 * 32> hash_{};

private:
	friend void Digest(Hash::SHA256& context, const Core::Block& block)
	{
		std::stringstream string_stream{};
		string_stream << block.GetBlockContents().data_
					  << block.GetBlockContents().previous_hash_
					  << block.GetBlockContents().proof_of_work_
					  << block.GetBlockContents().random_
					  << block.GetBlockContents().time_;

		context.CalculateDigest(reinterpret_cast<const unsigned char*>(string_stream.str().c_str()), string_stream.str().size());
	}

private:
	friend class Blockchain;
};
} // namespace Core

#endif // __RODRIK_BLOCK_CHAIN_H__