enable_testing()

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(bank_tests ${TEST_SOURCES})

target_link_libraries(bank_tests PRIVATE services GTest::GTest GTest::Main nanodbc odbc cpp_dotenv cpr)

add_test(NAME API_Tests COMMAND bank_tests)
