import os
import sys
import types
from unittest.mock import MagicMock

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from roro_mqtt.driver import MQTTRoofController, configure_topics


def make_msg(topic, payload):
    return types.SimpleNamespace(topic=topic, payload=payload)


def test_publish_commands():
    configure_topics()  # reset defaults
    client = MagicMock()
    driver = MQTTRoofController(
        host="localhost",
        client=client,
        topic_open="open",
        topic_close="close",
        topic_power="power",
    )

    driver.open_roof()
    client.publish.assert_any_call("open", payload="1")

    driver.close_roof()
    client.publish.assert_any_call("close", payload="1")

    driver.set_power(True)
    client.publish.assert_any_call("power", payload="ON")


def test_state_updates():
    configure_topics()  # reset defaults
    client = MagicMock()
    driver = MQTTRoofController(
        host="localhost",
        client=client,
        topic_open_limit="ol",
        topic_close_limit="cl",
        topic_percent="percent",
    )

    driver._on_message(client, None, make_msg("ol", b"1"))
    assert driver.open_limit is True

    driver._on_message(client, None, make_msg("cl", b"1"))
    assert driver.close_limit is True

    driver._on_message(client, None, make_msg("percent", b"45"))
    assert driver.percent_open == 45.0


def test_configure_topics():
    client = MagicMock()
    configure_topics(open="cfg/open", close="cfg/close", power="cfg/power")
    driver = MQTTRoofController(host="localhost", client=client)

    driver.open_roof()
    client.publish.assert_any_call("cfg/open", payload="1")

    driver.close_roof()
    client.publish.assert_any_call("cfg/close", payload="1")

    driver.set_power(True)
    client.publish.assert_any_call("cfg/power", payload="ON")

    # Reset for other tests
    configure_topics()
