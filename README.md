# RORO-MQTT

This project provides a small C++ module intended for INDI/EKOS setups to
control a roll-on roll-off roof through an MQTT service.  The
`MQTTRoofController` class publishes open/close commands, handles optional
limit switch feedback, power control, and percentage-open reporting over
configurable MQTT topics.

## Features

* Connect to an MQTT broker with optional username/password
* Publish open and close commands
* Optional topics for open/close limit switches
* Optional percentage-open feedback topic
* Optional power control topic

## Usage

```cpp
#include "mqtt_roof_controller.h"
#include "mqtt_client.h"
#include <memory>

int main() {
    auto client = std::make_shared<PahoMqttClient>(
        "tcp://mqtt.example.net:1883", "roof_controller", "user", "secret");
    MQTTRoofController controller(
        client,
        "observatory/roof/open",
        "observatory/roof/close",
        "observatory/roof/open_limit",
        "observatory/roof/close_limit",
        "observatory/roof/percent",
        "observatory/roof/power");

    controller.connect();
    controller.set_power(true);
    controller.open_roof();
}
```

Compile with:

```
g++ -std=c++17 -DPAHO_MQTT src/*.cpp tests/test_driver.cpp \
    -lpaho-mqttpp3 -lpaho-mqtt3as -o example
```
