import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    pkg_dir = get_package_share_directory('practica3')
    param_file = os.path.join(pkg_dir, 'config', 'params.yaml')

    detector_yolo_cmd = Node(
        package='camera',
        executable='yolo_detection',
        output='screen',
        parameters=[param_file],
        remappings=[
            ('input_detection', '/yolo/detections'),
            ('output_detection_2d', '/pub_example'),
        ],
    )

    detector_2d_cmd = Node(
        package='practica3',
        executable='detection_2d_node',
        output='screen',
        parameters=[param_file],
        remappings=[
            ('input_detection', '/pub_example'),
            ('output_detection_2d', '/detection_2d'),
        ],
    )

    detectoion_3d_cmd = Node(
        package='camera',
        executable='detection_2d_to_3d_depth',
        output='screen',
        parameters=[param_file],
        remappings=[
            ('input_depth', '/rgbd_camera/depth_image'),
            ('input_detection_2d', '/detection_2d'),
            ('camera_info', '/rgbd_camera/camera_info'),
            ('output_detection_3d', '/detection_3d_array'),
        ],
    )

    detector_3d_cmd = Node(
        package='practica3',
        executable='detection_3d_node',
        output='screen',
        parameters=[param_file],
        remappings=[
            ('input_detection_3d', '/detection_3d_array'),
        ],
    )

    ld = LaunchDescription()
    ld.add_action(detectoion_3d_cmd)
    ld.add_action(detector_3d_cmd)
    ld.add_action(detector_2d_cmd)
    ld.add_action(detector_yolo_cmd)

    return ld
