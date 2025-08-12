# RORO-MQTT

This project provides a simple module intended for INDI/EKOS setups to
control a roll-on roll-off roof through an MQTT service.  The
`MQTTRoofController` class can publish open/close commands and handle
limit switch feedback over configurable MQTT topics.

## Features

* Connect to an MQTT broker with optional username/password
* Publish open and close commands
* Optional topics for open/close limit switches
* Optional percentage-open feedback topic
* Optional power control topic

## Usage

```python
from roro_mqtt import MQTTRoofController

controller = MQTTRoofController(
    host="mqtt.example.net",
    username="user",
    password="secret",
    topic_open="observatory/roof/open",
    topic_close="observatory/roof/close",
    topic_open_limit="observatory/roof/open_limit",
    topic_close_limit="observatory/roof/close_limit",
    topic_percent="observatory/roof/percent",
    topic_power="observatory/roof/power",
)

controller.connect()
controller.set_power(True)
controller.open_roof()
```
