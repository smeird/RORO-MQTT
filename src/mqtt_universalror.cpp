#include "mqtt_universalror.h"
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

namespace roro {

static const char *PROP_OPEN = "DOME_OPEN";
static const char *PROP_CLOSE = "DOME_CLOSE";
static const char *PROP_STOP = "DOME_STOP";

MQTTUniversalROR::MQTTUniversalROR()
    : client_("tcp://localhost:1883", "indi-mqtt-universalror") {}

const char *MQTTUniversalROR::getDefaultName() { return "MQTT Universal ROR"; }

bool MQTTUniversalROR::initProperties() {
    INDI::Dome::initProperties();
    addDebugControl();
    // Topics defaults
    topic_open_ = "observatory/roof/cmd/open";
    topic_close_ = "observatory/roof/cmd/close";
    topic_stop_ = "observatory/roof/cmd/stop";
    topic_limit_open_ = "observatory/roof/limit/open";
    topic_limit_closed_ = "observatory/roof/limit/closed";
    return true;
}

bool MQTTUniversalROR::updateProperties() {
    INDI::Dome::updateProperties();
    return true;
}

bool MQTTUniversalROR::Connect() {
    setup_mqtt();
    return true;
}

bool MQTTUniversalROR::Disconnect() {
    client_.disconnect()->wait();
    return true;
}

IPState MQTTUniversalROR::Park() {
    if (!fsm_.command_close())
        return IPS_ALERT;
    publish_command(topic_close_);
    return IPS_BUSY;
}

IPState MQTTUniversalROR::UnPark() {
    if (!fsm_.command_open())
        return IPS_ALERT;
    publish_command(topic_open_);
    return IPS_BUSY;
}

bool MQTTUniversalROR::Abort() {
    fsm_.command_stop();
    publish_command(topic_stop_);
    return true;
}

void MQTTUniversalROR::publish_command(const std::string &topic) {
    mqtt::message_ptr msg = mqtt::make_message(topic, "1");
    client_.publish(msg);
}

void MQTTUniversalROR::setup_mqtt() {
    mqtt::connect_options connOpts;
    client_.connect(connOpts)->wait();
    client_.start_consuming();
    client_.subscribe(topic_limit_open_, 1);
    client_.subscribe(topic_limit_closed_, 1);
    std::thread([this]() {
        while (client_.is_connected()) {
            auto msg = client_.consume_message();
            if (msg)
                mqtt_message_arrived(msg->get_topic(), msg->to_string());
        }
    }).detach();
}

void MQTTUniversalROR::mqtt_message_arrived(std::string topic, std::string payload) {
    if (topic == topic_limit_open_ && payload == "1") {
        fsm_.limit_open_triggered();
        SetParked(false);
    } else if (topic == topic_limit_closed_ && payload == "1") {
        fsm_.limit_closed_triggered();
        SetParked(true);
    }
}

} // namespace roro

// Factory
static std::unique_ptr<INDI::Dome> roof;

extern "C" {
    void ISGetProperties(const char *dev) {
        if (!roof)
            roof = std::make_unique<roro::MQTTUniversalROR>();
        roof->ISGetProperties(dev);
    }

    void ISInit() {}

    void ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n) {
        if (!roof)
            roof = std::make_unique<roro::MQTTUniversalROR>();
        roof->ISNewNumber(dev, name, values, names, n);
    }

    void ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n) {
        if (!roof)
            roof = std::make_unique<roro::MQTTUniversalROR>();
        roof->ISNewSwitch(dev, name, states, names, n);
    }

    void ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n) {
        if (!roof)
            roof = std::make_unique<roro::MQTTUniversalROR>();
        roof->ISNewText(dev, name, texts, names, n);
    }

    void ISPoll(void *p) {}
}
