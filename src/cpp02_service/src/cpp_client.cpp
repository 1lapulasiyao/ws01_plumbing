#include "rclcpp/rclcpp.hpp"
#include "base_interface/srv/addints.hpp"

using base_interface::srv::Addints;
using namespace std::chrono_literals;

class AddIntsClient : public rclcpp::Node
{
public:
  AddIntsClient() : Node("cpp_client")
  {
    RCLCPP_INFO(this->get_logger(), "CPP客户端启动成功");
    client_ = this->create_client<Addints>("add_ints");
  }
  
  bool connect_service()
  {
    while (!client_->wait_for_service(1s))
    {
      if(!rclcpp::ok())
      {
        RCLCPP_INFO(rclcpp::get_logger("system"), "CPP客户端节点运行结束，连接服务失败，退出程序");
        return false;
      }
      RCLCPP_INFO(rclcpp::get_logger("system"), "CPP客户端等待服务启动中...");
    }
    return true;
  }

  auto send_request(int a, int b)
  {
    auto request = std::make_shared<Addints::Request>();
    request->num1 = a;
    request->num2 = b;
    return client_->async_send_request(request);
  }

private:
  rclcpp::Client<Addints>::SharedPtr client_;
};

int main(int argc, char **argv)
{
    if (argc != 3)
    {
      RCLCPP_INFO(rclcpp::get_logger("system"), "CPP客户端参数错误，需要指定两个整数");
      return 1;
    }

    rclcpp::init(argc, argv);

    auto client = std::make_shared<AddIntsClient>();
    bool flag = client->connect_service();

    if (!flag)
    {
      RCLCPP_INFO(rclcpp::get_logger("system"), "CPP客户端连接服务失败");
      return 1;
    }

    auto future = client->send_request(atoi(argv[1]), atoi(argv[2]));
    //处理响应结果
    if(rclcpp::spin_until_future_complete(client, future)==rclcpp::FutureReturnCode::SUCCESS)
    {
      auto result = future.get();
      RCLCPP_INFO(client->get_logger(), "CPP客户端收到服务响应，结果为：%d", result->sum);
    }
    else
    {
      RCLCPP_INFO(client->get_logger(), "CPP客户端等待服务响应超时");
    }

    rclcpp::shutdown();
    return 0;
}
