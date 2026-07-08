import rclpy
import sys
from rclpy.node import Node
from rclpy.logging import get_logger
from base_interface.srv import Addints

class AddIntsClient(Node):
    def __init__(self):
        super().__init__('py_client')
        self.get_logger().info('PY客户端已启动')
        self.client_ = self.create_client(Addints, 'add_ints')

    def connect_service(self):
        while(not self.client_.wait_for_service(1.0)):
            self.get_logger().info('PY客户端等待服务启动中...')
        self.get_logger().info('PY客户端连接服务成功')
        return True
    
    def send_request(self, num1, num2):
        request = Addints.Request()
        request.num1 = num1
        request.num2 = num2
        self.future = self.client_.call_async(request)

def main():
    if len(sys.argv) != 3:
        get_logger('system').info('PY客户端需要两个参数，分别是num1和num2')
        return
    
    rclpy.init()
    
    client = AddIntsClient()
    flag = client.connect_service()
    if not flag:
        get_logger('system').info('PY客户端连接服务失败')
        return
    
    client.send_request(int(sys.argv[1]), int(sys.argv[2]))
    rclpy.spin_until_future_complete(client, client.future)
    if client.future.done():
        response = client.future.result()
        get_logger('system').info(f'PY客户端服务返回结果：{response.sum}')
    else:
        get_logger('system').info('PY客户端服务调用失败')
    
    rclpy.shutdown()
        
if __name__ == '__main__':
    main()
        
        
