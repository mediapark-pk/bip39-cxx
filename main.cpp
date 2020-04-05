#include <iostream>

#include "src/bip39.h"
#include "src/mnemonic.h"

int main()
{
    auto mn = BIP39::Generate(12);
    std::cout << "Entropy: " << mn.entropy() << std::endl;

    return 0;
}
