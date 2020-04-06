#include <cassert>
#include <iostream>

#include "src/bip39.h"
#include "src/mnemonic.h"
#include "src/utils.h"

static std::string joined_mnemonic(const std::vector<std::string>& s)
{
    return BIP39_Utils::Join(s, " ");
}

int main()
{
    std::string passPhrase = "TREZOR";

    auto m = BIP39::Entropy("00000000000000000000000000000000");
    assert(m.entropy() == "00000000000000000000000000000000");
    assert(
        joined_mnemonic(m.words()) ==
        "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon "
        "about");
    std::string exp =
        "C55257C360C07C72029AEBC1B53C05ED0362ADA38EAD3E3E9EFA3708E53495531F09A6987599D18264C1E1C92F"
        "2CF141630C7A3C4AB7C81B2F001698E7463B04";
    std::string actual = BIP39_Utils::base16Encode(m.generateSeed("TREZOR"));
    assert(actual == exp);


    m = BIP39::Entropy("7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f");
    assert(m.entropy() == "7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f");
    assert(
        joined_mnemonic(m.words()) ==
        "legal winner thank year wave sausage worth useful legal winner thank yellow");
    exp =
        "2E8905819B8723FE2C1D161860E5EE1830318DBF49A83BD451CFB8440C28BD6FA457FE1296106559A3C80937A1"
        "C1069BE3A3A5BD381EE6260E8D9739FCE1F607";
    actual = BIP39_Utils::base16Encode(m.generateSeed("TREZOR"));
    assert(actual == exp);


    return 0;
}
