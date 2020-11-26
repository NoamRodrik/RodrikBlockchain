#include <Core/Blockchain/Blockchain.h>
#include <cstdlib>
#include <ctime>

using namespace Core;

int main()
{
    // Use current time as seed for random generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    Blockchain chain{};
    chain.Emplace("Block 1");
    chain.Emplace("Block 2");
    chain.Emplace("Block 3");

    printf("\nValidating Blockchain: %s.\n", chain.Validate() ? "Passed" : "Failed");

    return 0;
}