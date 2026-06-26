#include "rclcpp/rclcpp.hpp"

class Talker: public rclcpp::Node
{
public:
    Talker(): Node("talker_str_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "CPP_STR发布者启动成功");
    }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<Talker>());

    rclcpp::shutdown();
    return 0;
}