cmake_minimum_required(VERSION 3.14)
project(indi_mqtt_universalror)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

include_directories(/usr/include/libindi include)

add_executable(indi-mqtt-universalror src/mqtt_universalror.cpp src/state_machine.cpp)

target_link_libraries(indi-mqtt-universalror indidriver indiclient paho-mqttpp3 paho-mqtt3as pthread)

add_executable(mqtt_publish tools/mqtt_publish.cpp)
target_link_libraries(mqtt_publish paho-mqttpp3 paho-mqtt3as pthread)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

add_executable(state_machine_test tests/state_machine_test.cpp src/state_machine.cpp)

target_link_libraries(state_machine_test gtest_main pthread)

enable_testing()
add_test(NAME state_machine_test COMMAND state_machine_test)

install(TARGETS indi-mqtt-universalror DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES mqtt_universalror.xml DESTINATION ${CMAKE_INSTALL_DATADIR}/indi)
install(FILES indi-mqtt-universalror.rules DESTINATION ${CMAKE_INSTALL_LIBDIR}/udev/rules.d)
