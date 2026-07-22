#include "rclcpp/rclcpp.hpp"
#include "base_interface/action/progress.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

using base_interface::action::Progress;
using namespace std::chrono_literals;
using  std::placeholders::_1;
using  std::placeholders::_2;

class CppClient : public rclcpp::Node
{
public:
    CppClient(): Node("cpp_client")
    {
        RCLCPP_INFO(this->get_logger(), "CPP动作客户端启动成功");
        client_ = rclcpp_action::create_client<Progress>(this, "get_sum");
    }

    void send_goal(int num)
    {
        if(!client_->wait_for_action_server(10s))
        {
            RCLCPP_ERROR(this->get_logger(), "服务器连接失败");
            return;
        }
        RCLCPP_INFO(this->get_logger(), "服务器连接成功");
        auto goal = Progress::Goal();
        goal.num = num;
        rclcpp_action::Client<Progress>::SendGoalOptions options;
        options.goal_response_callback = std::bind(&CppClient::goal_response_callback,this,_1);
        options.feedback_callback = std::bind(&CppClient::feedback_callback,this,_1,_2);
        options.result_callback = std::bind(&CppClient::result_callback,this,_1);
        
        client_->async_send_goal(goal,options);
    }

    void goal_response_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr goal_handle)
    {
        if(!goal_handle)
        {
            RCLCPP_INFO(this->get_logger(), "提交失败,服务器拒绝");
            return;
        }
        RCLCPP_INFO(this->get_logger(), "提交成功,服务器接受");
    }

    void feedback_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr goal_handle,
                           const std::shared_ptr<const Progress::Feedback> feedback)
    {
        (void)goal_handle;
        auto progress = feedback->progress;
        int pro = (int)(progress*100);
        RCLCPP_INFO(this->get_logger(), "当前进度：%d%%",pro);
    }

    void result_callback(const rclcpp_action::ClientGoalHandle<Progress>::WrappedResult &result)
    {
        switch(result.code)
        {
        case rclcpp_action::ResultCode::SUCCEEDED:
            RCLCPP_INFO(this->get_logger(), "服务成功，结果：%d",result.result->sum);
            break;
        case rclcpp_action::ResultCode::ABORTED:
            RCLCPP_INFO(this->get_logger(), "服务被中断");
            break;
        case rclcpp_action::ResultCode::CANCELED:
            RCLCPP_INFO(this->get_logger(), "服务被取消");
            break;
        default:
            RCLCPP_INFO(this->get_logger(), "服务未知状态");
            break;
        }
    }

private:
    rclcpp_action::Client<Progress>::SharedPtr client_;
    
};

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        RCLCPP_INFO(rclcpp::get_logger("cpp_client"), "输入错误，请输入一个整数");
        return 1;
    }
    rclcpp::init(argc, argv);

    auto client = std::make_shared<CppClient>();
    client->send_goal(atoi(argv[1]));

    rclcpp::spin(client);

    rclcpp::shutdown();
    return 0;
}