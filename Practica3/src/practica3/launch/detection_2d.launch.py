import os

from ament_index_python.packages import get_package_share_directory
# from launch.actions import IncludeLaunchDescription
# from launch.launch_description_sources import PythonLaunchDescriptionSource


from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    pkg_dir = get_package_share_directory('practica3')
    param_file = os.path.join(pkg_dir, 'config', 'params.yaml')

    detector_cmd = Node(
        package='practica3',
        executable='detection_2d_node',
        output='screen',
        parameters=[param_file],
        remappings=[
            ('input_detection', '/pub_example'),
            ('output_detection_2d', '/detection_2d'),
        ],
    )

    # kobuki = get_package_share_directory('kobuki')  # kobuki es un string con el path

    # kobuki = IncludeLaunchDescription(              # aquí sobreescribes la variable anterior!
    #     PythonLaunchDescriptionSource(
    #         os.path.join(kobuki, 'launch', 'simulation.launch.py')  # kobuki ya no es el path!
    #     )
    # )

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

    # rviz2_cmd = Node(package='rviz2',
    #                 executable='rviz2',
    #                 name='rviz2',
    #                 output='screen',
    #                 parameters=[
    #                     {'use_sim_time': True},
    #                 ])

    ld = LaunchDescription()
    # ld.add_action(kobuki)
    ld.add_action(detector_yolo_cmd)
    # ld.add_action(rviz2_cmd)
    ld.add_action(detector_cmd)

    return ld
