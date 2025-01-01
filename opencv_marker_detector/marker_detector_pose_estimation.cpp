#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv2/aruco.hpp>


using namespace std;
using namespace cv;

static bool readCameraParameters(string filename, Mat &camMatrix, Mat &distCoeffs) {
    FileStorage fs(filename, FileStorage::READ);
    if(!fs.isOpened()){
        cout << "Failed to read camera parameters" << endl;
        return false;
    }
    cout << "found camera parameters" << endl;    
    fs["camera_matrix"] >> camMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
    return true;
}



int main() {
   Mat streamImage, detectingImage;//Declaring a matrix to load the frames//
   namedWindow("Video Player");//Declaring the video to show the video//
   VideoCapture cap(0);//Declaring an object to capture stream of frames from default camera//
   cv::Ptr<cv::aruco::Dictionary> dictionary =   cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);   
   cv::Mat distCoeffs = (Mat1d(1,5) << 0.189572, -0.795616, 0.001088, -0.006897, 0.000000);
   cv::Mat cameraMatrix = (Mat1d(3, 3) << 891.94889, 0, 308.19811, 0, 892.67595, 258.19079, 0, 0, 1);
   string filename = "../camera_calibration_ros2/ost.txt";
   //readCameraParameters(filename, cameraMatrix, distCoeffs);
   // Set coordinate system
    cv::Mat objPoints(4, 1, CV_32FC3);
    float markerLength = 0.02;
    objPoints.ptr<cv::Vec3f>(0)[0] = cv::Vec3f(-markerLength/2.f, markerLength/2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[1] = cv::Vec3f(markerLength/2.f, markerLength/2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[2] = cv::Vec3f(markerLength/2.f, -markerLength/2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[3] = cv::Vec3f(-markerLength/2.f, -markerLength/2.f, 0);

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

            int nMarkers = corners.size();
            std::vector<cv::Vec3d> rvecs(nMarkers), tvecs(nMarkers);//(nMarkers) specified how many vectors are initialized
            // Calculate pose for each marker
            for (int i = 0; i < nMarkers; i++) {
                solvePnP(objPoints, corners.at(i), cameraMatrix, distCoeffs, rvecs.at(i), tvecs.at(i));
            }
            //Draw axis for each marker
            for(unsigned int i = 0; i < ids.size(); i++) {
                cv::drawFrameAxes(detectingImage, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.1);
                if(ids.at(i) == 40 || ids.at(i) == 98){
                  cout << "id " << ids.at(i) <<" rvects " << endl << " "  <<rvecs[i] << endl;
                  cout << "id " << ids.at(i) <<" tvects " << endl << " "  <<tvecs[i] << endl;
                }
            }
            
            

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