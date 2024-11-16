find_package(GTest REQUIRED)

set(TEST_SOURCES
    test_block.cpp
    test_transaction.cpp
    test_blockchain.cpp
    test_crypto.cpp
    test_wallet.cpp
    test_consensus.cpp
)

add_executable(blockchain_tests ${TEST_SOURCES})

target_link_libraries(blockchain_tests PRIVATE
    GTest::GTest
    GTest::Main
    ${PROJECT_NAME}_lib
)

add_test(NAME blockchain_tests COMMAND blockchain_tests) 