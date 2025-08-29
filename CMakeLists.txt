cmake_minimum_required(VERSION 3.10)
project(indi_mqtt_universalror)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(/usr/include/libindi include)

add_executable(indi-mqtt-universalror src/mqtt_universalror.cpp src/state_machine.cpp)

target_link_libraries(indi-mqtt-universalror indidriver indiclient paho-mqttpp3 paho-mqtt3as pthread)

add_executable(mqtt_publish tools/mqtt_publish.cpp)
target_link_libraries(mqtt_publish paho-mqttpp3 paho-mqtt3as pthread)

add_executable(state_machine_test tests/state_machine_test.cpp src/state_machine.cpp)

target_link_libraries(state_machine_test gtest gtest_main pthread)

enable_testing()
add_test(NAME state_machine_test COMMAND state_machine_test)
