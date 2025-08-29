#pragma once

#include <indidome.h>
#include <mqtt/async_client.h>
#include "state_machine.h"

namespace roro {

class MQTTUniversalROR : public INDI::Dome {
public:
    MQTTUniversalROR();
    virtual ~MQTTUniversalROR() = default;

    virtual const char *getDefaultName() override;
    virtual bool initProperties() override;
    virtual bool updateProperties() override;

    virtual bool Connect() override;
    virtual bool Disconnect() override;

    virtual IPState Park() override;
    virtual IPState UnPark() override;
    virtual bool Abort() override;

    void mqtt_message_arrived(std::string topic, std::string payload);

private:
    void publish_command(const std::string &topic);
    void setup_mqtt();

    mqtt::async_client client_;
    RoofStateMachine fsm_;
    std::string topic_open_; 
    std::string topic_close_;
    std::string topic_stop_;
    std::string topic_limit_open_;
    std::string topic_limit_closed_;
};

} // namespace roro
