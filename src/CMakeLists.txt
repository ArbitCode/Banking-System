add_subdirectory(routes)
add_subdirectory(controllers)
add_subdirectory(services)

#Collect all source files from submodule
set(SOURCES
  main.cpp
)

add_executable(bank_backend ${SOURCES})

#Link libraries
target_link_libraries(bank_backend Threads::Threads nanodbc odbc routes controllers services)
# Ensure log directory exists
add_custom_command(
    TARGET bank_backend POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/out
    COMMENT "Creating output log directory..."
)

# Generate systemd service file using `file(GENERATE)`
file(GENERATE OUTPUT ${CMAKE_BINARY_DIR}/bank_backend.service CONTENT "
[Unit]
Description=Bank System Backend Service
After=network.target

[Service]
ExecStart=${CMAKE_BINARY_DIR}/bin/bank_backend -d
Restart=always
StandardOutput=file:${CMAKE_BINARY_DIR}/out/service.log
StandardError=file:${CMAKE_BINARY_DIR}/out/error.log
User=root
Group=root
WorkingDirectory=${CMAKE_BINARY_DIR}

[Install]
WantedBy=multi-user.target
")

# Install systemd service (Only when running `ninja vstart`)
add_custom_target(vstart
    COMMAND sudo cp ${CMAKE_BINARY_DIR}/bank_backend.service /etc/systemd/system/bank_backend.service
    COMMAND sudo systemctl daemon-reload
    COMMAND sudo systemctl enable bank_backend
    COMMAND sudo systemctl restart bank_backend
    COMMENT "Installing and starting systemd service..."
)

# Stop and remove the service
add_custom_target(stop
    COMMAND sudo systemctl stop bank_backend
    COMMAND sudo systemctl disable bank_backend
    COMMAND sudo rm -f /etc/systemd/system/bank_backend.service
    COMMAND sudo systemctl daemon-reload
    COMMENT "Stopping and removing systemd service..."
)
