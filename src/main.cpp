#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

uint8_t s_add(uint8_t val1, uint8_t val2);

int main(){
    VideoCapture cap("input.mp4");
    if (!cap.isOpened()){
        cerr << "Cant Open Video!" << endl; 
        return -1;
    }
    
    int err;

    Mat frame, gray_frame, resized_frame;

    while (cap.read(frame)){
        cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
        resize(gray_frame, resized_frame, Size(1280,720));

        for (int y = 0; y < resized_frame.rows; y++){
            for(int x = 0; x < resized_frame.cols; x++){
                uint8_t old_pixel = resized_frame.at<uint8_t>(y,x);
                uint8_t new_pixel = (old_pixel > 127) ? 255 : 0;
                int16_t err = old_pixel - new_pixel;
                
                resized_frame.at<uint8_t>(y,x) = new_pixel;
                
                if ( x + 1 < resized_frame.cols){
                    resized_frame.at<uint8_t>(y,x + 1) = s_add(resized_frame.at<uint8_t>(y, x + 1) , err * 7 / 16);
                }
                if ( y + 1 < resized_frame.rows){
                    if (x > 0){
                        resized_frame.at<uint8_t>(y + 1, x - 1) = s_add(resized_frame.at<uint8_t>(y + 1, x - 1) , err * 3 / 16);                        
                    }
                    resized_frame.at<uint8_t>(y + 1, x) = s_add(resized_frame.at<uint8_t>(y + 1, x) , err * 5 / 16);
                    if (x + 1 < resized_frame.cols){
                        resized_frame.at<uint8_t>(y + 1, x + 1) = s_add(resized_frame.at<uint8_t>(y + 1, x + 1) , err * 1 / 16);
                    }
                }
            }
        }
        imshow("testwindow", resized_frame);
        if (waitKey(1) >= 0) break;
    }
    return 0;
}

uint8_t s_add(uint8_t val1, uint8_t val2){
    uint16_t val1_int = val1;
    uint16_t val2_int = val2;
    
    uint16_t temp = val1_int + val2_int;
    return saturate_cast<uint8_t>(temp);
}
