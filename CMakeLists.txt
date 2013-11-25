cmake_minimum_required (VERSION 2.8)
project (Https-Tryout)
add_executable (HelloWorld helloWorld.cxx)

# Use Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.53.0 REQUIRED system thread regex)
include_directories(${Boost_INCLUDE_DIRS})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

#Use OpenSSL
find_package(OpenSSL 1.0.1 REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
set(LIBS ${LIBS} ${OPENSSL_LIBRARIES})

target_link_libraries(HelloWorld ${LIBS})

file(INSTALL ${CMAKE_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_BINARY_DIR})
