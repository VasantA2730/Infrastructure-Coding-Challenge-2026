#include <functional>
#include <memory>
#include <optional>
#include <string>

#include "rclcpp/node.hpp"
#include "rclcpp/subscription.hpp"
#include "rclcpp/executors.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;

class Greeter : public rclcpp::Node
{
public:
    Greeter()
    : Node("greeter")
    {
        greeting_subscription_ = this->create_subscription<std_msgs::msg::String>(
            "greeting", 10, std::bind(&Greeter::greeting_callback, this, _1)
        );

        greetee_subscription_ = this->create_subscription<std_msgs::msg::String>(
            "greetee", 10, std::bind(&Greeter::greetee_callback, this, _1)
        );

        output_publisher_ = this->create_publisher<std_msgs::msg::String>(
            "output", 10
        );
    }

private:
    void greeting_callback(const std_msgs::msg::String & msg)
    {
        greeting = msg.data;

        if (greetee) {
            fire_message();
        }
    }

    void greetee_callback(const std_msgs::msg::String & msg)
    {
        greetee = msg.data;

        if (greeting) {
            fire_message();
        }
    }

    void fire_message() {
        auto message = std_msgs::msg::String();
        message.data = *greeting + " " + *greetee;

        output_publisher_->publish(message);

        this->greeting = std::nullopt;
        this->greetee = std::nullopt;
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr greeting_subscription_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr greetee_subscription_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr output_publisher_;

    std::optional<std::string> greeting = std::nullopt;
    std::optional<std::string> greetee = std::nullopt;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Greeter>());
    rclcpp::shutdown();
    return 0;
}