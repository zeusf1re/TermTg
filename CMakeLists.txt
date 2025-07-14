cmake_minimum_required(VERSION 3.10)
project(telegram_terminal_client)

set(CMAKE_CXX_STANDARD 17)

# Ищем TDLib
find_package(Td REQUIRED PATHS /usr/local/lib/cmake/Td)

add_executable(client 
    td_example.cpp
    # Другие исходные файлы, если есть
)

# Вариант 1: Официальный способ
target_link_libraries(client PRIVATE Td::tdjson Td::tdjson_static Td::tdclient)

# Вариант 2: Альтернативный способ (если первый не работает)
# target_link_libraries(client PRIVATE 
#     tdjson
#     tdjson_static
#     tdclient
#     tdapi
#     tdactor
#     tdutils
#     tdcore
# )
