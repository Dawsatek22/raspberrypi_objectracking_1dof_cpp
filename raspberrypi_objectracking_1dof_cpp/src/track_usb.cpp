
// this is a code to track a color object with a usb camera and  a MG996R servo in the x direction angle 
#include <opencv2/opencv.hpp> // for computer vision 
#include <iostream> // for input and output strem
#include <string>
#include <PiPCA9685/PCA9685.h> // pca9685 header

#define SERVOMIN 300// This is the minimum pulse length count (out of 4096)
#define SERVOMAX 575// This is the maximum? pulse length count (out of 4096)
// the map function is created below to map the SERVOMIN and SERVOMAX values
long mapservo(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



int pulsval; // pulse value
int servoval; // map value for thr servos
int position; //  value for the servo
  int center= 130; // is the value of the center x value
int x_medium; //  x range value  thats gets measured

// namespaces to shorten the code
using namespace cv;


int main() {
PiPCA9685::PCA9685 track{"/dev/i2c-1",0x40}; // creates servo object.
// if PCA9685 default adress = 0x40 you can also do: PiPCA9685::PCA9685 track{}; instead. 
track.set_pwm_freq(60.0);
 servoval = mapservo(pulsval,0,180,SERVOMIN,SERVOMAX);

uint32_t width = 480; // the width of the frame
uint32_t height = 640; // the height of the frame 
VideoCapture  cam(0); // to capture the video
Mat frame ; // object we are gonna read

track.set_pwm(0,90,servoval); //  servos is calibrated
std::cout << "servo is set to 90 degrees angle"<< '\n';
sleep(2);
 
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
std::vector<std::vector<cv::Point>> contours;
findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE); 
// checks countour area
    for (size_t i = 0; i < contours.size(); ++i) {
        double const area = contourArea(contours[i]);
        if (area <= 300) {
            continue;
        }
           // creates object for detecting color
        Rect const box = boundingRect(contours[i]);
       
       
       x_medium = int(box.x + box.width/ 2   ); // is the x direction converted into a int
     
        //  puts a rectangle on countour
        rectangle(frame, box,Scalar(60, 255, 255), 2);
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

if (error > center) {


    position += 4;
}



if (error < center) {


    position -= 4;
}

// position limits are set below
if (position < 1) {

position = 0;
std::cout << "position of servos is reached 0" << '\n';

}



if (position > 180 ) {

position = 180;
std::cout << "position of servos is reached 180" << '\n';

}


else {

    std::cout << "position of servos is = 0" <<position  << '\n';

}
track.set_pwm(0,position,servoval); // moves servos acording to the position value

    }

 imshow("hsv",hsv); // optional
imshow("mask",mask); // optional
 imshow("show_coutours",frame); // now shows frame

if (waitKey(1) == ('q')) { // breaks loop when pressed q
  break;
  destroyAllWindows();
}

}
}