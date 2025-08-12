#include "mqtt_roof_controller.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>

// Simple fake client used to verify that commands are published to the
// expected topics.
class FakeMqttClient : public IMqttClient {
public:
    void connect() override {}
    void disconnect() override {}
    void publish(const std::string& topic, const std::string& payload) override {
        published.emplace_back(topic, payload);
    }
    void subscribe(const std::string& topic,
                   std::function<void(const std::string&)> cb) override {
        subs[topic] = std::move(cb);
    }

    void emit(const std::string& topic, const std::string& payload) {
        if (subs.count(topic)) subs[topic](payload);
    }

    std::vector<std::pair<std::string, std::string>> published;
    std::unordered_map<std::string, std::function<void(const std::string&)>> subs;
};

int main() {
    auto client = std::make_shared<FakeMqttClient>();
    MQTTRoofController controller(client,
                                 "roof/open",
                                 "roof/close",
                                 "roof/open_limit",
                                 "roof/close_limit",
                                 "roof/percent",
                                 "roof/power");
    controller.connect();

    controller.set_power(true);
    controller.open_roof();
    controller.close_roof();
    controller.set_open_percentage(42);

    assert(client->published.size() == 4);
    assert(client->published[0] == std::make_pair(std::string("roof/power"), std::string("1")));
    assert(client->published[1] == std::make_pair(std::string("roof/open"), std::string("1")));
    assert(client->published[2] == std::make_pair(std::string("roof/close"), std::string("1")));
    assert(client->published[3] == std::make_pair(std::string("roof/percent"), std::string("42")));

    // Simulate limit switch callbacks
    client->emit("roof/open_limit", "1");
    client->emit("roof/close_limit", "1");
    assert(controller.is_open());
    assert(controller.is_closed());

    std::cout << "All tests passed\n";
    return 0;
}
