#include "rclcpp/rclcpp.hpp"
#include "base_interface/action/progress.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

using base_interface::action::Progress;
using std::placeholders::_1;
using std::placeholders::_2;

class CppServer : public rclcpp::Node
{
public:
  CppServer(): Node("cpp_server")
  {
    RCLCPP_INFO(this->get_logger(), "CPP动作服务器启动成功");
    server_ = rclcpp_action::create_server<Progress>(this, "get_sum",
    std::bind(&CppServer::handle_goal,this,_1,_2),
    std::bind(&CppServer::handle_cancel,this,_1),
    std::bind(&CppServer::handle_accepted,this,_1));
  }

  rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID & uuid,
                                    std::shared_ptr<const Progress::Goal> goal)
  {
    (void)uuid;
    if(goal->num <1)
    {
      RCLCPP_INFO(this->get_logger(),"提交不合法，num小于1");
      return rclcpp_action::GoalResponse::REJECT;
    }
    RCLCPP_INFO(this->get_logger(),"提交合法，开始服务");
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  rclcpp_action::CancelResponse handle_cancel(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle)
  {
    (void)goal_handle;
    RCLCPP_INFO(this->get_logger(),"接受取消请求");
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  void execute(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle)
  {
    (void)goal_handle;
    int num = goal_handle->get_goal()->num;
    int sum = 0;
    double progress =0;
    auto feedback = std::make_shared<Progress::Feedback>();
    auto result = std::make_shared<Progress::Result>();
    rclcpp::Rate rate(1.0);

    for(int i=1;i<=num;i++)
    {
      sum += i;
      progress = i/(double)num;
      feedback->progress = progress;
      RCLCPP_INFO(this->get_logger(),"当前进度：%.2f",progress);
      goal_handle->publish_feedback(feedback);
      if(goal_handle->is_canceling())
      {
        RCLCPP_INFO(this->get_logger(),"服务被取消");
        goal_handle->canceled(result);
        return;
      }
      rate.sleep();
    }
  
    if(rclcpp::ok())
    {
      result->sum = sum;
      RCLCPP_INFO(this->get_logger(),"服务完成，结果：%d",sum);
      goal_handle->succeed(result);
    }
  }

  void handle_accepted(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle)
  {
    std::thread(std::bind(&CppServer::execute,this,goal_handle)).detach();
  }

private:
  rclcpp_action::Server<Progress>::SharedPtr server_; 
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  rclcpp::spin(std::make_shared<CppServer>());

  rclcpp::shutdown();
  return 0;
}
