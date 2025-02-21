add_library(services 
	AccountService.cpp TransactionService.cpp
)

target_link_libraries(services controllers)
