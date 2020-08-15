#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>

// Initialize odometry position
float x_odom = 0.0;
float y_odom = 0.0;

// Define callback function for odometry
void odometry_cb(const nav_msgs::Odometry::ConstPtr& msg) {
  ::x_odom = msg->pose.pose.position.x;
  ::y_odom = msg->pose.pose.position.y;
  // ROS_INFO("In callback, current x_odom is %f, y_odom is %f", x_odom, y_odom);
}

int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  // Subscribe to odometry values
  ros::Subscriber odometry_sub = n.subscribe("/odom", 1000, odometry_cb);
  
  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;

  // Initialize step counter for demo
  uint32_t step_counter_demo = 0;

  // Initialize distances and tolerance
  float x_distance = 0.0;
  float y_distance = 0.0;
  float tolerance = 0.3;
  float radius = 0.0;

  // Publish 2 goals to reach and pause 5 seconds after reacing the pickup zone
  // Define goals
  int num_goals = 2;
  int goals[num_goals][3] = { {2.0, 2.0, 1.0}, {-2.0, 3.0, 1.0} };

  // Initialize marker
  visualization_msgs::Marker marker;

  // Set the frame ID and timestamp.  See the TF tutorials for information on these.
  marker.header.frame_id = "map";
  marker.header.stamp = ros::Time::now();
  
  // Set the namespace and id for this marker.  This serves to create a unique ID
  // Any marker sent with the same namespace and id will overwrite the old one
  marker.ns = "basic_shapes";
  marker.id = 0;

  // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
  marker.type = shape;
  
  // Set the scale of the marker -- 1x1x1 here means 1m on a side
  marker.scale.x = 1.0;
  marker.scale.y = 1.0;
  marker.scale.z = 1.0;

  // Set the color -- be sure to set alpha to something non-zero!
  marker.color.r = 0.0f;
  marker.color.g = 1.0f;
  marker.color.b = 0.0f;
  marker.color.a = 1.0;

  while (ros::ok())
  {
    // Publish the marker
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }
    
    marker.header.stamp = ros::Time::now();
    marker.lifetime = ros::Duration();
    
    // You will need to first run this node and visualize the markers in rviz
    // Then you'll need to modify the code and publish a single shape example: a cube.
    // Your code should follow this algorithm:
    //   Publish the marker at the pickup zone
    //   Pause 5 seconds
    //   Hide the marker
    //   Pause 5 seconds
    //   Publish the marker at the drop off zone
    switch (step_counter_demo) {
      // Publish the marker at the pickup zone
      case 0:
        ROS_INFO("Publishing object for pick up.");

        // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
        marker.action = visualization_msgs::Marker::ADD;

        // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
        marker.pose.position.x = goals[0][0];
        marker.pose.position.y = goals[0][1];
        marker.pose.position.z = 0;
        marker.pose.orientation.x = 0.0;
        marker.pose.orientation.y = 0.0;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = goals[0][2];
        
        // Publish marker
        marker_pub.publish(marker);

        // Move to next state
        step_counter_demo = 1;
        break;
      case 1:
        ROS_INFO("Waiting for robot to pick up object.");

        // Pause 1 seconds
        sleep(1);

        // Check distance between odometry and pick-up goal
        // If distance is within tolerance then move to next state, otherwise stays.
        x_distance = fabs(marker.pose.position.x - x_odom);
        y_distance = fabs(marker.pose.position.y - y_odom);
        radius = sqrt(pow(x_distance, 2) + pow(x_distance, 2));
        if (radius < tolerance) {
          // Move to next state
          step_counter_demo = 2;            
        }
        else {
          // Do nothing
        }
        ROS_INFO("Current x_odom is %f, y_odom is %f", x_odom, y_odom);
        ROS_INFO("Current radius is %f, tolerance is %f", radius, tolerance);
        break;
      case 2:
        ROS_INFO("Object was picked up.");
        ROS_INFO("Hiding object.");
        // Pause 2 seconds
        sleep(2);

        // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
        marker.action = visualization_msgs::Marker::DELETE;

        // Publish marker
        marker_pub.publish(marker);

        // Move to next state
        step_counter_demo = 3;
        break;
      case 3:
        ROS_INFO("Waiting for robot to drop off object.");

        // Pause 1 seconds
        sleep(1);

        // Check distance between odometry and drop-off goal
        // If distance is within tolerance then move to next state, otherwise stays.
        x_distance = fabs(goals[1][0] - x_odom);
        y_distance = fabs(goals[1][1] - y_odom);
        radius = sqrt(pow(x_distance, 2) + pow(x_distance, 2));
        if (radius < tolerance) {
          // Move to next state
          step_counter_demo = 4;            
        }
        else {
          // Do nothing
        }
        ROS_INFO("Current x_odom is %f, y_odom is %f", x_odom, y_odom);
        ROS_INFO("Current radius is %f, tolerance is %f", radius, tolerance);
        break;
      case 4:
        ROS_INFO("Object was dropped off.");
        ROS_INFO("Placing object at the drop-off location.");

        // Pause 2 seconds
        sleep(2);

        // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
        marker.action = visualization_msgs::Marker::ADD;

        // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
        marker.pose.position.x = goals[1][0];
        marker.pose.position.y = goals[1][1];
        marker.pose.position.z = 0;
        marker.pose.orientation.x = 0.0;
        marker.pose.orientation.y = 0.0;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = goals[1][2];

        // Publish marker
        marker_pub.publish(marker);

        // Move to next state
        step_counter_demo = 5;
        break;
      // Demo finished
      case 5:
        ROS_INFO("Demo finished.");
        break;
      // Unknown step counter
      default:
        ROS_INFO("Unknown step counter.");
        ROS_INFO("No action. Current step is %d", step_counter_demo);
    }

    /*
    // Cycle between different shapes
    switch (shape)
    {
    case visualization_msgs::Marker::CUBE:
      shape = visualization_msgs::Marker::SPHERE;
      break;
    case visualization_msgs::Marker::SPHERE:
      shape = visualization_msgs::Marker::ARROW;
      break;
    case visualization_msgs::Marker::ARROW:
      shape = visualization_msgs::Marker::CYLINDER;
      break;
    case visualization_msgs::Marker::CYLINDER:
      shape = visualization_msgs::Marker::CUBE;
      break;
    }
    */

    // Call single-threaded spinning
    ros::spinOnce();

    r.sleep();
  }
}
