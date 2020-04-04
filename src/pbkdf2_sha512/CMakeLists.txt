add_library(pbkdf2_sha512 hmac.h options.h 
        common.h  pbkdf2.cpp
        hmac.cpp  pbkdf2.hpp  memzero.h memzero.cpp sha2.hpp sha2.cpp )
