#include "mqtt_client.h"

#ifdef PAHO_MQTT
#include <chrono>
#include <thread>

PahoMqttClient::PahoMqttClient(const std::string& server_uri,
                               const std::string& client_id,
                               const std::string& username,
                               const std::string& password)
    : client_(server_uri, client_id) {
    conn_opts_.set_clean_session(true);
    if (!username.empty()) {
        conn_opts_.set_user_name(username);
        conn_opts_.set_password(password);
    }
    client_.set_callback(*this);
}

void PahoMqttClient::connect() {
    client_.connect(conn_opts_)->wait();
}

void PahoMqttClient::disconnect() {
    client_.disconnect()->wait();
}

void PahoMqttClient::publish(const std::string& topic, const std::string& payload) {
    auto msg = mqtt::make_message(topic, payload);
    client_.publish(msg)->wait();
}

void PahoMqttClient::subscribe(const std::string& topic,
                               std::function<void(const std::string&)> cb) {
    callbacks_[topic] = std::move(cb);
    client_.subscribe(topic, 0)->wait();
}

void PahoMqttClient::message_arrived(mqtt::const_message_ptr msg) {
    auto it = callbacks_.find(msg->get_topic());
    if (it != callbacks_.end()) {
        it->second(msg->to_string());
    }
}
#endif // PAHO_MQTT
