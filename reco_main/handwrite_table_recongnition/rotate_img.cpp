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
      //  Get the table area from the diagram
    for(int i_=0;i_<img_names.size();i_++)
    {
        Mat srcImage=imread("./original_img/"+img_names[i_],0);
        Mat original=srcImage.clone();

        
        medianBlur(srcImage,srcImage,3);
        adaptiveThreshold(srcImage,srcImage,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV,61,15);

        Mat kernel=getStructuringElement(MORPH_RECT,Size(5,5));
        dilate(srcImage,srcImage,kernel);
        vector<Vec4i> hierarchy;
        vector<vector<cv::Point> > contours;

        findContours(srcImage,contours,hierarchy,RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE,Point(0,0));

        int index=0;
        double area=srcImage.rows*srcImage.cols/20;

        for(size_t i=0;i<contours.size();i++)
        {

            double t=contourArea(contours[i]);
            if(area<t)
            {
                area=t;
                index=i;
            }
        }

        RotatedRect box=minAreaRect(contours[index]);

        Point2f point4[4];
        box.points(point4);

        //    cout<<point4[3].x<<endl;
        
        Point2f srcTriangle[3];
        Point2f dstTriangle[3];

        for(int i=0;i<4;i++)
        {
            if(point4[i].x<box.center.x)
            {
                if(point4[i].y<box.center.y)
                {
                    srcTriangle[0]=point4[i];
                }
                else
                {
                    srcTriangle[2]=point4[i];
                }
            }

            if(point4[i].x>box.center.x)
            {
                if(point4[i].y<box.center.y)
                {
                    srcTriangle[1]=point4[i];
                }
            }
        }

        dstTriangle[0]=Point2f(0,0);
        dstTriangle[1]=Point2f(box.size.width,0);
        dstTriangle[2]=Point2f(0,box.size.height);

        Mat warpMat=getAffineTransform(srcTriangle,dstTriangle);

        warpAffine(original,original,warpMat,box.size);
        

        string save_name;
        if(img_names[i_].substr(0,8)=="template")
              save_name="./template.bmp";
            
        else
              save_name="./table_img/"+img_names[i_];
        // imshow("00",original);
        // waitKey(0);

        imwrite(save_name,original);
    }
   
  

  

  return 0;


}