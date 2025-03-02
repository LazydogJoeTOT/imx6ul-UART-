import serial
import time
import os

# 获取当前工作目录
script_dir = os.getcwd()
print("当前工作目录:", script_dir)

# 设置工作目录（如果需要）
os.chdir(script_dir)

# 文件路径
file_path = os.path.join('photo', 'badapple3.bin')  # 同目录下的photo文件夹中的badapple.bin文件

# 检查文件是否存在
if not os.path.exists(file_path):
    print(f"文件 {file_path} 不存在")
    exit(1)

# 配置串口参数
port = 'COM8'  # 串口端口号
baudrate = 115200  # 波特率
timeout = 5  # 超时时间

try:
    # 打开串口
    ser = serial.Serial(port, baudrate, timeout=timeout)
    print(f"串口 {port} 已打开")
    received_data = ser.read(ser.in_waiting)  # 读取缓冲区中的所有数据
    print(f"已接收数据: {received_data}")

    # 读取bin文件
    with open(file_path, 'rb') as file:  # 以二进制模式打开文件
        file_size = os.path.getsize(file_path)  # 获取文件大小
        print(f"已读取文件 {file_path}, 文件大小: {file_size} 字节")

        # 发送文件大小
        file_kb_size = int(file_size / 1024) 
        file_size_bytes = file_kb_size.to_bytes(4, byteorder='big')  # 转换为4字节的字节串
        ser.write(file_size_bytes)  # 发送文件大小


        # 分批次发送文件数据
        sent_bytes = 0
        chunk_size = 32  # 设定每次发送的字节数为32B
        while sent_bytes < file_size:
            chunk = file.read(chunk_size)  # 读取1KB的数据块
            ser.write(chunk)  # 发送数据块
            sent_bytes += len(chunk)  # 更新已发送字节数
            print(f"已发送 {sent_bytes}/{file_size} 字节", end='\r') 

            # 接收数据并检查
            while True:
                time.sleep(0.003)  # 等待，确保发送完毕
                received_data = ser.read(ser.in_waiting)  # 读取缓冲区中的所有数据
                #print(f"已接收数据: {received_data}")

                # 检查是否接收到0xFF
                if b'\xFF' in received_data:
                    # print("接收到0xFF，准备发送下一个数据块。")
                    break  # 如果接收到0xFF，就退出循环，准备发送下一个数据块
                else:
                    # print("未接收到0xFF，继续等待...")
                    continue  # 如果未接收到0xFF，就继续等待
                
        print(f"已发送 {sent_bytes}/{file_size} 字节")  # 补充输出最后一行
        print(f"文件数据发送完成: {file_path}")

    # 关闭串口
    ser.close()
    print(f"串口 {port} 已关闭")

except serial.SerialException as e:
    print(f"无法打开串口 {port}: {e}")
except Exception as e:
    print(f"发生错误: {e}")
