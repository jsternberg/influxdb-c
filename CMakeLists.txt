cmake_minimum_required(VERSION 2.6)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(influxdb)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

set(INFLUXDB_SOURCES
  client.c
  point.c
  protocol.c
  buffer.c
)

add_library(influxdb ${INFLUXDB_SOURCES})
target_link_libraries(influxdb ${CURL_LIBRARIES})

file(GLOB testfiles "*_test.cc")
add_executable(influxdb_test "${testfiles}")
target_link_libraries(influxdb_test influxdb)

include(CTest)
add_test(NAME influxdb COMMAND influxdb_test)

install(TARGETS influxdb DESTINATION lib)
install(FILES influxdb.h DESTINATION include)
