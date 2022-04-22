cmake_minimum_required(VERSION 3.5)

project(nomenus-rex
        VERSION 0.5.2
        HOMEPAGE_URL "https://github.com/ANGulchenko/nomenus-rex"
        DESCRIPTION "Nomenus-rex is a CLI utility for the file mass-renaming.")

configure_file(version.h.in version.h)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_custom_target(Docs SOURCES
    CHANGELOG
    README
)

ADD_LIBRARY(AnyOption STATIC
    AnyOption/anyoption.cpp AnyOption/anyoption.h
)

ADD_LIBRARY(Progressator STATIC
    Progressator/progressator.cpp Progressator/progressator.h
)

ADD_LIBRARY(Rules STATIC
    Rules/rules.h
    Rules/ruleBase.cpp          Rules/ruleBase.h
    Rules/ruleDate.cpp          Rules/ruleDate.h
    Rules/ruleText.cpp          Rules/ruleText.h
    Rules/ruleInteger.cpp       Rules/ruleInteger.h
    Rules/ruleExtension.cpp     Rules/ruleExtension.h
    Rules/ruleDir.cpp           Rules/ruleDir.h
    Rules/ruleFilename.cpp      Rules/ruleFilename.h
    Rules/ruleFilesize.cpp      Rules/ruleFilesize.h
    Rules/ruleReplace.cpp       Rules/ruleReplace.h
)

ADD_LIBRARY(Tests STATIC
    Tests/tests.cpp Tests/tests.h
)

add_executable (nomenus-rex
    main.cpp
    configurationparser.cpp configurationparser.h
    renamer.cpp renamer.h
    )

find_package(ICU REQUIRED COMPONENTS data i18n uc dt)

target_link_libraries(nomenus-rex AnyOption Rules Tests Progressator config++)
target_link_libraries(nomenus-rex ICU::data ICU::i18n ICU::uc ICU::dt)

target_include_directories(nomenus-rex PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}"
                           )


