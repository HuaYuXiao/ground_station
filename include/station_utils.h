/***************************************************************************************************************************
* station_utils.h
*
* Author: Qyp
* Maintainer: Eason Hua
* Update Time: 2024.05.30
***************************************************************************************************************************/
#ifndef PROMETHEUS_STATION_UTILS_H
#define PROMETHEUS_STATION_UTILS_H

#include <Eigen/Eigen>
#include <math.h>
#include <string>
#include <easondrone_msgs/Message.h>
#include <easondrone_msgs/ControlCommand.h>
#include <easondrone_msgs/SwarmCommand.h>
#include <easondrone_msgs/DroneState.h>
#include <easondrone_msgs/PositionReference.h>
#include <easondrone_msgs/AttitudeReference.h>
#include <easondrone_msgs/ControlOutput.h>
#include <easondrone_msgs/LogMessageControl.h>
#include <easondrone_msgs/LogMessageDetection.h>
#include <easondrone_msgs/LogMessagePlanning.h>
#include <easondrone_msgs/DetectionInfo.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>

using namespace std;

#define NUM_POINT 2

namespace station_utils
{
// 五种状态机
enum MISSION_TYPE
{
    CONTROL,
    LAND,
    PLAN,
    TRACK,
};

// 打印上层控制指令  
void printf_command_control(const easondrone_msgs::ControlCommand& _ControlCommand){
    cout <<">>>>>>>>>>>>>>>>>>>>>>>> Control Command <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" <<endl;

    //固定的浮点显示
    cout.setf(ios::fixed);
    //setprecision(n) 设显示小数精度为n位
    cout<<setprecision(NUM_POINT);
    //左对齐
    cout.setf(ios::left);
    // 强制显示小数点
    cout.setf(ios::showpoint);
    // 强制显示符号
    cout.setf(ios::showpos);

    cout << "Source: [ "<< _ControlCommand.source << " ]  Command_ID: "<< _ControlCommand.Command_ID <<endl;

        string move_mode = "undefined";
        string move_frame = "undefined";

        switch(_ControlCommand.Mode){
        case easondrone_msgs::ControlCommand::Idle:
            if(_ControlCommand.Reference_State.yaw_ref == 999){
                cout << "Command: [ Idle + Arming + OFFBOARD ] " <<endl;
            }else{
                cout << "Command: [ Idle ] " <<endl;
            }
            
            break;

        case easondrone_msgs::ControlCommand::Takeoff:
            cout << "Command: [ Takeoff ] " <<endl;
            break;

        case easondrone_msgs::ControlCommand::Hold:
            cout << "Command: [ Hold ] " <<endl;
            break;

        case easondrone_msgs::ControlCommand::Land:
            cout << "Command: [ Land ] " <<endl;
            break;

        case easondrone_msgs::ControlCommand::Move:
            if(_ControlCommand.Reference_State.Move_mode == easondrone_msgs::PositionReference::XYZ_POS){
                move_mode = "XYZ_POS";
            }else if(_ControlCommand.Reference_State.Move_mode == easondrone_msgs::PositionReference::XY_POS_Z_VEL){
                move_mode = "XY_POS_Z_VEL";
            }else if(_ControlCommand.Reference_State.Move_mode == easondrone_msgs::PositionReference::XY_VEL_Z_POS){
                move_mode = "XY_VEL_Z_POS";
            }else if(_ControlCommand.Reference_State.Move_mode == easondrone_msgs::PositionReference::XYZ_VEL){
                move_mode = "XYZ_VEL";
            }else if(_ControlCommand.Reference_State.Move_mode == easondrone_msgs::PositionReference::TRAJECTORY){
                move_mode = "TRAJECTORY";
            }else if(_ControlCommand.Reference_State.Move_mode == easondrone_msgs::PositionReference::POS_VEL_ACC) {
                move_mode = "POS_VEL_ACC";
            }

            if(_ControlCommand.Reference_State.Move_frame == easondrone_msgs::PositionReference::ENU_FRAME){
                move_frame = "ENU_FRAME";
            }else if(_ControlCommand.Reference_State.Move_frame == easondrone_msgs::PositionReference::BODY_FRAME){
                move_frame = "BODY_FRAME";
            }

            cout << "Command: [Move]; Move_frame: [" << move_frame << "]; Move_mode: [" << move_mode << "]" << endl;

            cout << "POS " << _ControlCommand.Reference_State.position_ref[0] << " " <<
                            _ControlCommand.Reference_State.position_ref[1] << " " <<
                            _ControlCommand.Reference_State.position_ref[2] << endl;
            cout << "VEL " << _ControlCommand.Reference_State.velocity_ref[0] << " " <<
                            _ControlCommand.Reference_State.velocity_ref[1] << " " <<
                            _ControlCommand.Reference_State.velocity_ref[2]<< endl;
            cout << "ACC " << _ControlCommand.Reference_State.acceleration_ref[0] << " " <<
                            _ControlCommand.Reference_State.acceleration_ref[1] << " " <<
                            _ControlCommand.Reference_State.acceleration_ref[2]<< endl;
            cout << "Yaw "  << _ControlCommand.Reference_State.yaw_ref* 180/M_PI << endl;

            break;

        case easondrone_msgs::ControlCommand::Disarm:
            cout << "Command: [ Disarm ] " <<endl;

            break;
    }

    cout << endl;
}

// 打印无人机状态
void prinft_drone_state(const easondrone_msgs::DroneState& _Drone_state){
    cout <<">>>>>>>>>>>>>>>>>>>>>>>>   Drone State   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" <<endl;

    //固定的浮点显示
    cout.setf(ios::fixed);
    //setprecision(n) 设显示小数精度为n位
    cout<<setprecision(NUM_POINT);
    //左对齐
    cout.setf(ios::left);
    // 强制显示小数点
    cout.setf(ios::showpoint);
    // 强制显示符号
    cout.setf(ios::showpos);

    cout << "Time: " << _Drone_state.time_from_start <<" [s] ";

    //是否和飞控建立起连接
    if (_Drone_state.connected == true)
    {
        cout << " [ Connected ]";
    }
    else
    {
        cout << " [ Unconnected ]";
    }

    //是否上锁
    if (_Drone_state.armed == true)
    {
        cout << " [ Armed ]";
    }
    else
    {
        cout << " [ DisArmed ]";
    }

    //是否在地面
    if (_Drone_state.landed == true)
    {
        cout << " [ Ground ] ";
    }
    else
    {
        cout << " [ Air ] ";
    }

    cout << "[ " << _Drone_state.mode<<" ] " <<endl;

    cout << "POS " << _Drone_state.position[0] << " "<<
                    _Drone_state.position[1]<<" "<<
                    _Drone_state.position[2]<< endl;
    cout << "VEL " << _Drone_state.velocity[0] << " [m/s] "<<
                    _Drone_state.velocity[1]<<" [m/s] "<<
                    _Drone_state.velocity[2]<<" [m/s] "<<endl;
    cout << "Attitude [R P Y] : " << _Drone_state.attitude[0] * 180/M_PI <<" [deg] "<<
                                    _Drone_state.attitude[1] * 180/M_PI << " [deg] "<<
                                    _Drone_state.attitude[2] * 180/M_PI<<" [deg] "<<endl;
    cout << "Att_rate [R P Y] : " << _Drone_state.attitude_rate[0] * 180/M_PI <<" [deg/s] "<<
                                    _Drone_state.attitude_rate[1] * 180/M_PI << " [deg/s] "<<
                                    _Drone_state.attitude_rate[2] * 180/M_PI<<" [deg/s] "<<endl;

        cout << endl;
}

// 打印位置控制器输出结果
void prinft_attitude_reference(const easondrone_msgs::AttitudeReference& _AttitudeReference){
    //固定的浮点显示
    cout.setf(ios::fixed);
    //setprecision(n) 设显示小数精度为n位
    cout<<setprecision(NUM_POINT);
    //左对齐
    cout.setf(ios::left);
    // 强制显示小数点
    cout.setf(ios::showpoint);
    // 强制显示符号
    cout.setf(ios::showpos);

    cout << "Attitude_sp [R P Y]  : " << _AttitudeReference.desired_attitude[0] * 180/M_PI <<" [deg]  "<<
                                        _AttitudeReference.desired_attitude[1] * 180/M_PI << " [deg]  "<<
                                        _AttitudeReference.desired_attitude[2] * 180/M_PI<<" [deg] "<<endl;
    cout << "Throttle_sp [ 0-1 ]  : " << _AttitudeReference.desired_throttle <<endl;

        cout << endl;
}

// tracking error
Eigen::Vector3d tracking_error(const easondrone_msgs::DroneState& _Drone_state, const easondrone_msgs::ControlCommand& _ControlCommand)
{
    Eigen::Vector3d error;

    error[0] = sqrt(pow(_Drone_state.position[0] - _ControlCommand.Reference_State.position_ref[0],2)+
                pow(_Drone_state.position[1] - _ControlCommand.Reference_State.position_ref[1],2)+
                pow(_Drone_state.position[2] - _ControlCommand.Reference_State.position_ref[2],2));
    error[1] = sqrt(pow(_Drone_state.velocity[0] - _ControlCommand.Reference_State.velocity_ref[0],2)+
                pow(_Drone_state.velocity[1] - _ControlCommand.Reference_State.velocity_ref[1],2)+
                pow(_Drone_state.velocity[2] - _ControlCommand.Reference_State.velocity_ref[2],2));
    error[2] = 0;

    return error;
}

void prinft_ref_pose(const geometry_msgs::PoseStamped& ref_pose){
    cout <<">>>>>>>>>>>>>>>>>>>>>>> Ref Pose <<<<<<<<<<<<<<<<<<<<<<<<<<<<" <<endl;

    //固定的浮点显示
    cout.setf(ios::fixed);
    //setprecision(n) 设显示小数精度为n位
    cout<<setprecision(NUM_POINT);
    //左对齐
    cout.setf(ios::left);
    // 强制显示小数点
    cout.setf(ios::showpoint);
    // 强制显示符号
    cout.setf(ios::showpos);
      
    cout << "Ref_position [X Y Z] : " << ref_pose.pose.position.x <<" "<<
    ref_pose.pose.position.y <<" " <<
    ref_pose.pose.position.z << endl;

    cout << endl;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 其 他 函 数 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 

// 【获取当前时间函数】 单位：秒
float get_time_in_sec(const ros::Time& begin_time)
{
    ros::Time time_now = ros::Time::now();
    float currTimeSec = time_now.sec - begin_time.sec;
    float currTimenSec = time_now.nsec / 1e9 - begin_time.nsec / 1e9;
    return (currTimeSec + currTimenSec);
}

// 将四元数转换至(roll,pitch,yaw)  by a 3-2-1 intrinsic Tait-Bryan rotation sequence
// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
// q0 q1 q2 q3
// w x y z
Eigen::Vector3d quaternion_to_euler(const Eigen::Quaterniond &q)
{
    float quat[4];
    quat[0] = q.w();
    quat[1] = q.x();
    quat[2] = q.y();
    quat[3] = q.z();

    Eigen::Vector3d ans;
    ans[0] = atan2(2.0 * (quat[3] * quat[2] + quat[0] * quat[1]), 1.0 - 2.0 * (quat[1] * quat[1] + quat[2] * quat[2]));
    ans[1] = asin(2.0 * (quat[2] * quat[0] - quat[3] * quat[1]));
    ans[2] = atan2(2.0 * (quat[3] * quat[0] + quat[1] * quat[2]), 1.0 - 2.0 * (quat[2] * quat[2] + quat[3] * quat[3]));
    return ans;
}
}
#endif
