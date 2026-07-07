#include "rclcpp/rclcpp.hpp"
#include "base_interface/msg/student.hpp"

using base_interface::msg::Student;

class ListenerStu : public rclcpp::Node
{
public:
    ListenerStu() : Node("listener_stu_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "CppStu订阅者启动成功");
        subscription_ = this->create_subscription<Student>("student",10,std::bind(
            &ListenerStu::stu_callback, this, std::placeholders::_1
        ));
    }

private:
    rclcpp::Subscription<Student>::SharedPtr subscription_;

    void stu_callback(const Student & msg)
    {
        RCLCPP_INFO(this->get_logger(), "CppStu订阅者收到消息: name=%s, age=%d, height=%.2f", 
                    msg.name.c_str(), msg.age, msg.height);
    }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<ListenerStu>());
    
    rclcpp::shutdown();
    return 0;
}
