import rclpy
from rclpy.node import Node
from base_interface.msg import Student

class ListenerStu(Node):
    def __init__(self):
        super().__init__('listener_stu_py')
        self.get_logger().info('PyStu订阅者已启动')
        self.subscription_ = self.create_subscription(Student, 'student', self.listener_callback, 10)

    def listener_callback(self, msg):
        self.get_logger().info('PyStu订阅者收到消息: name=%s, age=%d, height=%.2f' 
                               % (msg.name, msg.age, msg.height))
        
def main():
    rclpy.init()

    rclpy.spin(ListenerStu())

    rclpy.shutdown()
    
if __name__ == '__main__':
    main()