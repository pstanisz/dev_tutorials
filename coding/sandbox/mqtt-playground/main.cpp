#include <iostream>
#include <thread>
#include <format>

#include <mqtt/client.h>

const std::string TOPIC = "Status";

int main(int /*argc*/, char * /*argv*/[])
{
  std::cout << "mqtt-playground\n";

  const std::string address = "[::1]:1883";
  const std::string producer_id = "producer";
  const std::string consumer_id = "consumer";

  mqtt::client consumer(address, consumer_id);

  auto con_opts = mqtt::connect_options_builder().clean_session(false).finalize();

  mqtt::client producer(address, producer_id,
                        mqtt::create_options(mqtt::MQTTPP_V3_CODE));

  std::jthread producer_thread([&producer, &con_opts]()
                               {
    try {
      std::cout << "Producer thread started\n";

      producer.connect(con_opts);

      for (size_t idx = 0; idx < 10; ++idx) {

        auto payload = std::format("payload {}", idx);
        auto msg = mqtt::make_message(TOPIC, payload);
        msg->set_qos(mqtt::GRANTED_QOS_0);

        std::cout << "Producer\nTopic:\t\t" << TOPIC << "\nMessage:\t" << payload << "\n";

        producer.publish(msg);

        std::this_thread::sleep_for(std::chrono::seconds(1));
      }

      auto msg = mqtt::make_message(TOPIC, "exit");
      msg->set_qos(mqtt::GRANTED_QOS_0);
      std::cout << "Producer\nTopic:\t\t" << TOPIC << "\nMessage:\t" << "exit" << "\n";
      producer.publish(msg);

      producer.disconnect();
      std::cout << "Producer thread ended\n";

    } catch (const mqtt::exception &e) {
      std::cerr << "MQTT exception caught: " << e.what() << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Standard exception caught: " << e.what() << std::endl;
    } });

  std::jthread consumer_thread([&consumer, &con_opts]()
                               {
    try {
      std::cout << "Consumer thread started\n";

      consumer.connect(con_opts);
      consumer.subscribe(TOPIC);
      consumer.start_consuming();

      while (true) {
        auto msg = consumer.consume_message();
        if (!msg) {
          break;
        }

        std::cout << "Consumer\nTopic:\t\t" << msg->get_topic() << "\nMessage:\t" << msg->to_string() << "\n";

        if (msg->to_string() == "exit") {
          break;
        }
      }

      consumer.disconnect();
      std::cout << "Consumer thread ended\n";

    } catch (const mqtt::exception &e) {
      std::cerr << "MQTT exception caught: " << e.what() << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Standard exception caught: " << e.what() << std::endl;
    } });

  consumer_thread.join();
  producer_thread.join();

  return EXIT_SUCCESS;
}
