# sudo ip link set can0 up type can bitrate 1000000
#  板子烧了slcan固件之后，接入ubuntu系统中，不需要任何驱动，都可以查看到设备： 


sudo apt install can-utils    # 安装can-utils
sudo apt install ros-${ROS_DISTRO}-plotjuggler-ros  # 安装plotjuggler-ros



sudo modprobe peak_usb     	#加载USB-CAN驱动
sudo modprobe can			#加载CAN驱动
sudo modprobe can-raw      	#加载CAN原始套接字驱动
sudo modprobe can-gw  		#加载CAN网关驱动
sudo modprobe slcan 		#加载串口CAN驱动
sudo modprobe vcan  		#加载虚拟CAN驱动
ls /dev/ttyACM*     		# 查看ttyACM* 设备
sudo slcand -o -c -s8 /dev/ttyACM0 can0   			# ttyACM0 设备映射为 can0，并设置速率为1000k
sudo ip link set can0 up type can bitrate 1000000   # 设置can0速率为1000000
sudo ip link set can0 txqueuelen 1000   			# 设置can0发送队列长度为1000
sudo ifconfig can0 up   							# 激活can0设备

sudo ifconfig can0 txqueuelen 1000   # 设置can0发送队列长度为1000
ip -d -s link show can0    			# 查看can0设备信息
	
# ls /dev/ttyACM*       (x代表可能是 0，1，2，3，.......)
# ttyACMx 设备映射为 can 设备，比如我这里 ttyACM0 映射为 can0，并设置速率:

	# sudo slcand -o -c -s8 /dev/ttyACM0 can0	(-s8 = 1000k  速率为1000K)

# 接着可以对can0 设备进行操作：

	# sudo ifconfig can0 up
	# sudo ifconfig can0 txqueuelen 1000

	# cansend can0 999#DEADBEEF   		# Send a frame to 0x999 with payload 0xdeadbeef
	# candump can0                			# Show all traffic received by can0
	# canbusload can0 500000      			# Calculate bus loading percentage on can0 
	# cansniffer can0             			# Display top-style view of can traffic
	# cangen can0 -D 11223344DEADBEEF -L 8    	# Generate fixed-data CAN messages
	# sudo ip link set dev can0 down
	# sudo ip link delete can0

# 其中： sudo slcand -o -c -s6 /dev/ttyACM0 can0  命令中的 -s6代表 500速率，其他速率参考：

# -s0 = 10k
# -s1 = 20k
# -s2 = 50k
# -s3 = 100k
# -s4 = 125k
# -s5 = 250k
# -s6 = 500k
# -s7 = 750k
# -s8 = 1M


