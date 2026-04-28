import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch_ros.actions import Node

# from launch.actions import IncludeLaunchDescription
# from launch.launch_description_sources import PythonLaunchDescriptionSource


def generate_launch_description():
    pkg_dir = get_package_share_directory('practica4')
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
        package='practica4',
        executable='patrolling_robot_node',
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
    ld.add_action(detector_yolo_cmd)

    return ld
