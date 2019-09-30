#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <depth_image_proc/depth_traits.h>
#include <image_repub/ByteMultiArray.h>



cv_bridge::CvImagePtr cv_depth_ptr;

ros::Subscriber subcompressed;
image_transport::Publisher pub;

using namespace cv;
using namespace std;

int *buffer, *pBuffer, word, nibblesWritten;

short* output;
char* dataMat;

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


void DecompressRVL(char* input, short* output, int numPixels){
  buffer = pBuffer = (int*)input;
  nibblesWritten = 0;
  short current, previous = 0;
  int numPixelsToDecode = numPixels;

  while (numPixelsToDecode){
    
    int zeros = DecodeVLE(); // number of zeros
    numPixelsToDecode -= zeros;
    
    for (; zeros; zeros--)
      *output++ = 0;
    
    int nonzeros = DecodeVLE(); // number of nonzeros
    numPixelsToDecode -= nonzeros;
  
    for (; nonzeros; nonzeros--){
      int positive = DecodeVLE(); // nonzero value
      int delta = (positive >> 1) ^ -(positive & 1);
      current = previous + delta;
      *output++ = current;
      previous = current;
    }
  }
}



void msgCallback(const image_repub::ByteMultiArray::ConstPtr& array){
  int i=0;
  
  for (std::vector<signed char>::const_iterator it = array->data.begin(); it !=array->data.end(); ++it){
    dataMat[i] = *it;
    i++;
  }

  DecompressRVL((char*)dataMat, (short*) output, V*U);
  	
  cv::Mat image(V, U, CV_32FC1);

  for (int i = 0; i < V; ++i){
      for (int k = 0; k < U; ++k){
        image.at<float>(i, k) = depth_image_proc::DepthTraits<float>::toMeters(output[i*U+k]);
        printf("%d\n", image.at<float>(i, k));
      }
  }

  cv_bridge::CvImagePtr cv_depth_ptr(new cv_bridge::CvImage);

  ros::Time time = ros::Time::now();
	
	cv_depth_ptr->encoding = "32FC1";
  cv_depth_ptr->header.stamp = time;
  cv_depth_ptr->header.frame_id = "/compressedData";

  cv_depth_ptr->image = image;

  pub.publish(cv_depth_ptr->toImageMsg());

  ROS_INFO("published image_raw");
  
}

int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "repub_decompress");
  ros::NodeHandle nh;
  output=(short*)malloc(V*U*sizeof(short));
  dataMat = (char*)malloc(V*U*sizeof(short));
  image_transport::ImageTransport it(nh);
  subcompressed = nh.subscribe("/arrayCompressed", 1, msgCallback);
  pub = it.advertise("/compressedData", 10);
  ros::spin();
  free(output);
  free(dataMat);
  return 0;
}

