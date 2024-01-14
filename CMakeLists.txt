cmake_minimum_required(VERSION 3.27)

project("winsock2_sample")

# Server app
set(ServerApp "Server")

add_executable(
    ${ServerApp}
    src/server/server.cpp
)

# Client app
set(ClientApp "Client")

add_executable(
    ${ClientApp}
    src/client/client.cpp
)

# Common settings
set_target_properties(
    ${ServerApp} ${ClientApp}
    PROPERTIES
        CXX_STANDARD 20
)
