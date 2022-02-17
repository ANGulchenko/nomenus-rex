cmake_minimum_required(VERSION 3.5)

project(nomenus-rex
        VERSION 0.1.0
        HOMEPAGE_URL "https://"
        DESCRIPTION "Nomenus-rex")

configure_file(version.h.in version.h)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

ADD_LIBRARY(AnyOption
    AnyOption/anyoption.cpp
    AnyOption/anyoption.h
)

add_executable (nomenus-rex
    main.cpp
    CLIparser.cpp CLIparser.h
    rule.cpp rule.h
    renamer.cpp renamer.h
    )

target_link_libraries(nomenus-rex AnyOption)
target_link_libraries(nomenus-rex config++)

target_include_directories(nomenus-rex PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}"
                           )


