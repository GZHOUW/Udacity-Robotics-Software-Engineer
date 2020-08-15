#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;

  // Initialize step counter for demo
  uint32_t step_counter_demo = 0;

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
        marker.pose.position.x = 2.0;
        marker.pose.position.y = 2.0;
        marker.pose.position.z = 0;
        marker.pose.orientation.x = 0.0;
        marker.pose.orientation.y = 0.0;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = 1.0;

        // Publish marker
        marker_pub.publish(marker);

        // Move to next state
        step_counter_demo++;
        break;

      // Pause 5 seconds
      // Hide the marker
      case 1:
        sleep(5);
        ROS_INFO("Hiding object.");
        // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
        marker.action = visualization_msgs::Marker::DELETE;

        // Move to next state
        step_counter_demo++;
        break;
      // Pause 5 seconds
      // Publish the marker at the drop off zone
      case 2:
        sleep(5);
        ROS_INFO("Placing object at the drop-off location.");
        // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
        marker.action = visualization_msgs::Marker::ADD;

        // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
        marker.pose.position.x = -2.0;
        marker.pose.position.y = 3.0;
        marker.pose.position.z = 0;
        marker.pose.orientation.x = 0.0;
        marker.pose.orientation.y = 0.0;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = 0.5;

        // Publish marker
        marker_pub.publish(marker);

        // Move to next state
        step_counter_demo++;
        break;

      // Demo finished
      case 3:
        ROS_INFO("Demo finished.");
        break;
      // Unknown step counter
      default:
        ROS_INFO("Unknown step counter.");
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

    r.sleep();
  }
}
