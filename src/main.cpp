
#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "geometry_msgs/Twist.h"
#include <time.h>
#include <math.h>

#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Bool.h"

//constant setup variables change those values here
#define NODE_NAME "torque_step_sysid"
#define SUBSCRIBE_POWER "motor_power" //publishing channel
#define SUBSCRIBE_WHEEL_VEL "wheel_velocity"
#define BUFFER_SIZE 5
#define POWER_BUFFER_SIZE 200
#define LOOP_FREQ 20


geometry_msgs::Twist pwr_msg;
ros::Subscriber motor_power_sub;
ros::Subscriber wheel_vel_sub;

float current_L_vel = 0;
float current_R_vel = 0;
float current_L_pwr = 0;
float current_R_pwr = 0;


float t_elapsed = 0;
int loop_times = 0;


void encoderCallback(const std_msgs::Float32MultiArray::ConstPtr& array)
{
    current_L_vel = array->data[0];
    current_R_vel = array->data[1];
}
void motorPowerCallback(const geometry_msgs::Twist::ConstPtr & msg)
{

	current_R_pwr = msg->linear.y;
	current_L_pwr = msg->linear.x;

}

int main(int argc, char **argv)
{

  ros::init(argc, argv, NODE_NAME);

  ros::NodeHandle n;
  ros::Rate loop_rate(LOOP_FREQ);
  
  //set up communication channels
  // TODO add the other channels
  motor_power_sub = n.subscribe<geometry_msgs::Twist>(SUBSCRIBE_POWER, POWER_BUFFER_SIZE, motorPowerCallback);
  wheel_vel_sub = n.subscribe<std_msgs::Float32MultiArray>(SUBSCRIBE_WHEEL_VEL, BUFFER_SIZE, encoderCallback);
  
  float updatefreq = LOOP_FREQ;

  ROS_INFO("Data in format: t, L_pow, R_pow, L_vel, R_vel");

  while(ros::ok()){

  	t_elapsed = (float)loop_times/LOOP_FREQ ;

	ROS_INFO("%f,%f,%f,%f,%f",t_elapsed, current_L_pwr, current_R_pwr, current_L_vel, current_R_vel);
  	
  	loop_times++;
  	ros::spinOnce();
  	loop_rate.sleep();


  }

  return 0;
}