#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include<iostream>
#include<fstream>


using namespace std;
using namespace cv;

#define STD_WIDTH 20
#define STD_HEIGHT 20


 bool comp(const Rect &a,const Rect &b)
        {
            return a.x<b.x;
        }

Mat getStandardDigit(Mat img)
{
    int w=img.cols;
    int h=img.rows;

    int new_width=int(w*STD_HEIGHT/h);
    
    if(new_width>STD_WIDTH)
        new_width=STD_WIDTH;
    
    
    resize(img,img,Size(new_width,STD_HEIGHT));

    Mat canvas=Mat::zeros(Size(STD_WIDTH+8,STD_HEIGHT+8),CV_8UC1);

    int x=(STD_WIDTH-new_width)/2;
    int y=4;

    for (int i=y;i<STD_HEIGHT+4  ;i++)
    {
        for(int j=x+4;j<x+new_width+4;j++)
        {
            canvas.at<uchar>(i,j)=img.at<uchar>(i-y,j-x-4);
        }
    }
    // canvas.at<uchar>(STD_HEIGHT-1,new_width+x-1)=img2.at<uchar>(STD_HEIGHT-1,new_width-1);

    // Mat ROI=canvas(Rect(x,0,new_width,STD_HEIGHT));

    // canvas.copyTo(ROI,img);

    Mat element=getStructuringElement(MORPH_RECT,Size(2,2));
    dilate(canvas,canvas,element);


   return canvas;

    
}


int main(int argc,char ** argv)
{
    
    ifstream image_nums("img_nums.txt");
    if(!image_nums.is_open())
    {
        cout<<"cann't open the file: img_nums" ;
        exit(0);
    }

    string str;
    getline(image_nums,str);
    image_nums.close();
    int img_nums=atoi(str.c_str());


    ifstream in("nums.txt");
    if(!in.is_open())
    {
        cout<<"cann't open the file"<<endl;
        exit(0);
    }
   
    
    getline(in,str);
    string str_2=str.substr(0,str.size()-2);
    vector<int> vec_int;
    vector<bool> vec_bool;
    
    // cout<<str.substr(str.size()-1,1).c_str();
    while(str!="end")
    {
        vec_int.push_back(atoi(str_2.c_str()));
        if(str.substr(str.size()-1,1)=="0")
              vec_bool.push_back(false);
            
        else 
             vec_bool.push_back(true);
        getline(in,str);
    }
    in.close();

    // for(int i=0;i<vec_int.size();i++)
    // {
    //     cout<<vec_int[i]<<endl;
    // }

    // for(int i=0;i<vec_bool.size();i++)
    // {
    //     cout<<vec_bool[i]<<endl;
    // }


    int num_i;
    
    



    for(int img_num=0;img_num<img_nums;img_num++)
    {
        char s[32]={0};
        sprintf(s,"digit_num_%c.txt",img_num+'a');
        ofstream  digit_num(s);

        sprintf(s,"zh_nu_%c.txt",img_num+'a');
        ofstream  zh_num(s);
        sprintf(s,"num_real_%c.txt",img_num+'a');
        ofstream  num_real(s);
        for(int i_number=0;i_number<vec_int.size();i_number++)
        {
            num_i=0;
            for(int j_number=0;j_number<vec_int[i_number];j_number++)
            {
            
            char ss_name[32]={0};
            sprintf(ss_name,"./save_img/%c_%c_%c.bmp",'a'+img_num,'a'+i_number,'a'+j_number);

            Mat src=imread(ss_name,0);

            
            // if(vec_bool[i_number]==false)
            // {
            //      int rito=110/src.rows;
            //      if(rito>1)
            //      {
            //          resize(src,src,Size(src.cols*rito,src.rows*rito));

            //      }
            // }
        

            // Mat element=getStructuringElement
            
        

            // adaptiveThreshold(src,src,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY_INV,
                // 51,20.);
            // Mat element = getStructuringElement(MORPH_RECT, Size(2,2));
            // erode(src, src, element);


            int *pVerArr;
            pVerArr=new int[src.rows];
            memset(pVerArr,0,src.rows*4);
                

            for(int row=0;row<src.rows;row++)
                {
                    for (int col=0;col<src.cols;col++)
                    {
                        int tmpPixelValue=src.at<uchar>(row,col);
                        if (tmpPixelValue!=0)
                        {
                            pVerArr[row]++;
                        }
                    }
                }
            
            int ss_sum=0;
            for(int i=0;i<src.rows;i++)
            {
                ss_sum+=pVerArr[i];
            }

            if(ss_sum<28)
            {
            delete [] pVerArr;
            continue;

            }
            num_i++;

            if(vec_bool[i_number]==true)
            {
        vector<Vec4i> hierarchy;
        std::vector<std::vector<cv::Point> > contours;
        
        findContours(src,contours,hierarchy,RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
        
        int minHeight=15;

        int sum_=0;
        vector<Rect> vec_rect;
        for(size_t i =0; i< contours.size();i++)
        {
            Rect rect=boundingRect(contours[i]);
            
            if(rect.height< minHeight)
                continue;
            vec_rect.push_back(rect);

        }
        sort(vec_rect.begin(),vec_rect.end(),comp);

        for(int b=0;b<vec_rect.size();b++)
        {
            char s_[32]={0};
            sprintf(s_,"savedir_digit/%c_%c_%c_%c.jpg",'a'+img_num,i_number+'a',j_number+'a','a'+b);
            // Mat element = getStructuringElement(MORPH_RECT, Size(2,2));
            // erode(src(vec_rect[b]), src(vec_rect[b]), element);
            Mat digit=src(vec_rect[b]);
            digit=getStandardDigit(digit);
            imwrite(s_,digit);

        }
        digit_num<<vec_rect.size()<<" ";
            }

            else {

                medianBlur(src,src,3);

                int startIndex=0;
                int endIndex=0;
                bool inBlock=false;
                vector<Mat> rowImage;
                vector<Mat> perZH;
                int t_huan=-100;
        
                for (int i=0;i<src.rows;i++)
                {
                    if(!inBlock&&pVerArr[i]!=0)
                    {
                        inBlock=true;
                        startIndex=i;
                    }

                    else if(pVerArr[i]==0&&inBlock)
                    {
                            if(startIndex-t_huan<40)
                            startIndex=t_huan;
                            endIndex=i;
                            inBlock=false;
                            if ((endIndex-startIndex)>15)
                            {
                            Mat roiImg=src(Range(startIndex,endIndex+1),Range(0,src.cols));
                
                            rowImage.push_back(roiImg);
                            }
                            else
                            {
                                int t_tt=0;
                                for(int k_k=startIndex;k_k<endIndex;k_k++)
                                {
                                    t_tt+=pVerArr[k_k];
                                }
                                if(t_tt>20)
                                {
                                    t_huan=startIndex;
                                }
                            }  
                    }

                    else if(i==src.rows-1&&inBlock)
                    {
                        Mat roiImg=src(Range(startIndex,src.rows),Range(0,src.cols));
                
                            rowImage.push_back(roiImg);
                    }

                } 
                delete [] pVerArr;

             

            // for(int i=0;i<rowImage.size();i++)
            // {
            //       imshow("111",rowImage[i]);
            //       waitKey(0);
            // }

        //***************************************************************************************

                int *pHorArr;
                pHorArr=new int[src.cols];
                

                for (int x=0;x<rowImage.size();x++)
                {
                    

                    memset(pHorArr,0,src.cols*4);
                    Mat rowImg=rowImage.at(x);
                
                    int startIndex=0;
                    int endIndex=0;

                    for (int i=0;i<rowImg.cols;++i)
                    {
                        for (int j=0;j<rowImg.rows;++j)
                        {
                            int tmpPixelValue=rowImg.at<uchar>(j,i);
                            if(tmpPixelValue==255)
                            {
                                pHorArr[i]++;
                            }
                        }
                    }

                    bool inBlock=false;
                    vector<int> index_;

                    for (int col=0;col<rowImg.cols;col++)
                    {
                
                    if(!inBlock&&pHorArr[col]!=0)
                    {
                        inBlock=true;
                        startIndex=col;
                    }

                    else if(pHorArr[col]==0&&inBlock)
                    {
                        endIndex=col;
                        inBlock=false;

                        index_.push_back(startIndex);
                        index_.push_back(endIndex);
                        
                        
                        
                    }
                
                    }
            
                    for(int i=0 ;i<index_.size();i+=2)
                    {
                        if(i==0&&index_[i+1]-index_[i]<18)
                            index_.erase(index_.begin()+i+1,index_.begin()+i+3);

                        else if(i==index_.size()-2&&index_[i+1]-index_[i]<18)
                            index_.erase(index_.begin()+i-1,index_.begin()+i+1);

                        else if(index_[i+1]-index_[i]<18)
                        {
                            if(index_[i]-index_[i-1]>index_[i+2]-index_[i+1])
                            {
                            index_.erase(index_.begin()+i+1,index_.begin()+i+3);
                            }
                        else{
                            index_.erase(index_.begin()+i-1,index_.begin()+i+1);
                            }
                        }
                    }


                    for (int i=0;i<index_.size();i+=2)
                    {

                        Mat img=rowImg(Range(0,rowImg.rows),Range(index_[i],index_[i+1]));
                
                        int start=-1;
                        int end=-1;
                        for (int j=0;j<img.rows;j++)
                        {
                    
                            for (int k=0;k<img.cols;k++)
                            {
                            if(img.at<uchar>(j,k)!=0)
                            {
                                start=j;
                                break;
                            }
                        
                            }
                            if(start!=-1)
                            break;
                        }

                for (int j=img.rows-1;j>=0;j--)
                {
                    
                    for (int k=0;k<img.cols;k++)
                    {
                        if(img.at<uchar>(j,k)!=0)
                        {
                            end=j;
                            break;
                        }
                        
                    }
                    if(end!=-1)
                        break;
                }
                
                Mat img2=img(Range(max(start,0),min(end+1,img.rows)),Range(0,img.cols));

                perZH.push_back(img2);


                }
            }

            delete [] pHorArr;
            // for(int t=0;t<perZH.size();t++)
            // {
            //     imshow("555",perZH[t]);
            //     waitKey(0);
            // }
            
        //**********************************************************************************
        
            for (int x=0;x<perZH.size();x++)
            {
                Mat imgZh=perZH.at(x);
                char name[32]={0};
                // sprintf(name,"./savedir_zh/%c_%c.png",'a'+p,'a'+x);
                sprintf(name,"./savedir_zh/%c_%c_%c_%c.png",'a'+img_num,'a'+i_number,'a'+j_number,'a'+x);

                int width=imgZh.cols;
                int height=imgZh.rows;
                Mat canvas=Mat::zeros(Size(96,96),CV_8UC1);

                if(height>=width)
                {
                int new_width=int(width*90/height);
                if(new_width>90)
                new_width=90;
                
                resize(imgZh,imgZh,Size(new_width,90));
                
            

                int xa=(90-new_width)/2;

                for(int i=3;i<93;i++)
                {
                    for(int j=xa+3;j<xa+new_width+3;j++)
                    {
                        canvas.at<uchar>(i,j)=imgZh.at<uchar>(i-3,j-xa-3);
                    }
                }
                }
                

                else
                {

                int new_height=int(height*90/width);
                if(new_height>90)
                new_height=90;
                
                resize(imgZh,imgZh,Size(90,new_height));
                
                

                int xa=(96-new_height)/2;

                for(int i=xa+3;i<xa+new_height+3;i++)
                {
                    for(int j=3;j<93;j++)
                    {
                        canvas.at<uchar>(i,j)=imgZh.at<uchar>(i-xa-3,j-3);
                    }
                }



                }


                // sprintf(name,"../cause/savedir/%c.png",'a'+x);
                // Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
                // dilate(imgZh, imgZh, element);
                imwrite(name,canvas);
            }

            zh_num<<perZH.size()<<" ";
                }
            }
            if(vec_bool[i_number]==false)
            num_real<<num_i<<" 0 ";
            else
            num_real<<num_i<<" 1 ";
        }
         digit_num.close();
         zh_num.close();
         num_real.close();
    }


    

   
    

 
    return 0;
}