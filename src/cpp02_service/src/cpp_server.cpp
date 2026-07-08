#include "rclcpp/rclcpp.hpp"
#include "base_interface/srv/addints.hpp"

using base_interface::srv::Addints;
using std::placeholders::_1;
using std::placeholders::_2;

class AddIntsService : public rclcpp::Node
{
public:
  AddIntsService() : Node("cpp_server")
  {
    RCLCPP_INFO(this->get_logger(), "CPP服务器启动成功");
    server_ = this->create_service<Addints>("add_ints",std::bind(&AddIntsService::add_ints,this,_1,_2));
  }

private:
  rclcpp::Service<Addints>::SharedPtr server_;
  
  void add_ints(const Addints::Request::SharedPtr request, Addints::Response::SharedPtr response)
  {
    response->sum = request->num1 + request->num2;
    RCLCPP_INFO(this->get_logger(), "CPP服务器处理请求，num1: %d, num2: %d, sum: %d", request->num1, request->num2, response->sum);
  }
};


int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<AddIntsService>());
  rclcpp::shutdown();
  return 0;
}
