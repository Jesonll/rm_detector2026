from launch import LaunchDescription
from launch.actions import SetEnvironmentVariable, DeclareLaunchArgument
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare
import os

def generate_launch_description():
    pkg_share = FindPackageShare('rm_detector2026')
    config_file = PathJoinSubstitution([pkg_share, 'config', 'detector_params.yaml'])

    declare_namespace = DeclareLaunchArgument("namespace", default_value="d1", description="Namespace for the detector node")
    declare_camera_name = DeclareLaunchArgument("camera_name", default_value="camera", description="Name of the camera topic")

    camera_name = LaunchConfiguration("camera_name")
    namespace = LaunchConfiguration("namespace")

    return LaunchDescription([
        declare_namespace,
        declare_camera_name,
        SetEnvironmentVariable('HWLOC_HIDE_ERRORS', '1'),
        Node(
            package='rm_detector2026',
            executable='rm_detector2026_node',
            name='detector_node',
            namespace=namespace,
            parameters=[
                config_file
            ],
            remappings=[
                ('/image_raw', ['/', camera_name, '/image_raw']),
                ('/detector/result', ['/', camera_name, '/detector/result'])
            ],
            output='screen'
        )
    ])
