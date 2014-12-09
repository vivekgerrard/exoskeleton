#include <string>
    #include <ros/ros.h>
    #include <stdio.h>
    #include <sensor_msgs/JointState.h>
    #include <tf/transform_broadcaster.h>
    #include <std_msgs/Int32.h>
    #include <geometry_msgs/Point.h>
double tilt;
double swivel;
 double height;


    void callBack(const geometry_msgs::Point::ConstPtr& a)
    {
           
           swivel = ((((double)(a->x))/1023)-0.76344)*3.75;
           tilt = ((((double)(a->y))/1023)*2.5);
           height = -((((double)(a->z))/1023)*4.5)+0.45;
     //printf("I heard [%f],[%f],[%f]", swivel, tilt, height);

     
}



      
    int main(int argc, char** argv)
 {
 
         ros::init(argc, argv, "exoskeleton");
         ros::NodeHandle n;
   ros::Subscriber pot_input1 = n.subscribe("joint1", 1000, callBack);  
   ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 1);
   


     tf::TransformBroadcaster broadcaster;
      ros::Rate loop_rate(30);
  
       
       // robot state
     
   
       // message declarations
       geometry_msgs::TransformStamped odom_trans;
       sensor_msgs::JointState joint_state;
      odom_trans.header.frame_id = "odom";
       odom_trans.child_frame_id = "base";
   
       while (ros::ok()) 
{
           //update joint_state
           joint_state.header.stamp = ros::Time::now();
           joint_state.name.resize(3);
           joint_state.position.resize(3);
           joint_state.name[0] ="joint1";
           joint_state.position[0] = swivel;
		 joint_state.name[1] ="joint2";
           joint_state.position[1] = tilt;
		 joint_state.name[2] ="joint3";
		 joint_state.position[2]= height;
		
   

   
   
           // update transform
           // (moving in a circle with radius=2)
           odom_trans.header.stamp = ros::Time::now();
		 odom_trans.transform.rotation = tf::createQuaternionMsgFromRollPitchYaw((M_PI/2), 0, 0);

   
          //send the joint state and transform
           joint_pub.publish(joint_state);
          broadcaster.sendTransform(odom_trans);
         
   
           
            
           //tilt += 0.01;
           //swivel += 0.01;
		 //height += 0.01;




           
           // This will adjust as needed per iteration
           loop_rate.sleep();
		 ros::spinOnce();

}

            
           //tilt += 0.01;
           //swivel += 0.01;
		 //height += 0.01;




           

       
     return 0;
 }

