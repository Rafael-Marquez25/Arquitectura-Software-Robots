// Copyright 2026 Rafael Márquez
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "practica3/Detection3DNode.hpp"

#include "vision_msgs/msg/object_hypothesis_with_pose.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

using std::placeholders::_1;

Detection3DNode::Detection3DNode()
: Node("detection_3d_node"),
  tf_buffer_(this->get_clock()),
  tf_listener_(tf_buffer_)
{
  detection3d_sub_ = create_subscription<vision_msgs::msg::Detection3DArray>(
    "input_detection_3d", rclcpp::SensorDataQoS().reliable(),
    std::bind(&Detection3DNode::detection_callback, this, _1));

  detection_3d_pub_ = create_publisher<vision_msgs::msg::Detection3D>("/detection_3d", 10);

  tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*this);

}

void
Detection3DNode::detection_callback(const vision_msgs::msg::Detection3DArray::ConstSharedPtr & msg)
{
  if (!msg->detections.empty()) {
    vision_msgs::msg::Detection3D detection;
    if (!tracking_) {
      // en la primera detección, elige la de mayor score
      auto best = std::max_element(msg->detections.begin(), msg->detections.end(),
          [](const auto & a, const auto & b) {
            return a.results[0].hypothesis.score < b.results[0].hypothesis.score;
        });
      tracked_person_.point.x = best->bbox.center.position.x;
      tracked_person_.point.y = best->bbox.center.position.y;
      tracked_person_.point.z = best->bbox.center.position.z;
      tracking_ = true;
      detection = *best;
      detection_3d_pub_->publish(detection);

    } else {
      // si ya está siguiendo a alguien, busca la detección más cercana a la última posición
      auto closest = std::min_element(msg->detections.begin(), msg->detections.end(),
          [this](const auto & a, const auto & b) {
            double da = pow(a.bbox.center.position.x - tracked_person_.point.x, 2) +
            pow(a.bbox.center.position.z - tracked_person_.point.z, 2);
            double db = pow(b.bbox.center.position.x - tracked_person_.point.x, 2) +
            pow(b.bbox.center.position.z - tracked_person_.point.z, 2);
            return da < db;
        });
      tracked_person_.point.x = closest->bbox.center.position.x;
      tracked_person_.point.y = closest->bbox.center.position.y;
      tracked_person_.point.z = closest->bbox.center.position.z;
      detection = *closest;
      detection_3d_pub_->publish(detection);
    }

    tracked_person_.header = msg->header;
    // usa tracked_person_ para generate_tf

    std::string error;
    if(tf_buffer_.canTransform("base_link", "camera_depth_optical_frame", tf2::TimePointZero,
      &error))
    {
      auto tf = tf_buffer_.lookupTransform(
      "base_link", "camera_depth_optical_frame", tf2::TimePointZero);

      geometry_msgs::msg::PointStamped obstacle_point_base;
      tf2::doTransform(tracked_person_, obstacle_point_base, tf);

      generate_tf(obstacle_point_base);
      publish_tf();

      RCLCPP_INFO(get_logger(), "Person detected at x: %f, y: %f, z: %f (base_link)",
      obstacle_point_base.point.x,
      obstacle_point_base.point.y,
      obstacle_point_base.point.z);

    } else {
      RCLCPP_WARN_STREAM(get_logger(), "Error in TF camera_link -> base_link [<< " << error << "]");
    }

  } else {
    tracking_ = false;
  }
}

void
Detection3DNode::generate_tf(geometry_msgs::msg::PointStamped point)
{
  transform_.header.stamp = point.header.stamp;
  transform_.header.frame_id = "base_link";
  transform_.child_frame_id = "person_detected";

  transform_.transform.translation.x = point.point.x;
  transform_.transform.translation.y = point.point.y;
  transform_.transform.translation.z = point.point.z;
}

void
Detection3DNode::publish_tf()
{
  tf_broadcaster_->sendTransform(transform_);
}
