#!/bin/bash
# sudo apt install ros-$ROS_DISTRO-pinocchio  # 安装pinocchio
workspace=$(pwd)
colcon build "$@"
source install/setup.bash       #  设置环境变量
# sudo cp ${workspace}/install/rh6_ctrl/lib/*.so /opt/ros/foxy/lib

# colcon build # 编译所有包
# colcon build --packages-select rh6_ctrl   # 编译指定包
# ros2 run  rh6_ctrl rh_ctrl   # 控制
# ros2 run  rh6_ctrl rh_test   # 测试 