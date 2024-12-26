#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv2/aruco.hpp>

using namespace std;
using namespace cv;


int main() {
   Mat streamImage, detectingImage;//Declaring a matrix to load the frames//
   namedWindow("Video Player");//Declaring the video to show the video//
   VideoCapture cap(0);//Declaring an object to capture stream of frames from default camera//
   cv::Ptr<cv::aruco::Dictionary> dictionary =   cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
   //cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
   if (!cap.isOpened()){ //This section prompt an error message if no video stream is found//
      cout << "No video stream detected" << endl;
      system("pause");
      return-1;
   }

   while (true){ //Taking an everlasting loop to show the video//
      cap >> streamImage;
      streamImage.copyTo(detectingImage);
      if (streamImage.empty()){ //Breaking the loop if no video frame is detected//
         break;
      }
      std::vector<int> ids;
      std::vector<std::vector<cv::Point2f> > corners;
      cv::aruco::detectMarkers(detectingImage, dictionary, corners, ids);
      // if at least one marker detected
      if (ids.size() > 0){
            cv::aruco::drawDetectedMarkers(detectingImage, corners, ids);
            cout << "There are " << ids.size() << " markers detected." << endl;
      }
      
      imshow("Video Player", detectingImage);//Showing the video//
      char c = (char)waitKey(25);//Allowing 25 milliseconds frame processing time and initiating break condition//
      if (c == 27){ //If 'Esc' is entered break the loop//
         break;
      }
   }
   cap.release();//Releasing the buffer memory//
   return 0;
}