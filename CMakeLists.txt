cmake_minimum_required ( VERSION 2.8.3 )

set ( PROJECT "MARKOV_CHAIN_RANDOM_TEXT_GENERATOR" )
project ( ${PROJECT} )

set ( PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT}/bin )
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_ROOT}/bin )
set ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_ROOT}/bin )

set ( CMAKE_CXX_FLAGS "-W -Wall -std=c++14" )
set ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g3" )
set ( CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" )

add_definitions ( "-D_FILE_OFFSET_BITS=64" )


include ( ${PROJECT_ROOT}/cmake/icu.cmake )


include_directories (
    src
    ${ICU_INC}
                    )


set ( HEADERS_
      src/markchain.h
      src/utils.h
    )

set ( SOURCES_LEARNER
      src/learner.cpp
    )

set ( SOURCES_GENERATOR
      src/generator.cpp
    )

set ( LIBRARIES_
    ${ICU_LIB}
    )

add_executable ( learner ${HEADERS_} ${SOURCES_LEARNER} )
target_link_libraries ( learner ${LIBRARIES_} )

add_executable ( generator ${HEADERS_} ${SOURCES_GENERATOR} )
target_link_libraries ( generator ${LIBRARIES_} )
