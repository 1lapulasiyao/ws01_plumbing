import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class Listener(Node):
    def __init__(self):
        super().__init__('listener_str_py')
        self.get_logger().info('PY订阅者已启动')
        self.subscription_ = self.create_subscription(String, 'chatter', self.listener_callback, 10)
    
    def listener_callback(self, msg):
        self.get_logger().info('PY订阅者收到消息: %s' % msg.data)
        

def main():
    rclpy.init()

    rclpy.spin(Listener())

    rclpy.shutdown()

if __name__ == '__main__':
    main()