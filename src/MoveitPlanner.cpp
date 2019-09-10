#include <moveit_interface_example/MoveitPlanner.h>
#include <moveit_msgs/MoveItErrorCodes.h>

MoveitPlanner::MoveitPlanner(const std::string &moveGroupName) : moveGroup(moveGroupName)
{
}

moveit::planning_interface::MoveItErrorCode MoveitPlanner::moveByJointValues(const std::vector<double> &jointRadians)
{
    if (!this->moveGroup.setJointValueTarget(jointRadians))
    {
        return moveit::planning_interface::MoveItErrorCode(moveit_msgs::MoveItErrorCodes::INVALID_GOAL_CONSTRAINTS);
    }
    return this->moveGroup.move();
}