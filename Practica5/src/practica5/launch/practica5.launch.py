import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    pkg_dir = get_package_share_directory('practica5')
    param_file = os.path.join(pkg_dir, 'config', 'params.yaml')

    detector_cmd = Node(package='practica5',
                        executable='detection_node',
                        output='screen',
                        parameters=[param_file],
                        remappings=[
                          ('input_detection', '/yolo/detections'),
                        ])
    bt_node = Node(
          package='practica5',
          executable='bt_node',
          output='screen',
          parameters=[param_file],
          remappings=[])

    ld = LaunchDescription()
    ld.add_action(detector_cmd)
    ld.add_action(bt_node)

    return ld
