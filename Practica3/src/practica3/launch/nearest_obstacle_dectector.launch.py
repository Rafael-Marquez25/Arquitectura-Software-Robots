import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    pkg_dir = get_package_share_directory('practica3')
    param_file = os.path.join(pkg_dir, 'config', 'params.yaml')

    detector_cmd = Node(
        package='practica3',
        executable='nearest_obstacle_detector_node',
        output='screen',
        parameters=[param_file],
        remappings=[('input_scan', 'scan_raw')],
    )

    ld = LaunchDescription()
    ld.add_action(detector_cmd)

    return ld
