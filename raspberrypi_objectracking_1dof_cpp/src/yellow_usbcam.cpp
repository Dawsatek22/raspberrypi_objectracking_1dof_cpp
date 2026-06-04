// this is a code to detect  contours colors  with a camera and opencv
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;



int center; // is the value of the center x value
int x_medium; //  x range value  thats gets measured

int main() {
    
    VideoCapture  cam(0); // to capture the video
    Mat frame ; // object we are gonna read

 

 
   while (true) {

cam.read(frame); // reads frame
// checks if camera is opened
if(!cam.isOpened()){
    std::cout << "camera is not opened"<< '\n';
break;
}
    // yellow wraps around hue=0, so use two ranges.
    Scalar lower_color1(22, 38, 160);
    Scalar upper_color1(33, 244, 255);
    Scalar lower_color2(23, 39, 170);
    Scalar upper_color2(34, 244, 255);


Mat mask1 ,mask2, mask, hsv;
cvtColor(frame , hsv, cv::COLOR_BGR2HSV);
inRange(hsv,lower_color1,upper_color1,mask1);
inRange(hsv,lower_color1,upper_color2,mask2);
mask = mask1 | mask2;

    // Clean noise before contour extraction.
    
Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(5,5));

    erode(mask, mask, kernel);
    dilate(mask, mask, kernel);

    std::vector<std::vector<Point>> contours;
    findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);



  
// checks countour area
    for (std::size_t i = 0; i < contours.size(); ++i) {
        double const area = cv::contourArea(contours[i]);
        if (area <= 500) {
            continue;
        }
           // creates object for detecting color
        Rect const box = cv::boundingRect(contours[i]);
       
       
       x_medium = int(box.x + box.width/ 2   ); // is the x direction converted into a int
     
        //  puts a rectangle on countour
        rectangle(frame, box,Scalar(255, 0, 0), 2);
        // put the color name on the countour
        putText(
            frame,
            "yellow",
            box.tl(),
            FONT_HERSHEY_SIMPLEX,
            1.0,
            Scalar(255, 230, 70),2
          
        );
       

 int error = x_medium/6; // supossed to be the offset
//position = error;

std::cout << "position of center" << center <<'\n';
std::cout << "position of error" << error <<'\n';
std::cout << "position of x_medium" << x_medium <<'\n';

        
    }

imshow("hsv",hsv); // optional
imshow("mask",mask); // optional
 imshow("show_coutours",frame); // now shows frame
if (waitKey(1) == ('q')) {
  break;
  destroyAllWindows();
}

}
}