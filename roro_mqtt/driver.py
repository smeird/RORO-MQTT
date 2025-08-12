import paho.mqtt.client as mqtt
from typing import Optional

class MQTTRoofController:
    """Control a roll-on roll-off roof via MQTT."""

    def __init__(
        self,
        host: str,
        port: int = 1883,
        username: Optional[str] = None,
        password: Optional[str] = None,
        topic_open: str = "roof/open",
        topic_close: str = "roof/close",
        topic_open_limit: Optional[str] = None,
        topic_close_limit: Optional[str] = None,
        topic_percent: Optional[str] = None,
        topic_power: Optional[str] = None,
        client: Optional[mqtt.Client] = None,
    ) -> None:
        """Create a new controller.

        Parameters
        ----------
        host: str
            MQTT broker address.
        port: int, optional
            MQTT broker port, default 1883.
        username, password: optional
            Credentials for the broker.
        topic_open, topic_close: str
            Topics to publish open/close commands.
        topic_open_limit, topic_close_limit: optional
            Topics publishing limit switch states. Payload ``b"1"`` means
action triggered.
        topic_percent: optional
            Topic with percentage open payload (0-100).
        topic_power: optional
            Topic to power the roof hardware on/off.
        client: optional
            Injected ``paho.mqtt.client.Client`` instance for testing.
        """
        self.host = host
        self.port = port
        self.topic_open = topic_open
        self.topic_close = topic_close
        self.topic_open_limit = topic_open_limit
        self.topic_close_limit = topic_close_limit
        self.topic_percent = topic_percent
        self.topic_power = topic_power

        self.client = client or mqtt.Client()
        if username is not None and password is not None:
            self.client.username_pw_set(username, password)
        self.client.on_connect = self._on_connect
        self.client.on_message = self._on_message

        self._open_limit = False
        self._close_limit = False
        self._percent_open: Optional[float] = None

    # ------------------------------------------------------------------
    def connect(self) -> None:
        """Connect to the MQTT broker and subscribe to state topics."""
        self.client.connect(self.host, self.port)
        if any([self.topic_open_limit, self.topic_close_limit, self.topic_percent]):
            self.client.loop_start()

    # ------------------------------------------------------------------
    def _on_connect(self, client, userdata, flags, rc):  # pragma: no cover - callback
        for topic in [self.topic_open_limit, self.topic_close_limit, self.topic_percent]:
            if topic:
                client.subscribe(topic)

    # ------------------------------------------------------------------
    def _on_message(self, client, userdata, msg):  # pragma: no cover - callback
        if msg.topic == self.topic_open_limit:
            self._open_limit = msg.payload.strip() == b"1"
        elif msg.topic == self.topic_close_limit:
            self._close_limit = msg.payload.strip() == b"1"
        elif msg.topic == self.topic_percent:
            try:
                self._percent_open = float(msg.payload)
            except ValueError:
                pass

    # ------------------------------------------------------------------
    def open_roof(self) -> None:
        """Send command to open the roof."""
        self.client.publish(self.topic_open, payload="1")

    # ------------------------------------------------------------------
    def close_roof(self) -> None:
        """Send command to close the roof."""
        self.client.publish(self.topic_close, payload="1")

    # ------------------------------------------------------------------
    def set_power(self, on: bool) -> None:
        """Power the roof hardware on or off."""
        if self.topic_power:
            self.client.publish(self.topic_power, payload="ON" if on else "OFF")

    # ------------------------------------------------------------------
    @property
    def open_limit(self) -> bool:
        return self._open_limit

    @property
    def close_limit(self) -> bool:
        return self._close_limit

    @property
    def percent_open(self) -> Optional[float]:
        return self._percent_open
