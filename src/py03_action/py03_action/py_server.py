import rclpy
import time
from rclpy.node import Node
from rclpy.action import ActionServer, GoalResponse, CancelResponse
from rclpy.callback_groups import ReentrantCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from base_interface.action import Progress

class PyServer(Node):
    def __init__(self):
        super().__init__('py_server')
        self.get_logger().info('Python动作服务器已启动')
        self.server = ActionServer(self, Progress, 'get_sum',
                                   self.execute_callback,
                                   goal_callback=self.goal_callback,
                                   cancel_callback=self.cancel_callback,
                                   callback_group=ReentrantCallbackGroup())
    
    def goal_callback(self, goal_request):
        if goal_request.num < 1:
            self.get_logger().info('提交不合法，num小于1')
            return GoalResponse.REJECT
        self.get_logger().info('提交合法，开始服务')
        return GoalResponse.ACCEPT

    def cancel_callback(self, goal_handle):
        self.get_logger().info('接受取消请求')
        return CancelResponse.ACCEPT

    def execute_callback(self, goal_handle):
        self.get_logger().info('开始执行任务')
        num = goal_handle.request.num
        sum_val = 0
        feedback_msg = Progress.Feedback()
        result = Progress.Result()
        
        for i in range(1, num + 1):
            if goal_handle.is_cancel_requested:
                self.get_logger().info('服务被取消')
                result.sum = sum_val
                goal_handle.canceled()
                return result
            
            sum_val += i
            progress = i / num
            feedback_msg.progress = progress
            self.get_logger().info(f'当前进度：{progress:.2f}')
            goal_handle.publish_feedback(feedback_msg)
            
            time.sleep(1.0)
        
        if rclpy.ok():
            result.sum = sum_val
            self.get_logger().info(f'服务完成，结果：{sum_val}')
            goal_handle.succeed()
            return result


def main():
    rclpy.init()
    node = PyServer()
    executor = MultiThreadedExecutor()
    executor.add_node(node)
    executor.spin()
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()