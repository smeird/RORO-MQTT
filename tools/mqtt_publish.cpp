#include <mqtt/async_client.h>
#include <iostream>

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "Usage: mqtt_publish <topic> <payload>" << std::endl;
        return 1;
    }
    std::string server = "tcp://localhost:1883";
    mqtt::async_client client(server, "mqtt-pub-cli");
    mqtt::connect_options opts;
    client.connect(opts)->wait();
    client.publish(argv[1], argv[2]);
    client.disconnect()->wait();
    return 0;
}
