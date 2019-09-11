#include <ros/ros.h>
#include <moveit_interface_example/MoveitPlanner.h>
#include <stdio.h>  

double J1 = -2.617994;
double J2 = -1.047198;
double J3 = 0.3141593;
double J4 = -2.96706;
double J5 = -1.658063;
double J6 = -2.96706;
double speed = 0.1;

void lower_upper();

void cobotta_move();

int main(int argc, char** argv)
{
	char key;

	ros::init(argc, argv, "moveit_interface_example");
	ros::NodeHandle node;

	//moveitの動作計画を使用するために必要
	ros::AsyncSpinner spinner(2);
	spinner.start();

	MoveitPlanner moveitPlanner("arm");
	while (true)
	{
		key = getchar();
		switch (key) {
		case '1':
			speed *= 0.1;
			ROS_INFO("Speed * 0.2");
			break;
		case '2':
			speed = 0.1;
			ROS_INFO("Speed initialized");
			break;
		case '3':
			speed *= 10;
			ROS_INFO("Speed * 5");
			break;

		case 'a':
			J1 += speed;
			cobotta_move();

			break;
		case 'z':
			J1 -= speed; 
			
			cobotta_move();
			break;

		case 's':
			J2 += speed;
			cobotta_move();

			break;
		case 'x':
			J2 -= speed;
			cobotta_move();

			break;

		case 'd':
			J3 += speed;
			cobotta_move();

			break;
		case 'c':
			J3 -= speed;
			cobotta_move();

			break;

		case 'f':
			J4 += speed;
			cobotta_move();

			break;
		case 'v':
			J4 -= speed;
			cobotta_move();

			break;

		case 'g':
			J5 += speed;
			cobotta_move();
			break;
		case 'b':
			J5 -= speed;
			cobotta_move();

			break;

		case 'h':
			J6 += speed;
			cobotta_move();
			break;
		case 'n':
			J6 -= speed;
			cobotta_move();

			break;

		default:
			ROS_INFO("Push 1,2,3 azsxdcfvgbhn");
			continue;
		}

		

		
	}

	ros::waitForShutdown();
	return 0;

}


void lower_upper() {
	if (J1 < -2.617994)
	{
		J1 = -2.617994;
		ROS_INFO("J1 lower");
	}

	if (J1 > 2.617994)
	{
		J1 = 2.617994;
		ROS_INFO("J1 upper");
	}

	if (J2 < -1.047198)
	{
		J2 = -1.047198;
		ROS_INFO("J2 lower");
	}

	if (J2 > 1.743461)
	{
		J2 = 1.743461;
		ROS_INFO("J2 upper");
	}

	if (J3 < 0.3141593)
	{
		J3 = 0.3141593;
		ROS_INFO("J3 lower");
	}

	if (J3 > 2.443461)
	{
		J3 = 2.443461;
		ROS_INFO("J3 upper");
	}

	if (J4 < -2.96706)
	{
		J4 = -2.96706;
		ROS_INFO("J4 lower");
	}

	if (J4 > 2.96706)
	{
		J4 = 2.96706;
		ROS_INFO("J4 upper");
	}

	if (J5 < -1.658063)
	{
		J5 = -1.658063;
		ROS_INFO("J5 lower");
	}

	if (J5 > 2.356194)
	{
		J5 = 2.356194;
		ROS_INFO("J5 upper");
	}

	if (J6 < -2.96706)
	{
		J6 = -2.96706;
		ROS_INFO("J6 lower");
	}

	if (J6 > 2.96706)
	{
		J6 = 2.96706;
		ROS_INFO("J6 upper");
	}

}

void cobotta_move() {
	lower_upper();

	const std::vector<double> jointRadians = { J1,J2,J3,J4,J5,J6 };
	const auto moveResult = moveitPlanner.moveByJointValues(jointRadians);

	if (moveResult == MoveItErrorCode::SUCCESS)
	{
		ROS_INFO("Succeeded to move cobotta");
	}
	else
	{
		ROS_WARN_STREAM("Failed to move cobotta. error code: " << moveResult);
	}
}
