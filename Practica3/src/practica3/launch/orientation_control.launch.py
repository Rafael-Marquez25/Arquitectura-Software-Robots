import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch_ros.actions import Node

# from launch.actions import IncludeLaunchDescription
# from launch.launch_description_sources import PythonLaunchDescriptionSource


def generate_launch_description():
    pkg_dir = get_package_share_directory('practica3')
    param_file = os.path.join(pkg_dir, 'config', 'params.yaml')

    # tf_seeker_dir = get_package_share_directory('kobuki')

    # kobuki_cmd = IncludeLaunchDescription(
    #     PythonLaunchDescriptionSource(os.path.join(
    #         get_package_share_directory('kobuki'),
    #         'launch',
    #         'simulation.launch.py')),
    #         #launch_arguments={'world': tf_seeker_dir + '/worlds/empty.world'}.items()
    #         )

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
            ('input_depth', '/depth/image_raw'),
            ('input_detection_2d', '/detection_2d'),
            ('camera_info', '/depth/camera_info'),
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

    obstacle_detector_cmd = Node(
        package='practica3',
        executable='nearest_obstacle_detector_node',
        output='screen',
        parameters=[param_file],
        remappings=[('input_scan', 'scan_raw')],
    )

    orientation_control_cmd = Node(
        package='practica3',
        executable='orientation_control_node',
        output='screen',
        parameters=[param_file],
    )

    # rviz2_cmd = Node(package='rviz2',
    #                 executable='rviz2',
    #                 name='rviz2',
    #                 output='screen',
    #                 parameters=[
    #                     {'use_sim_time': True},
    #                 ])

    ld = LaunchDescription()
    # ld.add_action(kobuki_cmd)
    ld.add_action(detector_2d_cmd)
    ld.add_action(detectoion_3d_cmd)
    ld.add_action(detector_3d_cmd)
    ld.add_action(obstacle_detector_cmd)
    ld.add_action(orientation_control_cmd)
    # ld.add_action(rviz2_cmd)
    ld.add_action(detector_yolo_cmd)

    return ld
