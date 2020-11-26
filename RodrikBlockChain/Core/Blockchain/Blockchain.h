#ifndef __BLOCKCHAIN_H__
#define __BLOCKCHAIN_H__

#include <Core/Block/Block.h>
#include <vector>

namespace Core
{
class Blockchain
{
public:
	Blockchain();

public:
	void Add(Block block);

	template <typename... Args>
	void Emplace(Args&&... args)
	{
		this->chain_.emplace_back(std::forward<Args...>(args...));
		this->InitializeLastBlock();
		printf("Hash for %s: %64s\n", this->chain_.back().GetData().c_str(), this->chain_.back().GetHash().data());
	}

	bool Validate() const;

private:
	void InitializeLastBlock();

private:
	static constexpr std::size_t DEFAULT_DIFFICULTY{3};
	static constexpr std::size_t BLOCKS_AMOUNT{100};

private:
	std::vector<Block> chain_{};
	std::size_t        difficulty_{DEFAULT_DIFFICULTY};
};
}

#endif // __BLOCKCHAIN_H__