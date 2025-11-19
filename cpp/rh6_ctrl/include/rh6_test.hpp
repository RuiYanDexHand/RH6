#pragma once

#include <rclcpp/rclcpp.hpp>
#include <chrono>
#include <memory>


#include "rh6_cmd/msg/rh6_cmd.hpp"



namespace ruiyan::rh6
{
    class rh6_test : public rclcpp::Node
    {
    public:

        rh6_test( std::string name );

        void PubCmd();

        float rad_to_deg(float rad);
        float deg_to_rad(float deg);

    private:

        int tick;
        int tspan_ms;

        rh6_cmd::msg::Rh6Cmd rh6cmd;
        rclcpp::TimerBase::SharedPtr timer_;

        rclcpp::Publisher<rh6_cmd::msg::Rh6Cmd>::SharedPtr ryhand_cmd_publisher_;


    };
}