#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class Talker: public rclcpp::Node
{
public:
    Talker(): Node("talker_str_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "CPP发布者启动成功");
        publisher_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);
        timer_ = this->create_wall_timer(1s, std::bind(&Talker::timer_callback, this));
    }

private:
    void timer_callback()
    {
        auto message = std_msgs::msg::String();
        message.data = "Hello World!" + std::to_string(count_);
        publisher_->publish(message);
        count_++;
        RCLCPP_INFO(this->get_logger(), "CPP发布者发布消息: %s", message.data.c_str());
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    size_t count_ = 0;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<Talker>());

    rclcpp::shutdown();
    return 0;
}