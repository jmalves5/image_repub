#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <depth_image_proc/depth_traits.h>


sensor_msgs::ImageConstPtr depth_msg;
cv_bridge::CvImagePtr cv_depth_ptr;

image_transport::Subscriber sub;
image_transport::Publisher pub;

void imageCallback(const sensor_msgs::ImageConstPtr& depth_msg)
{
  try
  {
    
    if ("16UC1" == depth_msg->encoding){
      try{
        cv_depth_ptr = cv_bridge::toCvCopy(depth_msg, sensor_msgs::image_encodings::TYPE_16UC1);
      }catch (cv_bridge::Exception& e){
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
      }
    }else if ("32FC1" == depth_msg->encoding){
      try{
        cv_depth_ptr = cv_bridge::toCvCopy(depth_msg, sensor_msgs::image_encodings::TYPE_32FC1);
      }catch (cv_bridge::Exception& e){
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
      }

      cv::Mat convertedDepthImg(cv_depth_ptr->image.size(), CV_16UC1);

      const int V = cv_depth_ptr->image.size().height;
      const int U = cv_depth_ptr->image.size().width;

      #pragma omp parallel for
      for (int v = 0; v < V; ++v){
        for (int u = 0; u < U; ++u){
          convertedDepthImg.at<uint16_t>(v, u)= depth_image_proc::DepthTraits<uint16_t>::fromMeters(cv_depth_ptr->image.at<float>(v, u));
        }
      }

      cv_depth_ptr->encoding = "16UC1";
      cv_depth_ptr->image = convertedDepthImg;

      pub.publish(cv_depth_ptr->toImageMsg());

      printf("Published Image\n");
    }

  }catch (cv_bridge::Exception& e){
    ROS_ERROR("Could not convert", depth_msg->encoding.c_str());
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "repub");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  sub = it.subscribe("camera/depth_registered/image_raw", 1, imageCallback);
  pub = it.advertise("repub/image_16", 1);
  ros::spin();

}

