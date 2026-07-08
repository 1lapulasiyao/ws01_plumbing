import rclpy
from rclpy.node import Node
from base_interface.srv import Addints

class AddIntsService(Node):
    def __init__(self):
        super().__init__('py_server')
        self.get_logger().info('PY服务器已启动')
        self.server=self.create_service(Addints,'add_ints',self.handle_add_ints)

    def handle_add_ints(self,request,response):
        response.sum=request.num1+request.num2
        self.get_logger().info(f'PY服务器处理请求，num1={request.num1}，num2={request.num2}，sum={response.sum}')
        return response

def main():
    rclpy.init()
    rclpy.spin(AddIntsService())
    rclpy.shutdown()

if __name__ == '__main__':
    main()
        
        