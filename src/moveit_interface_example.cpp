#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <stdio.h>  
#include <termios.h>
#include <string.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include<thread>

using namespace std;
using namespace moveit::planning_interface;
using std::thread;

#define PI 3.14159265

void lower_upper(double j1p, double j2p, double j3p, double j4p, double j5p, double j6p);

int cobotta_move(int argc, char** argv, double j1p, double j2p, double j3p, double j4p, double j5p, double j6p);

void jointout(double j1p, double j2p, double j3p, double j4p, double j5p, double j6p);
void TcpThread1(double j1p, double j2p, double j3p, double j4p, double j5p, double j6p);

static struct termios old, current;


/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
	tcgetattr(0, &old); /* grab old terminal i/o settings */
	current = old; /* make new settings same as old settings */
	current.c_lflag &= ~ICANON; /* disable buffered i/o */
	if (echo) {
		current.c_lflag |= ECHO; /* set echo mode */
	}
	else {
		current.c_lflag &= ~ECHO; /* set no echo mode */
	}
	tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
	tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

/* Read 1 character without echo */
char getch(void)
{
	return getch_(0);
}

/* Read 1 character with echo */
char getche(void)
{
	return getch_(1);
}

double rad2deg(double radian)
{
	return radian * 180 / PI;
}

void error(char* msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char** argv)
{
	char *key;

	double J1 = -2.617994;
	double J2 = -1.047198;
	double J3 = 1.5708;
	double J4 = -2.96706;
	double J5 = -1.658063;
	double J6 = -2.96706;
	double speed = 0.0872665;

	double *j1p = &J1;
	double *j2p = &J2;
	double *j3p = &J3;
	double *j4p = &J4;
	double *j5p = &J5;
	double*j6p =  &J6;

	thread th1(TcpThread1, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);

	while (true)
	{
		*key = getche();

		switch (*key) {
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
			*j1p += speed;
			cobotta_move(argc, argv,*j1p,*j2p,*j3p,*j4p,*j5p,*j6p);
			break;

		case 'z':
			*j1p -= speed;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);
			break;

		case 's':
			*j2p += speed;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);

			break;
		case 'x':
			*j2p -= speed;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);

			break;

		case 'd':
			*j3p += speed;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);

			break;
		case 'c':
			*j3p -= speed;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);

			break;

		case 'f':
			*j4p += speed;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);
			break;

		case 'v':
			*j4p -= speed;	
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);
			break;

		case 'g':
			*j5p += speed;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);
			break;

		case 'b':
			*j5p -= speed;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);

			break;

		case 'h':
			*j6p += speed;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);
			break;

		case 'n':
			*j6p -= speed;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);

			break;

		case 'q':
			*j1p = 0;
			*j2p = 0;
			*j3p = 1.5708;
			*j4p = 0;
			*j5p = 0;
			*j6p = 0;


			speed = 0.1;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);
			break;

		case '0':
			break;

		default:
			ROS_INFO("Push 1,2,3 azsxdcfvgbhn q");
			continue;
		}
		jointout(*j1p, *j2p, *j3p, *j4p, *j5p, *j6p);

		
	}		

	ros::waitForShutdown();
	th1.join();
	return 0;

}

void TcpThread1(double j1p, double j2p, double j3p, double j4p, double j5p, double j6p) {
	struct sockaddr_in addr;
	struct sockaddr_in client;

	int len;
	int sock;

	int i;

	string Jinfo;

	/* ソケットの作成 */
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) {
		perror("ERROR opening socket");
		exit(1);
	}

	/* ソケットの設定 */
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = INADDR_ANY;

	bind(sock, (struct sockaddr*) & addr, sizeof(addr));

	/* TCPクライアントからの接続要求を待てる状態にする */
	listen(sock, 5);


	/* TCPクライアントからの接続要求を受け付ける */
	len = sizeof(client);
	sock = accept(sock, (struct sockaddr*) & client, (socklen_t*)& len);

	while (true) {
		Jinfo = "J" + to_string(*j1p) + "J" + to_string(*j2p) + "J" + to_string(*j3p) + "J" + to_string(*j4p) + "J" + to_string(*j5p) + "J" + to_string(*j6p);
		char JJ[Jinfo.length()];
		for (i = 0; i < sizeof(JJ); i++) {
			JJ[i] = *Jinfo[i];
		}

		write(sock, JJ, strlen(JJ));
	}

		/* TCPセッションの終了*/
		close(sock);

}

void stringtochar(string *Jinfo, char JJ[]) {
	
	
}

void jointout(double j1p, double j2p, double j3p, double j4p, double j5p, double j6p) {
	std::cout << "J1:" << rad2deg(*j1p) << "deg" << std::endl;
	std::cout << "J2:" << rad2deg(*j2p) << "deg" << std::endl;
	std::cout << "J3:" << rad2deg(*j3p) << "deg" << std::endl;
	std::cout << "J4:" << rad2deg(*j4p) << "deg" << std::endl;
	std::cout << "J5:" << rad2deg(*j5p) << "deg" << std::endl;
	std::cout << "J6:" << rad2deg(*j6p) << "deg" << std::endl;
}



void lower_upper(double j1p, double j2p, double j3p, double j4p, double j5p, double j6p) {
	if (*j1p < -2.617994)
	{
		*j1p = -2.617994;
		ROS_INFO("J1 lower");
	}

	if (*j1p > 2.617994)
	{
		*j1p = 2.617994;
		ROS_INFO("J1 upper");
	}

	if (*j2p < -1.047198)
	{
		*j2p = -1.047198;
		ROS_INFO("J2 lower");
	}

	if (*j2p > 1.743461)
	{
		*j2p = 1.743461;
		ROS_INFO("J2 upper");
	}

	if (*j3p < 0.3141593)
	{
		*j3p = 0.3141593;
		ROS_INFO("J3 lower");
	}

	if (*j3p > 2.443461)
	{
		*j3p = 2.443461;
		ROS_INFO("J3 upper");
	}

	if (*j4p < -2.96706)
	{
		*j4p = -2.96706;
		ROS_INFO("J4 lower");
	}

	if (*j4p > 2.96706)
	{
		*j4p = 2.96706;
		ROS_INFO("J4 upper");
	}

	if (*j5p < -1.658063)
	{
		*j5p = -1.658063;
		ROS_INFO("J5 lower");
	}

	if (*j5p > 2.356194)
	{
		*j5p = 2.356194;
		ROS_INFO("J5 upper");
	}

	if (*j6p < -2.96706)
	{
		*j6p = -2.96706;
		ROS_INFO("J6 lower");
	}

	if (*j6p > 2.96706)
	{
		*j6p = 2.96706;
		ROS_INFO("J6 upper");
	}

}

MoveItErrorCode moveByJointValues(MoveGroupInterface& moveGroup, const std::vector<double>& jointRadians) {
	if (!moveGroup.setJointValueTarget(jointRadians))
	{
		return moveit::planning_interface::MoveItErrorCode(moveit_msgs::MoveItErrorCodes::INVALID_GOAL_CONSTRAINTS);
	}
	return moveGroup.move();
}

int cobotta_move(int argc, char** argv, double j1p, double j2p, double j3p, double j4p, double j5p, double j6p) {
	lower_upper(*j1p, *j2p, *j3p, *j4p, *j5p, *j6p);

	ros::init(argc, argv, "moveit_interface_example");
	ros::NodeHandle node;

	//moveitの動作計画を使用するために必要
	ros::AsyncSpinner spinner(2);
	spinner.start();

	MoveGroupInterface moveGroupInterface("arm");

	moveGroupInterface.setMaxVelocityScalingFactor(1.0);
	const std::vector<double> jointRadians = { *j1p,*j2p,*j3p,*j4p,*j5p,*j6p };
	const auto moveResult =moveByJointValues(moveGroupInterface, jointRadians);

	if (moveResult == MoveItErrorCode::SUCCESS)
	{
		ROS_INFO("Succeeded to move cobotta");
	}
	else
	{
		ROS_WARN_STREAM("Failed to move cobotta. error code: " << moveResult);
	}
	return 0;
}
