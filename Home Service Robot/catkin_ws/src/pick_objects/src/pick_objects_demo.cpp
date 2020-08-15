#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/tf.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "pick_objects");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  // Publish 2 goals to reach and pause 5 seconds after reacing the pickup zone
  // Define goals
  int num_goals = 2;
  int goals[num_goals][3] = { {2.0, 2.0, 1.0}, {-2.0, 3.0, 1.0} };

  // Loop through all the goals
  for (int i = 0; i < num_goals; i++) {
    // set up the frame parameters
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();

    // Define a position and orientation for the robot to reach
    goal.target_pose.pose.position.x = goals[i][0];
    goal.target_pose.pose.position.y = goals[i][1];
    goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(goals[i][2]);

    // Send the goal position and orientation for the robot to reach
    ROS_INFO("Sending goal");
    ac.sendGoal(goal);

    // Wait an infinite time for the results
    ac.waitForResult();

    // Check if the robot reached its goal
    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
      ROS_INFO("Goal reached!");
    else
      ROS_INFO("Goal cannot be reached!");
    }

  return 0;
}
