#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include<iostream>
#include<math.h>
#include<dirent.h>
using namespace std;
using namespace cv;


int main()
{
    //************************************************************************
       // get the names of all original file  such as template.jpg, a.jpg  
       DIR *dir=opendir("./original_img");
       dirent* p=NULL;
       vector<string> img_names;

       while((p=readdir(dir))!=NULL)
       {
           if(p->d_name[0]!='.')
           {
               string name=string(p->d_name);
               img_names.push_back(name);
           }
       }
       closedir(dir);

//************************************************************
    for(int img_name=0;img_name<img_names.size();img_name++)
    {
            Mat srcImage=imread("./original_img/"+img_names[img_name],0);
            resize(srcImage,srcImage,Size(srcImage.cols/2,srcImage.rows/2));
            
            Mat original=srcImage.clone();
           
            medianBlur(srcImage,srcImage,3);

            


            adaptiveThreshold(srcImage,srcImage,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV,61,15);
            
            Mat kernel=getStructuringElement(MORPH_RECT,Size(25,25));
            
            dilate(srcImage,srcImage,kernel);
          
            vector<Vec4i> hierarchy;
            vector<vector<cv::Point> > contours;

            findContours(srcImage,contours,hierarchy,RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);

            int index=0;
            double area=srcImage.rows*srcImage.cols/10;

            for(size_t i=0;i<contours.size();i++)
            {

                double t=contourArea(contours[i]);
                if(area<t)
                {
                    area=t;
                    index=i;
                  
                }
            }


            double epsilon=0.1*arcLength(contours[index],true);
            vector<vector<Point> > approx(contours.size());
            approxPolyDP(Mat(contours[index]),approx[0],epsilon,true);

            Point2f srcRectangle[4];
            Point2f dstRectangle[4];
            Point2f avg;

            avg.x=(approx[0][0].x+approx[0][1].x+approx[0][2].x+approx[0][3].x)/4;
            avg.y=(approx[0][0].y+approx[0][1].y+approx[0][2].y+approx[0][3].y)/4;

            for(int i=0; i<4; i++)
            {
                if(approx[0][i].x<avg.x)
                {
                    if(approx[0][i].y<avg.y)
                    {
                        srcRectangle[0]=Point2f(approx[0][i]);
                    }
                    else
                        srcRectangle[2]=Point2f(approx[0][i]);
                }

                if(approx[0][i].x>avg.x)
                {
                    if(approx[0][i].y<avg.y)
                    {
                        srcRectangle[1]=Point2f(approx[0][i]);
                    }

                    else
                        srcRectangle[3]=Point2f(approx[0][i]);
                }
            }

        int x_len=sqrt((srcRectangle[0].x-srcRectangle[1].x)*(srcRectangle[0].x-srcRectangle[1].x)+
        (srcRectangle[0].y-srcRectangle[1].y)*(srcRectangle[0].y-srcRectangle[1].y));

        int y_len=sqrt((srcRectangle[0].x-srcRectangle[2].x)*(srcRectangle[0].x-srcRectangle[2].x)+
        (srcRectangle[0].y-srcRectangle[2].y)*(srcRectangle[0].y-srcRectangle[2].y));

        dstRectangle[0]=Point2f(0,0);
        dstRectangle[1]=Point2f(x_len,0);
        dstRectangle[2]=Point2f(0,y_len);
        dstRectangle[3]=Point2f(x_len,y_len);

        Mat M=getPerspectiveTransform(srcRectangle,dstRectangle);
        warpPerspective(original,original,M,Size(x_len,y_len));

        string save_name;
        if(img_names[img_name].substr(0,8)=="template")
              save_name="./template.bmp";
        else
              save_name="./table_img/"+img_names[img_name];

        imwrite(save_name,original);
    }

   



    // waitKey(0); 
    


    return 0;
}
