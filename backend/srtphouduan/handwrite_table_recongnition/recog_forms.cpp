#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include<iostream>
#include<fstream>
#include<dirent.h>
using namespace std;
using namespace cv;

int main()
{

    //************************************************************************
       // get the names of all original file  such as template.jpg, a.jpg  
       DIR *dir=opendir("./table_img");
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

     for(int i_i=0;i_i<img_names.size();i_i++)
     {
        Mat srcImg=imread("./table_img/"+img_names[i_i],0);
        Mat bw;

        //binaryzation
        adaptiveThreshold(srcImg,bw,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV,51,20);
        
        Mat src_bw=bw.clone();
        Mat tmp(src_bw.rows,src_bw.cols,CV_8UC1,Scalar::all(0));
        int sum_pixel=0;
        bool end_=false;
        Point2f cross_kernel[4]={Point2f(0,1),Point2f(1,0),Point2f(-1,0),Point2f(0,-1)};

        
        queue<Point2f> tmp_queue; 
        for(int i=0;i<bw.rows;i++)
        {
            for(int j=0;j<bw.cols;j++)
            {
                
                if(src_bw.at<uchar>(i,j)!=0)
                {
                
                    tmp_queue.push(Point2f(j,i));
                    while(!tmp_queue.empty())
                    {
                    
                        Point2f point=tmp_queue.front();
                        tmp_queue.pop();
                        if(tmp.at<uchar>(point.y,point.x)!=0)
                        continue;
                        tmp.at<uchar>(point.y,point.x)=255;
                        sum_pixel++;
                        for(int x=0;x<4;x++)
                        {
                            
                                int tmp_x=point.x+cross_kernel[x].x;
                                int tmp_y=point.y+cross_kernel[x].y;
                                if(tmp_x<0 || tmp_y<0 ||tmp_x>=bw.cols || tmp_y>=bw.rows)
                                {
                                    continue;
                                }

                                if(tmp.at<uchar>(tmp_y,tmp_x)==255)
                                {
                                    continue;
                                }

                                if(src_bw.at<uchar>(tmp_y,tmp_x)!=0)
                                {
                                
                                    tmp_queue.push(Point2f(tmp_x,tmp_y));
                                }
                            
                        }
                    }

                    

                    if(sum_pixel>20)
                    {
            
                
                
                        end_=true;
                        bitwise_not(tmp,tmp);
                        bitwise_and(src_bw,tmp,src_bw);
                        break;
                    }
    
                else
                {
                    
                        sum_pixel=0;
                        bitwise_not(tmp,tmp);
                        bitwise_and(src_bw,tmp,src_bw);
                        bitwise_xor(tmp,tmp,tmp);
                    
                }              
                }
                

                
            }
        
            if(end_==true)
            break;
            
        }



        Mat kernel_=getStructuringElement(MORPH_RECT,Size(2,2));
        dilate(bw, bw, kernel_, Point(-1, -1));

    //extract horizontal and vertical lines
        Mat horizontal=bw.clone();
        Mat vertical=bw.clone();

        int scale =15;
        int horizontalsize=horizontal.cols/scale;
        Mat horizontalStructure=getStructuringElement(MORPH_RECT,Size(horizontalsize,1));
        erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
        dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));
        // imshow("horizontal",horizontal);



        int verticalsize=vertical.rows/scale;
        Mat verticalStructure=getStructuringElement(MORPH_RECT,Size(1,verticalsize));
        erode(vertical,vertical,verticalStructure,Point(-1,-1));
        dilate(vertical,vertical,verticalStructure,Point(-1,-1));
        // imshow("vertical",vertical);

        Mat joints;
        bitwise_and(horizontal,vertical,joints);
        // imshow("joints",joints);

        Mat joints_(joints.rows+7,joints.cols+7,CV_8UC1,Scalar::all(0));
        for(int i=0;i<joints.rows;i++)
        {
            for(int j=0;j<joints.cols;j++)
            {
                if(joints.at<uchar>(i,j)!=0)
                joints_.at<uchar>(i,j)=255;
                joints.at<uchar>(i,j)=0;
            }
        }



        for(int i=0; i<joints.rows;i++)
        {
            for(int j=0;j<joints.cols;j++)
            {
                if(joints_.at<uchar>(i,j)!=0)
                {
                    for(int m=i;m<i+7;m++)
                    {
                        for(int n=j;n<j+7;n++)
                        {
                            joints_.at<uchar>(m,n)=0;
                        }
                    }

                    if(j>0)
                    {
                        joints_.at<uchar>(i+1,j-1)=0;
                        joints_.at<uchar>(i+2,j-1)=0;
                        joints_.at<uchar>(i+3,j-1)=0;
                        joints_.at<uchar>(i+4,j-1)=0;
                        joints_.at<uchar>(i+5,j-1)=0;
                        joints_.at<uchar>(i+6,j-1)=0;
                    }
                    if(j>1)
                    {
                        joints_.at<uchar>(i+1,j-2)=0;
                        joints_.at<uchar>(i+2,j-2)=0;
                        joints_.at<uchar>(i+3,j-2)=0;
                        joints_.at<uchar>(i+4,j-2)=0;
                        joints_.at<uchar>(i+5,j-2)=0;
                        joints_.at<uchar>(i+6,j-2)=0;
                    }
                    joints_.at<uchar>(i,j)=255;
                }

            }
        }

        for(int i=0;i<joints.rows;i++)
        {
            for(int j=0;j<joints.cols;j++)
            {
                if(joints_.at<uchar>(i,j)!=0)
                joints.at<uchar>(i,j)=255;
            }
        }

        // for(int i=0;i<joints.rows;i++)
        // {
        //     for(int j=0;j<joints.cols;j++)
        //     {
        //         if(joints.at<uchar>(i,j)!=0)
        //         cout<<i<<" "<<j<<endl;
        //     }
        // }


    // //****************************************************************************************
    //    //correct the location of the vertices
    //    //correct the location of the vertices
        vector<int> ver_index;

        vector<vector<Point2f> > vertexs;
        // vector<vector<Point2f> > v_vertexs;
        vector<vector<int> >   vertex_in;
        int sum_pixels;
        vector<int> hori_shadow;
        for(int i=0;i<joints.rows;i++)
        {

            sum_pixels=0;
            for(int j=0;j<joints.cols;j++)
            {
            if(joints.at<uchar>(i,j)!=0)
                sum_pixels++;
            }
            hori_shadow.push_back(sum_pixels);
        }

        vector<int> ver_shadom;

        for(int j=0;j<joints.cols;j++)
        {
            sum_pixels=0;
            for(int i=0;i<joints.rows;i++)
            {
                if(joints.at<uchar>(i,j)!=0)
                {
                    sum_pixels++;               
                }
            }
            ver_shadom.push_back(sum_pixels);
        }
        

        bool swith=false;
        int  sn=0;
        int sn_end=0;
        int cache=0;
        for(int i=0;i<joints.rows;i++)
        {
            if(hori_shadow[i]!=0)
            { 
                if(swith==false)
                {
                    swith=true;
                    sn=i;
                    cache=16;
                }
                sn_end=i;
            }
            if(swith==true&&cache>0)
            {

                if (hori_shadow[i]==0)
                cache--;
                // continue;
            }
            if(swith==true&&((cache==0&&hori_shadow[i]==0)||i==joints.rows-1))
            {
                vector<Point2f> vertex;
            for(int n=0;n<joints.cols;n++)
            {
                
                for(int m=sn;m<=sn_end;m++)
                {
                    if(joints.at<uchar>(m,n)!=0)
                    {
                        vertex.push_back(Point2f(n,m));
                        break;
                    }
                }
            }
                vertexs.push_back(vertex);
                swith=false;
            }
        }



    
    //**************************************************************************************************
    //check if the last operation is correct.
        // for(int i=0;i<hor_index.size();i++)
        // {
        //     cout<<hor_index[i]<<endl;
        // }

    // for(int i=0;i<vertexs.size();i++)
    // {
    //     for(int j=0;j<vertexs[i].size();j++)
    //     {
    //           cout<<vertexs[i][j]<<endl;
    //     }
    //     cout<<"***********"<<endl;
    // }




    // for(int i=0;i<joints.rows;i++)
    // {
    //     for(int j=0;j<joints.cols;j++)
    //     {
    //         if(joints.at<uchar>(i,j)!=0)
    //         {
    //             cout<<i<<" "<<j<<endl;
    //         }
    //     }
    // }


    //************************************************************************************************


        ifstream in("rects.txt");
        if(!in.is_open())
        {
            cout<<"cann't open the file"<<endl;
            exit(0);
        }
        
        string str;
        getline(in,str);
        int start;
        vector<vector<int> > vec_int;
        while(str!="end")
        {
            vector<int> vec;
            start=0;
            for(int i=0;i<str.size();i++)
            {
                if(str.at(i)==' ')
                {
                    string tmp=str.substr(start,i-start);
                    vec.push_back(atoi(tmp.c_str()));
                    start=i+1;
                }
            }
            vec_int.push_back(vec);

            getline(in,str);

        }

        for(int i=0;i<vec_int.size();i++)
        {
            for(int j=0;j<vec_int[i].size();j+=4)
            {
                
                Mat tmp=src_bw(Rect(Point2f(vertexs[vec_int[i][j+2]][vec_int[i][j]].x+4,
                            vertexs[vec_int[i][j+2]][vec_int[i][j]].y+4),
                            Point2f(vertexs[vec_int[i][j+3]][vec_int[i][j+1]].x-1,
                            vertexs[vec_int[i][j+3]][vec_int[i][j+1]].y-1)));
                char s_name[32]={0};
                sprintf(s_name,"./save_img/%c_%c_%c.bmp",'a'+i_i,'a'+i,'a'+j/4);
                imwrite(s_name,tmp);
                
            }


        }

           

        in.close();

     }

     ofstream nums_img_("img_nums.txt");
        nums_img_<<img_names.size();
        nums_img_.close();


    

    return 0;
}