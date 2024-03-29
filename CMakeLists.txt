cmake_minimum_required(VERSION 3.27)

project("winsock2_sample")

# Common lib
set(CommonLib "Common")

add_library(
    ${CommonLib}
    STATIC
    src/common/context.cpp
    src/common/context.h
    src/common/types.cpp
    src/common/types.h
)

target_include_directories(
    ${CommonLib}
    PUBLIC
        src/.
)

# Server app
set(ServerApp "Server")

add_executable(
    ${ServerApp}
    src/server/server.cpp
)

target_link_libraries(
    ${ServerApp}
    PRIVATE
        ${CommonLib}
        Ws2_32.lib
)

# Client app
set(ClientApp "Client")

add_executable(
    ${ClientApp}
    src/client/client.cpp
)

target_link_libraries(
    ${ClientApp}
    PRIVATE
        ${CommonLib}
        Ws2_32.lib
)

# Common settings
set_target_properties(
    ${CommonLib} ${ServerApp} ${ClientApp}
    PROPERTIES
        CXX_STANDARD 20
)
