# BIP39 Mnemonic

Mnemonic BIP39 implementation in C++

## Build

```sh
mkdir build && cd build
cmake .. && make
```

# Usage
```cpp
//use specified entropy
auto mnemonic = BIP39::Entropy("00000000000000000000000000000000");

//use randomly generated entropy
auto mnemonic = BIP39::Generate(12 /*word count*/);

//reverse (Mnemonic to entropy)
auto mnemonic = BIP39::Words("virtual wear virtual wear number paddle spike usage degree august buffalo layer");
//get entropy
mnemonic.entropy();

```

## License
- MIT
