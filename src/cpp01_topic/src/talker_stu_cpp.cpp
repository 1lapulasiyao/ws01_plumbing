#include "rclcpp/rclcpp.hpp"
#include "base_interface/msg/student.hpp"

using base_interface::msg::Student;
using namespace std::chrono_literals;

class TalkerStu : public rclcpp::Node
{
public:
    TalkerStu() : Node("talker_stu_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "CppStu发布者启动成功");
        publisher_ = this->create_publisher<Student>("student", 10);
        timer_ = this->create_wall_timer(1s, std::bind(&TalkerStu::timer_callback, this));
    }

private:
    rclcpp::Publisher<Student>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    void timer_callback()
    {
        auto message = Student();
        message.name = "张三";
        message.age = 18;
        message.height = 1.75;

        publisher_->publish(message);
        RCLCPP_INFO(this->get_logger(), "CppStu发布者发布消息: name=%s, age=%d, height=%.2f", 
                    message.name.c_str(), message.age, message.height);
    }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<TalkerStu>());
    
    rclcpp::shutdown();
    return 0;
}
