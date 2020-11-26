#include "Block.h"

#include <algorithm>
#include <cstdlib>
#include <memory>
#include <vector>

using namespace Core;

namespace
{
template <typename T1, typename T2>
void randomize(T1 t1, T2 t2)
{
	std::fill(t1, t2, std::rand());
}
}

Block::BlockContents::BlockContents(std::string data) : data_{std::move(data)}, time_{time(nullptr)}
{
	std::fill(std::begin(this->previous_hash_), std::end(this->previous_hash_), '0');
	randomize(std::begin(this->random_), std::end(this->random_));
}

Block::BlockContents::BlockContents(const BlockContents& other) : data_{other.data_}, time_{other.time_}
{
	this->previous_hash_ = other.previous_hash_;
	this->proof_of_work_ = other.proof_of_work_;
	this->random_ = other.random_;
}

Block::BlockContents::BlockContents(BlockContents&& other) noexcept : data_{std::move(other.data_)}, time_{other.time_}
{
	this->previous_hash_ = other.previous_hash_;
	this->proof_of_work_ = other.proof_of_work_;
	this->random_ = other.random_;
}

Block::BlockContents& Block::BlockContents::operator=(const BlockContents& other)
{
	this->data_ = other.data_;
	this->previous_hash_ = other.previous_hash_;
	this->proof_of_work_ = other.proof_of_work_;
	this->random_ = other.random_;
	this->time_ = other.time_;

	return *this;
}

Block::BlockContents& Block::BlockContents::operator=(BlockContents&& other) noexcept
{
	this->data_ = std::move(other.data_);
	this->previous_hash_ = other.previous_hash_;
	this->proof_of_work_ = other.proof_of_work_;
	this->random_ = other.random_;
	this->time_ = other.time_;

	return *this;
}

Block::Block(std::string data) : block_contents_{std::move(data)}
{
	std::fill(this->hash_.begin(), this->hash_.end(), '0');
}

Block::Block(const Block& other) : block_contents_{other.block_contents_}
{
	this->hash_ = other.hash_;
}

Block::Block(Block&& other) noexcept : block_contents_{std::move(other.block_contents_)}
{
	this->hash_ = other.hash_;
}

Block& Block::operator=(const Block& other)
{
	this->block_contents_ = other.block_contents_;
	this->hash_ = other.hash_;

	return *this;
}

Block& Block::operator=(Block&& other) noexcept
{
	this->block_contents_ = std::move(other.block_contents_);
	this->hash_ = other.hash_;

	return *this;
}

void Block::Mine(std::size_t difficulty)
{
	// Notice we need () here and NOT {} (otherwise it'll think it's an initializer list)
	const std::vector<unsigned char> PREFIX(difficulty, '0');

	do
	{
		this->block_contents_.proof_of_work_ += 1;
		Hash::SHA256{*this}.FillHash(this->hash_);
	} while (std::search(this->hash_.begin(), this->hash_.end(), PREFIX.begin(), PREFIX.end()) != this->hash_.begin());
}