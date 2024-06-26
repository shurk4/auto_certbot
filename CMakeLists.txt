cmake_minimum_required(VERSION 3.5)

project(auto_certbot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(auto_certbot
    main.cpp
    config.h
    config.cpp
    func.hpp
)

install(TARGETS auto_certbot
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
