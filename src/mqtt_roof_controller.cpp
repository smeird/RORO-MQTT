#include "mqtt_roof_controller.h"

#include <utility>

MQTTRoofController::MQTTRoofController(std::shared_ptr<IMqttClient> client,
                                       const std::string& topic_open,
                                       const std::string& topic_close,
                                       const std::string& topic_open_limit,
                                       const std::string& topic_close_limit,
                                       const std::string& topic_percent,
                                       const std::string& topic_power,
                                       const std::string& topic_telescope)
    : client_(std::move(client)),
      topic_open_(topic_open),
      topic_close_(topic_close),
      topic_open_limit_(topic_open_limit),
      topic_close_limit_(topic_close_limit),
      topic_percent_(topic_percent),
      topic_power_(topic_power),
      topic_telescope_(topic_telescope) {}

void MQTTRoofController::connect() {
    client_->connect();
    if (!topic_open_limit_.empty()) {
        client_->subscribe(topic_open_limit_, [this](const std::string& payload) {
            open_state_ = (payload != "0");
        });
    }
    if (!topic_close_limit_.empty()) {
        client_->subscribe(topic_close_limit_, [this](const std::string& payload) {
            close_state_ = (payload != "0");
        });
    }
    if (!topic_telescope_.empty()) {
        client_->subscribe(topic_telescope_, [this](const std::string& payload) {
            telescope_parked_ = (payload != "0");
        });
    }
}

void MQTTRoofController::disconnect() {
    client_->disconnect();
}

void MQTTRoofController::open_roof() {
    client_->publish(topic_open_, "1");
}

void MQTTRoofController::close_roof() {
    client_->publish(topic_close_, "1");
}

void MQTTRoofController::set_power(bool on) {
    if (!topic_power_.empty()) {
        client_->publish(topic_power_, on ? "1" : "0");
    }
}

void MQTTRoofController::set_open_percentage(int percent) {
    if (!topic_percent_.empty()) {
        client_->publish(topic_percent_, std::to_string(percent));
    }
}
