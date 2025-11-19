"""Launch file to visualize RH6 URDF models in RViz2."""

import os
from typing import List

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def _load_urdf(model_path: str) -> str:
    if not os.path.exists(model_path):
        raise FileNotFoundError(f"URDF file not found: {model_path}")
    with open(model_path, "r", encoding="utf-8") as urdf_file:
        return urdf_file.read()


def _resolve_model_path(pkg_share: str, variant: str) -> str:
    mapping = {
        "ruihand6y": os.path.join(pkg_share, "urdf", "ruihand6y.urdf"),
        "ruihand6z": os.path.join(pkg_share, "urdf", "ruihand6z.urdf"),
    }
    if variant not in mapping:
        raise ValueError(
            f"Unknown hand variant '{variant}'. "
            f"可选项: {', '.join(sorted(mapping.keys()))}"
        )
    return mapping[variant]


def _launch_setup(context, pkg_share: str, *args, **kwargs) -> List[Node]:
    hand_variant = LaunchConfiguration("hand_variant").perform(context)
    model_path = LaunchConfiguration("urdf_file").perform(context)
    if not model_path:
        model_path = _resolve_model_path(pkg_share, hand_variant)
    robot_description = _load_urdf(model_path)

    use_sim_time = LaunchConfiguration("use_sim_time").perform(context).lower() == "true"
    rviz_config_path = LaunchConfiguration("rviz_config").perform(context)

    nodes: List[Node] = [
        Node(
            package="joint_state_publisher_gui",
            executable="joint_state_publisher_gui",
            name="joint_state_publisher_gui",
        ),
        Node(
            package="robot_state_publisher",
            executable="robot_state_publisher",
            name="robot_state_publisher",
            parameters=[
                {"robot_description": robot_description},
                {"use_sim_time": use_sim_time},
            ],
            output="screen",
        ),
    ]

    rviz_arguments: List[str] = []
    if rviz_config_path:
        rviz_arguments = ["-d", rviz_config_path]

    nodes.append(
        Node(
            package="rviz2",
            executable="rviz2",
            name="rviz2",
            arguments=rviz_arguments,
            output="screen",
        )
    )

    return nodes


def generate_launch_description() -> LaunchDescription:
    pkg_share = get_package_share_directory("rh6_ctrl")

    return LaunchDescription(
        [
            DeclareLaunchArgument(
                "hand_variant",
                default_value="ruihand6y",
                description="手型版本（默认 ruihand6y，可选 ruihand6z）",
            ),
            DeclareLaunchArgument(
                "urdf_file",
                default_value="",
                description="绝对路径；留空则根据 hand_variant 自动选择",
            ),
            DeclareLaunchArgument(
                "rviz_config",
                default_value="",
                description="可选：RViz2 配置文件（.rviz）的绝对路径",
            ),
            DeclareLaunchArgument(
                "use_sim_time",
                default_value="false",
                description="是否使用仿真时间 (use_sim_time)",
            ),
            OpaqueFunction(function=_launch_setup, kwargs={"pkg_share": pkg_share}),
        ]
    )

