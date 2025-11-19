#pragma once

#include <rclcpp/rclcpp.hpp>
#include <chrono>
#include <memory>
#include <array>

#include <pinocchio/parsers/urdf.hpp>
#include <pinocchio/algorithm/kinematics.hpp>
#include <pinocchio/algorithm/joint-configuration.hpp>
#include <pinocchio/algorithm/jacobian.hpp>
#include <pinocchio/algorithm/frames.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/pose_array.hpp> 
#include <ament_index_cpp/get_package_share_directory.hpp>
#include <Eigen/Dense>

#include "rh6_cmd/msg/rh6_cmd.hpp"
#include "rh6_msg/msg/rh6_msg.hpp"
#include "rh6_cmd/srv/rh6fk.hpp"
#include "rh6_cmd/srv/rh6ik.hpp"




namespace ruiyan::rh6
{
    class rh6_ctrl : public rclcpp::Node
    {
    public:
        rh6_ctrl( std::string name );

        void CmdCallback(const rh6_cmd::msg::Rh6Cmd::SharedPtr msg);
        void PubState();
        void UpdataMotor( void );

        void rh6fk_callback(const rh6_cmd::srv::Rh6fk::Request::SharedPtr request,const rh6_cmd::srv::Rh6fk::Response::SharedPtr response);
        void rh6ik( pinocchio::Model& model, pinocchio::Data& data, Eigen::VectorXd &q_ik, std::string ftip[5], const rh6_cmd::srv::Rh6ik::Request::SharedPtr request, const rh6_cmd::srv::Rh6ik::Response::SharedPtr response);
        void rh6ik_callback(const rh6_cmd::srv::Rh6ik::Request::SharedPtr request,const rh6_cmd::srv::Rh6ik::Response::SharedPtr response);

        void req_angle_to_model_q(const std::array<float, 11> &req, Eigen::VectorXd &q, int size);
        void model_q_to_res_angle(const Eigen::VectorXd &q, std::array<float, 11> &res );

    private:

        pinocchio::Model model_fk_,model_ik_,model_,model_l_,model_r_;
        pinocchio::Data data_fk_,data_ik_,data_,data_l_,data_r_;
        Eigen::VectorXd q_fk_,q_ik_, q_iik_, q_;
        std::string urdf_path,urdf_filename_l,urdf_filename_r;
        std::string fingertip_l_[5], fingertip_r_[5];
        std::string fingertip[5];
        std::string fingertip_fk[5];
        std::string fingertip_ik[5];

        double poly_coeff[5][4];
        int si;

        rh6_msg::msg::Rh6Msg rh6msg;
        rh6_cmd::msg::Rh6Cmd rh6cmd;


        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Publisher<rh6_msg::msg::Rh6Msg>::SharedPtr ryhand_state_publisher_;
        rclcpp::Subscription<rh6_cmd::msg::Rh6Cmd>::SharedPtr ryhand_cmd_subscriber_;


        // 声明服务回调组
        rclcpp::CallbackGroup::SharedPtr callback_group_service_;

        // 声明服务端
        rclcpp::Service<rh6_cmd::srv::Rh6fk>::SharedPtr server_rh6fk_;
        rclcpp::Service<rh6_cmd::srv::Rh6ik>::SharedPtr server_rh6ik_;


        rh6_cmd::srv::Rh6fk::Request req_fk;
        rh6_cmd::srv::Rh6fk::Response res_fk;

        rh6_cmd::srv::Rh6ik::Request req_ik;
        rh6_cmd::srv::Rh6ik::Response res_ik;


    };
}