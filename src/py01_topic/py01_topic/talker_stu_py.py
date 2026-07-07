import rclpy
from rclpy.node import Node
from base_interface.msg import Student

class TalkerStu(Node):
    def __init__(self):
        super().__init__('talker_stu_py')
        self.get_logger().info('PyStu发布者已启动')
        self.publisher_ = self.create_publisher(Student, 'student', 10)
        self.timer_ = self.create_timer(1.0, self.timer_callback)
        
    def timer_callback(self):
        msg = Student()
        msg.name = '小张'
        msg.age = 18
        msg.height = 1.75
        self.publisher_.publish(msg)
        self.get_logger().info('PyStu发布者发布消息: name=%s, age=%d, height=%.2f' 
                               % (msg.name, msg.age, msg.height))
        
        
def main():
    rclpy.init()

    rclpy.spin(TalkerStu())

    rclpy.shutdown()

if __name__ == '__main__':
    main()