#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class Listener: public rclcpp::Node
{
public:
    Listener(): Node("listener_str_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "CPP订阅者启动成功");
        subscription_ = this->create_subscription<std_msgs::msg::String>("chatter", 10, 
            std::bind(&Listener::callback, this,std::placeholders::_1));
    }

private:
    void callback(const std_msgs::msg::String & msg)
    {
        RCLCPP_INFO(this->get_logger(), "CPP订阅者收到消息: %s", msg.data.c_str());
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<Listener>());
    
    rclcpp::shutdown();
    return 0;
}