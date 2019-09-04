#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
#include <iostream>
#include <depth_image_proc/depth_traits.h>
#include <image_repub/ByteMultiArray.h>



image_transport::Subscriber sub;
ros::Publisher pubcompressed;

using namespace cv;
using namespace std;

int *buffer, *pBuffer, word, nibblesWritten;

char* output;
short* dataMat;

cv_bridge::CvImagePtr cv_depth_ptr;

int V = 480;
int U = 640;


void EncodeVLE(int value)
{
  do{
    int nibble = value & 0x7; // lower 3 bits
    if (value >>= 3) nibble |= 0x8; // more to come
    word <<= 4;
    word |= nibble;     //Bitwise OR AND ASSIGNMENT (word = word | nibble)
  
    if (++nibblesWritten == 8){ // output word
      *pBuffer++ = word;
      nibblesWritten = 0;
      word = 0;
    }
  } while (value);
}


int DecodeVLE(){
  unsigned int nibble;
  int value = 0, bits = 29;

  do{
    if (!nibblesWritten){
      word = *pBuffer++; // load word
      nibblesWritten = 8;
    }

    nibble = word & 0xf0000000;
    value |= (nibble << 1) >> bits;
    word <<= 4;
    nibblesWritten--;
    bits -= 3;
  } while (nibble & 0x80000000);

return value;

}



int CompressRVL(short* input, char* output, int numPixels){

  buffer = pBuffer = (int*)output;
  nibblesWritten = 0;
  short *end = input + numPixels;
  short previous = 0;


  while (input != end){
    int zeros = 0, nonzeros = 0;
    for (; (input != end) && !*input; input++, zeros++);
    EncodeVLE(zeros); // number of zeros
    for (short* p = input; (p != end) && *p++; nonzeros++);
    EncodeVLE(nonzeros); // number of nonzeros
    for (int i = 0; i < nonzeros; i++){
      short current = *input++; 
      int delta = current - previous;
      int positive = (delta << 1) ^ (delta >> 31);
      EncodeVLE(positive); // nonzero value
      previous = current;
    }
  }


  if (nibblesWritten) // last few values
  *pBuffer++ = word << 4 * (8 - nibblesWritten);
  
  return int((char*)pBuffer - (char*)buffer); // num bytes
}


void imageCallback(const sensor_msgs::ImageConstPtr& depth_msg)
{
 
  cv_depth_ptr = cv_bridge::toCvCopy(depth_msg, sensor_msgs::image_encodings::TYPE_32FC1);

  /*COMPRESS*/

  for (int i = 0; i < V; i++){
    for (int k = 0; k < U; k++){
       dataMat[i*U+k] = depth_image_proc::DepthTraits<uint16_t>::fromMeters(cv_depth_ptr->image.at<float>(i, k));
    }
  }


  int n = CompressRVL(dataMat, output, V*U);
  
  
  //cv::Mat compressedData(480, 640, CV_16S, output);
 
  image_repub::ByteMultiArray array;
  array.data.clear();

  for (int i = 0; i < n; i++)
  {
    array.data.push_back(output[i]);
  }

  pubcompressed.publish(array);

  ROS_INFO("published");
  
}

int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "repub_compress");
  ros::NodeHandle nh;
  output=(char*)malloc(V*U*sizeof(short));
  dataMat = (short*)malloc(V*U*sizeof(short));
  image_transport::ImageTransport it(nh);
  sub = it.subscribe("camera/depth_registered/image_raw", 1, imageCallback);
  pubcompressed = nh.advertise<image_repub::ByteMultiArray>("arrayCompressed", 10);
  ros::spin();
  free(output);
  free(dataMat);
  return 0;
}



