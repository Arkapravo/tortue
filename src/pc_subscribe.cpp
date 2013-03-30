#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <boost/foreach.hpp>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/PointCloud2.h>

typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;
float min_dist[3];
int i;
ros::Publisher vel_pub;
geometry_msgs::Twist cmdvel;

void callback(const PointCloud::ConstPtr& msg)
{
  i=0;
 // printf ("Cloud: width = %d, height = %d\n", msg->width, msg->height);
  BOOST_FOREACH (const pcl::PointXYZ& pt, msg->points)  //it works like a loop which assignes the values 1 by 1 to a variable from an array
{

    if( !pcl_isfinite( pt.x ) || !pcl_isfinite( pt.y ) || !pcl_isfinite( pt.z ) )  // this is done so that the nan values from the pointcloud are not processed
        continue;    

  if (i==0) 
  {
	min_dist[0]=pt.x;
	min_dist[1]=pt.y;
	min_dist[2]=pt.z;
	//printf("First run");
  }

  else if (pt.z<min_dist[2])// && pt.z>0)
  {
        min_dist[0]=pt.x;
	min_dist[1]=pt.y;
	min_dist[2]=pt.z;
	//printf("Koushik\n");
  }
  i++;
//printf ("\t(%f, %f, %f)\n", pt.x, pt.y, pt.z);
}

printf("Minimim distancs point :: x = %f \t , y = %f \t , z = %f \n", min_dist[0],min_dist[1],min_dist[2]);

if(min_dist[0] > 0 && min_dist[2]<=0.65) 
{
	printf("Turn Left\n");
	cmdvel.angular.z=0.30;
	cmdvel.linear.x=0;
}
else if(min_dist[0] < 0 && min_dist[2]<=0.65)
{
	printf("Turn Right\n");
	cmdvel.angular.z=-0.30;
	cmdvel.linear.x=0;
}
else
{
	printf("Go Straight\n");
	cmdvel.linear.x=0.20;
	cmdvel.angular.z=0;
}
	vel_pub.publish(cmdvel);
}
int main(int argc, char** argv)
{
  ros::init(argc, argv, "sub_pcl");
  ros::NodeHandle nh,nh1;
  vel_pub = nh1.advertise<geometry_msgs::Twist>("cmd_vel",1);
  cmdvel.linear.x=0;
  cmdvel.linear.y=0;
  cmdvel.linear.z=0;
  cmdvel.angular.x=0;
  cmdvel.angular.y=0;
  cmdvel.angular.z=0; 	
  ros::Subscriber sub = nh.subscribe<PointCloud>("camera/rgb/points", 1, callback);
  ros::spin();
}


