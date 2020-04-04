add_subdirectory(pbkdf2_sha512)
add_library(bip39-cxx bip39.cpp mnemonic.cpp wordlist.cpp utils.h utils.cpp)

target_link_libraries(bip39-cxx PRIVATE pbkdf2_sha512)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/wordlists/english.txt
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/wordlists/french.txt
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/wordlists/spanish.txt
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/wordlists/italian.txt
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../)

