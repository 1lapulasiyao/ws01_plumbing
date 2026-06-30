import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class Talker(Node):
    def __init__(self):
        super().__init__('talker_str_py')
        self.count = 0  
        self.get_logger().info('PY发布者已启动')
        self.publisher_ = self.create_publisher(String, 'chatter', 10)
        self.timer_ = self.create_timer(1.0, self.timer_callback)

    def timer_callback(self):
        msg = String()
        msg.data = 'Hello World(PY): %d' % self.count
        self.get_logger().info('PY发布者发布消息: %s' % msg.data)
        self.publisher_.publish(msg)
        self.count += 1



def main():
    rclpy.init()

    rclpy.spin(Talker())

    rclpy.shutdown()

if __name__ == '__main__':
    main()
    
