#include <iostream>

#include "src/bip39.h"
#include "src/mnemonic.h"

int main()
{
    //    auto mn = BIP39::Generate(12);
    //    std::cout << "Entropy: " << mn.entropy() << std::endl;

    //    std::cout << "seed: " << mn.generateSeed("waqar") << std::endl;
    auto m = BIP39::Entropy("00000000000000000000000000000000");
    std::cout << m.entropy() << std::endl;
    for (auto a : m.words()) {
        std::cout << a << " ";
    }
    std::cout << std::endl;
    std::cout << m.generateSeed() << std::endl;


    return 0;
}
