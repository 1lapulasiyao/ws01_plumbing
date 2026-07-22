import rclpy
import sys
from rclpy.logging import get_logger
from rclpy.node import Node
from rclpy.action import ActionClient
from base_interface.action import Progress

class PyClient(Node):
    def __init__(self):
        super().__init__('py_client')
        self.get_logger().info('Python动作客户端已启动')
        self.client = ActionClient(self, Progress, 'get_sum')

    def send_goal(self, num):
        if not self.client.wait_for_server(timeout_sec=10.0):
            self.get_logger().error('服务器连接超时')
            return
        self.get_logger().info('服务器连接成功')
        goal = Progress.Goal()
        goal.num = num
        self.future = self.client.send_goal_async(goal,self.feedback_callback)
        self.future.add_done_callback(self.goal_and_result_callback)

    def feedback_callback(self, feedback_msg):
        progress = feedback_msg.feedback.progress
        self.get_logger().info(f'当前进度：{progress:.2f}')

    def goal_and_result_callback(self, future):
        goal_handle = future.result()
        if not goal_handle.accepted:
            self.get_logger().info('服务器拒绝任务')
            return
        self.get_logger().info('服务器接受任务')
        self.result_future = goal_handle.get_result_async()
        self.result_future.add_done_callback(self.result_callback)

    def result_callback(self, future):
        result = future.result().result.sum
        self.get_logger().info(f'任务完成，结果：{result}')

def main():
    if len(sys.argv) != 2:
        get_logger('py_client').error('请输入num参数')
        return
    
    rclpy.init()

    client = PyClient()
    client.send_goal(int(sys.argv[1]))

    rclpy.spin(client)
    rclpy.shutdown()
    
if __name__ == '__main__':
    main()
