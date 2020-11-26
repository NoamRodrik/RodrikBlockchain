#include "Blockchain.h"

#include <algorithm>
#include <Tools/Hash/SHA256.h>

using namespace Core;
using namespace Hash;

Blockchain::Blockchain()
{
    this->chain_.reserve(BLOCKS_AMOUNT);
    this->Emplace("Genesis");
}

void Blockchain::Add(Block block)
{
    this->chain_.push_back(std::move(block));
    this->InitializeLastBlock();
}

void Blockchain::InitializeLastBlock()
{
    auto&& current_block{this->chain_.back()};

    // We need at least 2 for the previous block.
    if (this->chain_.size() >= 2)
    {
        const auto& previous_block{this->chain_.rbegin()[1]};

        // Copy the N-1 Block's hash into the N's previous hash.
        std::copy(std::begin(previous_block.GetHash()),
                  std::end(previous_block.GetHash()),
                  std::begin(current_block.GetPreviousHash()));
    }

    current_block.Mine(this->difficulty_);
}

bool Blockchain::Validate() const
{
    std::array<unsigned char, 2 * SHA256::HASH_BYTES> calculated_hash{};

    return std::adjacent_find(std::rbegin(this->chain_),
                              std::rend(this->chain_),
                              [&calculated_hash](const auto& current_block, const auto& previous_block)
                              {
                                SHA256{previous_block}.FillHash(calculated_hash);
                              
                                return !std::equal(std::begin(calculated_hash),
                                                   std::end(calculated_hash),
                                                   std::begin(current_block.GetPreviousHash()),
                                                   std::end(current_block.GetPreviousHash()));
                              }) == std::rend(this->chain_);
}