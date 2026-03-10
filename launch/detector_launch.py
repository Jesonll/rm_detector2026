from launch import LaunchDescription
from launch.actions import SetEnvironmentVariable
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    pkg_share = get_package_share_directory('rm_detector2026')
    
    config_file = os.path.join(pkg_share, 'config', 'detector_params.yaml')

    return LaunchDescription([
        SetEnvironmentVariable('HWLOC_HIDE_ERRORS', '1'),
        Node(
            package='rm_detector2026',
            executable='rm_detector2026_node',
            name='detector_node',
            parameters=[
                config_file
            ],
            output='screen'
        )
    ])
