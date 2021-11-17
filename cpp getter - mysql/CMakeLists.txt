cmake_minimum_required(VERSION 3.7)
project(get)
set (CMAKE_CXX_STANDARD 17)
add_executable(get main.cpp)

TARGET_LINK_LIBRARIES( get PUBLIC -lboost_system -lpthread -lm -lcrypto -lcurl)

TARGET_LINK_LIBRARIES(get PUBLIC mysqlclient -lmysqlcppconn)
INCLUDE_DIRECTORIES(/usr/include/mysql/)
LINK_DIRECTORIES(/usr/include/mysql/)

#
# OpenSSL
#
    message("Compiling with SSL support")

    if (USE_CYASSL)
        # Use CyaSSL as OpenSSL replacement.
        # TODO: Add a find_package command for this also.
        message("CyaSSL include dir: ${CYASSL_INCLUDE_DIRS}")
        message("CyaSSL libraries: ${CYASSL_LIB}")

        # Additional to the root directory we need to include
        # the cyassl/ subdirectory which contains the OpenSSL
        # compatability layer headers.
        foreach(inc ${CYASSL_INCLUDE_DIRS})
            include_directories(${inc} ${inc}/cyassl)
        endforeach()

        list(APPEND LIB_LIST ${CYASSL_LIB})
    else()
        # TODO: Add support for STATIC also.
        find_package(OpenSSL REQUIRED)

        message("OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
        message("OpenSSL libraries: ${OPENSSL_LIBRARIES}")

        include_directories(${OPENSSL_INCLUDE_DIR})
        list(APPEND LIB_LIST ${OPENSSL_LIBRARIES})
    endif()
