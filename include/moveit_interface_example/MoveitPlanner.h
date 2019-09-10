#pragma once

#include <moveit/move_group_interface/move_group_interface.h>

class MoveitPlanner
{
public:
  MoveitPlanner(const std::string &moveGroupName);
  moveit::planning_interface::MoveItErrorCode moveByJointValues(const std::vector<double> &jointRadians);

private:
  moveit::planning_interface::MoveGroupInterface moveGroup;
};