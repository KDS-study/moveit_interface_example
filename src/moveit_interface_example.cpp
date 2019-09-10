#include <ros/ros.h>
#include <moveit_interface_example/MoveitPlanner.h>

using namespace moveit::planning_interface;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "moveit_interface_example");
    ros::NodeHandle node;

    //moveitの動作計画を使用するために必要
    ros::AsyncSpinner spinner(2);
    spinner.start();

    MoveitPlanner moveitPlanner("move_group");

    const std::vector<double> jointRadians = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    const auto moveResult = moveitPlanner.moveByJointValues(jointRadians);

    if (moveResult == MoveItErrorCode::SUCCESS)
    {
        ROS_INFO("Succeeded to move cobotta");
    }
    else
    {
        ROS_WARN_STREAM("Failed to move cobotta. error code: " << moveResult);
    }

    ros::waitForShutdown();
    return 0;
}
