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

void lower_upper();

int cobotta_move(int argc, char** argv, int* j1p, int* j2p, int* j3p, int* j4p, int* j5p, int* j6p);

void jointout();
void TcpThread1(int* j1p, int* j2p, int* j3p, int* j4p, int* j5p, int* j6p);

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

	int* j1p = J1;
	int* j2p = J2;
	int* j3p = J3;
	int* j4p = J4;
	int* j5p = J5;
	int* j6p = J6;

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
			J1 += speed;
			*j1p = J1;
			cobotta_move(argc, argv,*j1p,*j2p,*j3p,*j4p,*j5p,*j6p);
			break;
		case 'z':
			J1 -= speed;
			*j1p = J1;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);
			break;

		case 's':
			J2 += speed;
			*j2p = J2;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);

			break;
		case 'x':
			J2 -= speed;
			*j2p = J2;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);

			break;

		case 'd':
			J3 += speed;
			*j3p = J3;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);

			break;
		case 'c':
			J3 -= speed;
			*j3p = J3;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);

			break;

		case 'f':
			J4 += speed;
			*j4p = J4;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);
			break;

		case 'v':
			J4 -= speed;	
			*j4p = J4;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);
			break;

		case 'g':
			J5 += speed;
			*j5p = J5;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);
			break;

		case 'b':
			J5 -= speed;
			*j5p = J5;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);

			break;

		case 'h':
			J6 += speed;
			*j6p = J6;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);
			break;

		case 'n':
			J6 -= speed;
			*j6p = J6;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);

			break;

		case 'q':
			J1 = 0;
			J2 = 0;
			J3 = 1.5708;
			J4 = 0;
			J5 = 0;
			J6 = 0;

			*j1p = J1;
			*j2p = J2;
			*j3p = J3;
			*j4p = J4;
			*j5p = J5;
			*j6p = J6;

			speed = 0.1;
			cobotta_move(argc, argv, *j1p, *j2p, *j3p, *j4p, *j5p, *j6p);
			break;

		case '0':
			break;

		default:
			ROS_INFO("Push 1,2,3 azsxdcfvgbhn q");
			continue;
		}
		jointout();

		
	}		

	ros::waitForShutdown();
	th1.join();
	return 0;

}

void TcpThread1(int *j1p, int* j2p, int* j3p, int* j4p, int* j5p, int* j6p ) {
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

void jointout() {
	std::cout << "J1:" << rad2deg(J1) << "deg" << std::endl;
	std::cout << "J2:" << rad2deg(J2) << "deg" << std::endl;
	std::cout << "J3:" << rad2deg(J3) << "deg" << std::endl;
	std::cout << "J4:" << rad2deg(J4) << "deg" << std::endl;
	std::cout << "J5:" << rad2deg(J5) << "deg" << std::endl;
	std::cout << "J6:" << rad2deg(J6) << "deg" << std::endl;
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

MoveItErrorCode moveByJointValues(MoveGroupInterface& moveGroup, const std::vector<double>& jointRadians) {
	if (!moveGroup.setJointValueTarget(jointRadians))
	{
		return moveit::planning_interface::MoveItErrorCode(moveit_msgs::MoveItErrorCodes::INVALID_GOAL_CONSTRAINTS);
	}
	return moveGroup.move();
}

int cobotta_move(int argc, char** argv,int *j1p,int *j2p,int *j3p,int *j4p,int *j5p,int *j6p) {
	lower_upper();

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
