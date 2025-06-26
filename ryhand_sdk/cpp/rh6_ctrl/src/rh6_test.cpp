#include "rh6_test.hpp"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>
#include <string>
#include <sched.h>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>


#define  MOTOR_NUM 6

namespace ruiyan::rh6
{
    rh6_test::rh6_test( std::string name ) : Node( name )
    {
        RCLCPP_INFO(this->get_logger(), "hello %s",name.c_str());

        rh6cmd = rh6_cmd::msg::Rh6Cmd();
        rh6cmd.mode = 0;
        rh6cmd.lr = 0;
        tick = 0;
        tspan_ms = 5;

        for(int i = 0; i < MOTOR_NUM; i++)
        {
            rh6cmd.m_pos[i] = 4095;
            rh6cmd.m_spd[i] = 1000;
            rh6cmd.m_curlimit[i] = 1000;
        }

        // 创建发布器 - 命令
        ryhand_cmd_publisher_ = this->create_publisher<rh6_cmd::msg::Rh6Cmd>( "ryhand6_cmd", 1 );
        rclcpp::Publisher<rh6_cmd::msg::Rh6Cmd>::SharedPtr ryhand_cmd_publisher_;
        
        // 创建定时器 tspan_ms ms 
        timer_ = this->create_wall_timer(std::chrono::milliseconds(tspan_ms), std::bind(&rh6_test::PubCmd, this));
    }


    float rh6_test::rad_to_deg(float rad) 
    {
        return rad * 180 / M_PI;
    }


    float rh6_test::deg_to_rad(float deg) 
    {
        return deg * M_PI / 180;
    }


    void rh6_test::PubCmd()
    {
        float p1,p2,p3,p4;      // p5,p6,j; 
        float period = 10000;   // 周期 ms
        float amplitude = 1500; // 振幅为 1500
        float fs = sin( 2 * M_PI * tick / period  );
        float fc = cos( 2 * M_PI * tick / period  );

        // 定义主动序列和被动序列
        std::vector<int> active_sequence = {0, 1, 3, 5, 7, 9};
        // std::vector<int> passive_sequence = {2, 4, 6, 8, 10};

        switch (rh6cmd.mode)
        {
            // raw cmd
            case 0:
                // 生成幅值为 amplitude 的正弦波，周期为 periods
                for(int i = 0; i < MOTOR_NUM; i++)
                {
                        p1 = amplitude + amplitude  * fs;
                        p2 = 1000 * 1000 * (amplitude * 4) / 4095 * fc / period + 800;  // 600+

                        if(i<=1)
                        {
                            rh6cmd.m_pos[i] = 0;
                            rh6cmd.m_spd[i] = 1000;
                        }
                        else
                        {
                            rh6cmd.m_pos[i] = p1;
                            rh6cmd.m_spd[i] = p2;
                        }

                }
                break;


            // angle cmd
            case 1:
                for( int i = 0; i < 11; i++ )
                {
                    // 如果 i 是主动序列中的元素
                    if (std::find(active_sequence.begin(), active_sequence.end(), i) != active_sequence.end())
                    {
                        if (i == 0)
                        {
                            p1 = 40 + 40 * fs;
                        }
                        else if (i == 1)
                        {
                            p1 = 10 + 10 * fs;
                        }
                        else 
                        {
                            p1 = 30 + 30 * fs;
                        }
                        rh6cmd.j_ang[ i ] = deg_to_rad( p1 );
                    }
                    else
                    {
                        rh6cmd.j_ang[ i ] = rh6cmd.j_ang[ i-1 ];
                    }
                }
                break;

            // end pos cmd    
            case 2:

                // float64 x_base
                // float64 y_base
                // float64 z_base

                // float64 roll_base
                // float64 pitch_base
                // float64 yaw_base

                // float64[5] x
                // float64[5] y
                // float64[5] z

                // float64[5] roll
                // float64[5] pitch
                // float64[5] yaw

                // double end_pos[6] = {msg->end_pos[0], msg->end_pos[1], msg->end_pos[2], msg->end_pos[3], msg->end_pos[4], msg->end_pos[5]};
                // Eigen::Isometry3d transform = solve::Xyzrpy2Isometry(end_pos);
                // interfaces_ptr_->setEndPose(transform);
                // std::vector<double> joint_positions = {0, 0, 0, 0, 0, 0};
        
                // for (int i = 0; i < 6; i++)
                // {
                //     joint_positions[i] = msg->joint_pos[i];
                // }

                break;

            default:
                break;
        }
         
        tick = (tick + tspan_ms)%100000;

        // 发布消息
        ryhand_cmd_publisher_->publish(rh6cmd);
    }


}




int main(int argc, char *argv[])
{
    // ROS2 初始化
    rclcpp::init(argc, argv);

    auto node =  std::make_shared<ruiyan::rh6::rh6_test>("rh6_test");
    rclcpp::spin(node);
    rclcpp::shutdown();


    return 0;
}



