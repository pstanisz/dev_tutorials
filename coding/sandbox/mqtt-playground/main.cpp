#include <iostream>
#include <thread>

#include <mqtt/client.h>

const std::string TOPIC = "topic";
const std::string PAYLOAD = "hello!";

int main(int /*argc*/, char * /*argv*/[])
{
    std::cout << "mqtt-playground\n";

    const std::string address = "127.0.0.1:1234";
    const std::string producer_id = "producer";
    const std::string consumer_id = "consumer";

    mqtt::client consumer(address, consumer_id, mqtt::create_options(mqtt::MQTTPP_V3_CODE));

    mqtt::connect_options con_opts;
    con_opts.set_keep_alive_interval(100);
    con_opts.set_clean_session(true);

    mqtt::client producer(address, producer_id, mqtt::create_options(mqtt::MQTTPP_V3_CODE));

    std::jthread producer_thread([&producer, &con_opts]()
                                 {
    try {
      producer.connect(con_opts);

      // auto msg = mqtt::make_message(TOPIC, PAYLOAD);
      // msg->set_qos(mqtt::GRANTED_QOS_0);

      // producer.publish(msg);

      producer.disconnect();
    } catch (const mqtt::exception &e) {
      std::cerr << "MQTT exception caught: " << e.what() << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Standard exception caught: " << e.what() << std::endl;
    } });

    std::jthread consumer_thread([&consumer, &con_opts]()
                                 {
    try {
      consumer.connect(con_opts);

      consumer.subscribe(TOPIC);
      consumer.start_consuming();

      // auto msg = mqtt::make_message(TOPIC, PAYLOAD);
      // msg->set_qos(mqtt::GRANTED_QOS_0);

      // consumer.publish(msg);

      consumer.disconnect();
    } catch (const mqtt::exception &e) {
      std::cerr << "MQTT exception caught: " << e.what() << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Standard exception caught: " << e.what() << std::endl;
    } });

    consumer_thread.join();
    producer_thread.join();

    return EXIT_SUCCESS;
}
