#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <functional>
#include <string>
#include <unordered_map>

// Generic MQTT client interface so that the roof controller can be
// unit-tested without a real broker.
class IMqttClient {
public:
    virtual ~IMqttClient() = default;
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void publish(const std::string& topic, const std::string& payload) = 0;
    virtual void subscribe(const std::string& topic,
                           std::function<void(const std::string&)> cb) = 0;
};

#ifdef PAHO_MQTT
#include <mqtt/async_client.h>

// Paho implementation of the MQTT client interface.
class PahoMqttClient : public IMqttClient, public virtual mqtt::callback {
public:
    PahoMqttClient(const std::string& server_uri,
                   const std::string& client_id,
                   const std::string& username = "",
                   const std::string& password = "");

    void connect() override;
    void disconnect() override;
    void publish(const std::string& topic, const std::string& payload) override;
    void subscribe(const std::string& topic,
                   std::function<void(const std::string&)> cb) override;

    // mqtt::callback
    void connected(const std::string&) override {}
    void connection_lost(const std::string&) override {}
    void message_arrived(mqtt::const_message_ptr msg) override;
    void delivery_complete(mqtt::delivery_token_ptr) override {}

private:
    mqtt::async_client client_;
    mqtt::connect_options conn_opts_;
    std::unordered_map<std::string,
        std::function<void(const std::string&)>> callbacks_;
};
#endif // PAHO_MQTT

#endif // MQTT_CLIENT_H
