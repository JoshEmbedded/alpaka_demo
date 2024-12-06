#ifndef LASER_CALIBRATION_H
#define LASER_CALIBRATION_H

#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <ros/package.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/JointState.h>
#include <mutex>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH


class LaserCalibration
{
public:
    LaserCalibration(const std::string &group_name, ros::NodeHandle node_handle);

    ~LaserCalibration()
    {
        if (bag_open)
        {
            bag.close();
            ROS_INFO("Bag file closed.");
        }
        scan_sub.shutdown();
    };

    bool handlePlanError(const moveit::core::MoveItErrorCode& my_plan, const std::string planning);
    bool computeTrajectory(const geometry_msgs::PoseStamped &goal_pose, moveit::planning_interface::MoveGroupInterface::Plan &plan);
    bool robotMovement(const geometry_msgs::Pose &goal_pose);
    bool cartesianMovement(std::vector<geometry_msgs::Pose> &goal_poses, moveit::planning_interface::MoveGroupInterface::Plan &plan);
    geometry_msgs::Pose offsetMovement(geometry_msgs::Pose &pose, float X, float Y, float Z, float w, float x, float y, float z);
    bool sensorCalibration();
    void recordCalibration(geometry_msgs::Pose pose);
    void readCalibrationData();

private:
    ros::NodeHandle nh;
    moveit::planning_interface::MoveGroupInterface move_group;
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    rosbag::Bag bag;
    bool bag_open;
    void laserScanCallback(const sensor_msgs::LaserScan::ConstPtr &msg);
    void jointStateCallback(const sensor_msgs::JointState::ConstPtr &msg);
    ros::Subscriber scan_sub;
    ros::Subscriber joint_sub;
    std::mutex bag_mutex;  // Mutex to protect the bag file
    std::string bag_path;
};

#endif // LASER_CALIBRATION_H
