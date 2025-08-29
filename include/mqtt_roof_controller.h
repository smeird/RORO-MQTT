#ifndef MQTT_ROOF_CONTROLLER_H
#define MQTT_ROOF_CONTROLLER_H

#include "mqtt_client.h"
#include <memory>
#include <string>

// Controller capable of driving a roll-on roll-off roof via MQTT topics.
class MQTTRoofController {
public:
    MQTTRoofController(std::shared_ptr<IMqttClient> client,
                       const std::string& topic_open,
                       const std::string& topic_close,
                       const std::string& topic_open_limit = "",
                       const std::string& topic_close_limit = "",
                       const std::string& topic_percent = "",
                       const std::string& topic_power = "",
                       const std::string& topic_telescope = "");

    void connect();
    void disconnect();
    void open_roof();
    void close_roof();
    void set_power(bool on);
    void set_open_percentage(int percent);

    bool is_open() const { return open_state_; }
    bool is_closed() const { return close_state_; }
    bool is_telescope_parked() const { return telescope_parked_; }

private:
    std::shared_ptr<IMqttClient> client_;
    std::string topic_open_;
    std::string topic_close_;
    std::string topic_open_limit_;
    std::string topic_close_limit_;
    std::string topic_percent_;
    std::string topic_power_;
    std::string topic_telescope_;

    bool open_state_ = false;
    bool close_state_ = false;
    bool telescope_parked_ = false;
};

#endif // MQTT_ROOF_CONTROLLER_H
