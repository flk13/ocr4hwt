#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include<iostream>
#include<fstream>


using namespace std;
using namespace cv;




 

int main(int argc,char ** argv)
{
    
    
    
    ifstream in0("out.txt");
    if(!in0.is_open())
    {
        cout<<"cann't open out.txt";
        exit(0);
    }
    string str_;
    getline(in0,str_);
    in0.close();

    int nums_=atoi(str_.c_str());
    
    
   ofstream on0("out2.txt");


    for(int img_num=0;img_num<nums_;img_num++)
    {
           
            
            char ss_name[32]={0};
            sprintf(ss_name,"./template/%c.bmp",'b'+img_num);

            Mat src=imread(ss_name,0);
            
        //     adaptiveThreshold(src,src,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV,61,15);
    
        // //    Mat kernel_=getStructuringElement(MORPH_RECT,Size(3,3));
        // //     dilate(src, src, kernel_, Point(-1, -1));
        // // medianBlur(src,src,3);

        //      Mat kernel_=getStructuringElement(MORPH_RECT,Size(2,2));
        //     dilate(src, src, kernel_, Point(-1, -1));
          
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
                                if(t_tt>40)
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
                           {
                               cout<<"666";
                                if(index_[i+2]-index_[i+1]>=(index_[i+3]-index_[i+2])/2)
                                    {
                                    index_.erase(index_.begin()+i,index_.begin()+i+2);
                                    cout<<"555";
                                    }
                                else 
                                    index_.erase(index_.begin()+i+1,index_.begin()+i+3);
                           }

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
                sprintf(name,"./savedir_zh0/%c_%c.bmp",'a'+img_num,'a'+x);

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
            on0<<perZH.size()<<" ";

           
            
    }

    on0.close();
    

   
    

 
    return 0;
}
