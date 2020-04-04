#include <iostream>

#include "src/bip39.h"
#include "src/mnemonic.h"

int main()
{
    BIP39::Generate(12);

    return 0;
}
