cmake_minimum_required(VERSION 3.10)
project(telegram_terminal_client)

set(CMAKE_CXX_STANDARD 17)

find_package(Td REQUIRED PATHS /usr/local/lib/cmake/Td)

add_executable(client 
    main.cpp
)

target_link_libraries(client PRIVATE Td::tdjson Td::tdjson_static Td::tdclient)
