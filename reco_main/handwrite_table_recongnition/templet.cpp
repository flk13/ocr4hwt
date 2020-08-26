#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include<iostream>
#include<fstream>

using namespace std;
using namespace cv;

int main()
{
    
    Mat srcImg=imread("./template.bmp",0);  
    
    char name_a[32];
    if(srcImg.rows==1730)
    sprintf(name_a,"zh_dig%c.txt",'b');
    else if(srcImg.rows==1362)
    sprintf(name_a,"zh_dig%c.txt",'c');
    else
    sprintf(name_a,"zh_dig%c.txt",'a');
    ifstream on_(name_a);
    if(!on_.is_open())
    {
        cout<<"can't open zh_dig.txt";
        exit(0);
    }
    string zh_dig;
    getline(on_,zh_dig);
    on_.close();


  
    Mat bw;
    //binaryzation
    adaptiveThreshold(srcImg,bw,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV,61,18);
    Mat src_bw=bw.clone();
    Mat tmp(src_bw.rows,src_bw.cols,CV_8UC1,Scalar::all(0));

    int sum_pixel=0;
    bool end_=false;
    Point2f cross_kernel[4]={Point2f(0,1),Point2f(1,0),Point2f(-1,0),Point2f(0,-1)};

    // cout<<cross_kernel[0].x<<cross_kernel[0].y;
    
    queue<Point2f> tmp_queue; 
    for(int i=0;i<bw.rows;i++)
    {
        for(int j=0;j<bw.cols;j++)
        {
            
            if(src_bw.at<uchar>(i,j)!=0)
            {
                // cout<<i<<" "<<j<<endl;
                // cout<<"--------";
                tmp_queue.push(Point2f(j,i));
                while(!tmp_queue.empty())
                {
                    // cout<<"*";
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
                                // cout<<x<<" ";
                                 tmp_queue.push(Point2f(tmp_x,tmp_y));
                            }
                        
                    }
                }

                

                if(sum_pixel>20)
                {
                // cout<<sum_pixel<<endl;
               
               
                    end_=true;
                    bitwise_not(tmp,tmp);
                    bitwise_and(src_bw,tmp,src_bw);
                    break;
                }
 
               else
               {
                    // imshow("tmp",tmp);
                    // waitKey(0);
                    // cout<<"*";
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

    
    
    // condense the points in a small area  to single point

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

//****************************************************************************************
   //correct the location of the vertices
    vector<int> ver_index;
    vector<int> hor_index;
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


    swith=false;
    sn=0;
    cache=0;
    for(int i=0;i<joints.cols;i++)
    {
        if(ver_shadom[i]!=0)
        {
            if(swith==false)
            {
                swith=true;
                sn=i;
                cache=10;
            }
            sn_end=i;
        }

        if(swith==true&&cache>0)
        {
           if (ver_shadom[i]==0)
            cache--;
            // continue;
        }

        if(swith==true&&((cache==0&&ver_shadom[i]==0)||i==joints.cols-1))
        {

            // vector<Point2f> vertex;
            ver_index.push_back(sn);
            // for(int n=0;n<joints.rows;n++)
            // {
               
            //    for(int m=sn;m<=sn_end;m++)
            //    {
            //       if(joints.at<uchar>(n,m)!=0)
            //       {
            //           vertex.push_back(Point2f(m,n));
            //           break;
            //       }
            //    }
            // }
            // v_vertexs.push_back(vertex);     
            swith=false;
        }
    }
//****************************************
        //  for(int i=0;i<ver_index.size();i++)
        //  {
        //      cout<<ver_index[i]<<" ";
        //  }

        //  cout<<"\n";

//*************************************
    for(int i=0;i<vertexs.size();i++)
    {
        vector<int> vec;
        for(int j=0;j<vertexs[i].size();j++)
        {
            for(int k=0;k<ver_index.size();k++)
            {
                if(vertexs[i][j].x-ver_index[k]>-1&&vertexs[i][j].x-ver_index[k]<10)
                {
                    vec.push_back(k);
                    break;
                }
            }

            
        }
        vertex_in.push_back(vec);
    }




//**********************************************************************************************
    // check if the correctd piexl is correct.
    //  for(int i=0;i<joints.rows;i++)
    //  {
    //      for(int j=0;j<joints.cols;j++)
    //      {
    //          if(joints.at<uchar>(i,j)!=0)
    //          {
    //              cout<<i<<" "<<j<<endl;
    //          }
    //      }
    //  }
    // for(int i_=0;i_<vertex_in.size();i_++)
    // {
    //     for(int j_=0;j_<vertex_in[i_].size();j_++)
    //     {
    //         cout<<vertex_in[i_][j_]<<" ";
    //     }
    //     cout<<endl;
    // }



//***********************************************************************************************************     
    //  imshow("joints",joints);
    //  waitKey(0);
    //  Get the location and number of rectangles.
    int end_row=0;
    int end_col=0;

    int end_f;
    int end_n;
    bool b_is;
    
    vector<Rect> rects;
    vector<int>  indexs;
    vector<int> rect_index;
    int sum=0;
    for(int i=0;i<vertexs.size()-1;i++)
    {
        sum=0;
        for(int j=0;j<vertexs[i].size()-1;j++)
        {
            int fir=vertex_in[i][j];

            for(int z=j+1;z<vertexs[i].size();z++)
            {
                int nex=vertex_in[i][z];
           
            
            b_is=false;
            for(int t=i+1;t<vertex_in.size();t++)
            {
                end_f=-1;
                end_n=-1;
                for(int k=0;k<vertex_in[t].size();k++)
                {
                    if(fir==vertex_in[t][k])
                        end_f=k;
                    if(nex==vertex_in[t][k])
                       end_n=k;

                    if(end_f!=-1&&end_n!=-1)
                    { 
                        b_is=true;
                        rects.push_back(Rect(Point2f(max(vertexs[i][j].x+3,vertexs[t][end_f].x+3),max(vertexs[i][j].y+5,vertexs[i][z].y+5)),
                         Point2f(min(vertexs[i][z].x-1,vertexs[t][end_n].x-1),min(vertexs[t][end_f].y-1,vertexs[t][end_n].y-1))));
                         rect_index.push_back(end_f);
                         rect_index.push_back(end_n);
                         rect_index.push_back(i);
                         rect_index.push_back(t);
                         sum+=1;
                         break;
                    }
                }
                if(b_is==true)
                    break;
              }
              if(b_is==true)
                    {
                        j=z-1;
                        break;
                    }
            
            }
        }
        indexs.push_back(sum);
    }
//*******************************************************
    //check if the last operatioin  is correct.
    // for(int i=0;i<rects.size();i++)
    // {
    //     Mat tmp=srcImg(rects[i]);
    //     imshow("111",tmp);
    //     waitKey(0);

    // }
    // for(int i=0;i<indexs.size();i++)
    // {
    //     cout<<indexs[i]<<endl;
    // }

//***********************************************************************************************
    //check if the pane is blank,and keep the information in the vector named is_blank.
    sum=0;
    float sum_pixel_;
    vector<bool> is_blank ;
    vector<bool> is_selet;
    for(int i=0;i<indexs.size();i++)
    {
        for(int j=0;j<indexs[i];j++)
        {
            Mat tmpimg=src_bw(Rect(rects[sum].x+3,rects[sum].y+3,rects[sum].width-4,rects[sum].height-4));
            sum+=1;
            sum_pixel_=0;
            for(int row_=0;row_<tmpimg.rows;row_++)
            {
                for(int col_=0;col_<tmpimg.cols;col_++)
                {
                    if(tmpimg.at<uchar>(row_,col_)!=0)
                    sum_pixel_+=1;
                }
            }
            // cout<<sum_pixel_<<endl;
            if(sum_pixel_>30)
               is_blank.push_back(true);


            else
               is_blank.push_back(false);
            
            is_selet.push_back(false);
            // imshow("666",tmpimg);
            // waitKey(0);

        }
        
    }
//*****************************************************************************
   // check if the last operation  is  correct.

    // for(int i=0;i<is_blank.size();i++)
    // {
    //     cout<<is_blank[i]<<" ";
    // }


//************************************************************************************************************
    sum=0;
    
    vector<int> sum_indexs;
    for(int i=0;i<indexs.size();i++)
    {
      sum+=indexs[i];
      sum_indexs.push_back(sum);
 
    }

    sum=0;
    bool is_end;

    ofstream out("rects.txt");
    ofstream out2("nums.txt");
    int out2_num;

    if(!out.is_open())
    {
        cout<<"无法打开文件"<<endl;
        exit(0);
    }

    if(!out2.is_open())
    {
        cout<<"无法打开文件"<<endl;
        exit(0);
    }

    bool b_get;
    int sum_=0;
    
    
    for(int i=0; i<indexs.size();i++)
    {
        for(int j=0;j<indexs[i];j++)
        {
            
            if(is_blank[sum]==true)
            {
                out2_num=0;
                // cout<<sum<<endl;
                b_get=false;
                if(is_selet[sum+1]==false&&j!=indexs[i]-1&&is_blank[sum+1]==false&&rect_index[4*sum+7]==rect_index[4*sum+3])
                {
                      //记录类和空白框的位置   
                      is_selet[sum+1]=true;    
                      out<<rect_index[4*sum+4]<<" "<<rect_index[4*sum+5]<<" "<<rect_index[4*sum+6]<<" "<<rect_index[4*sum+7]<<" "<<endl;;
                      b_get=true;
                      out2<<1<<" "<<zh_dig[sum_]<<"\n";
                      sum_++;

                }

                if(i!=indexs.size()-1&&b_get==false)
                {
                for(int k=i+1;k<indexs.size();k++)
                {
                    is_end=true; 
                    for(int m=0;m<indexs[k];m++)
                    {
                        if(is_selet[sum_indexs[k-1]+m]==false&&abs(rects[sum].x-rects[sum_indexs[k-1]+m].x)<10&&abs(rects[sum].width-rects[sum_indexs[k-1]+m].width)<10&&is_blank[sum_indexs[k-1]+m]==false)
                        {
                            is_selet[sum_indexs[k-1]+m]=true;
                            int g_index=4*(sum_indexs[k-1]+m);
                            out<<rect_index[g_index]<<" "<<rect_index[g_index+1]<<" "<<rect_index[g_index+2]<<" "<<rect_index[g_index+3]<<" ";
                            b_get=true;
                            out2_num++;
                            //记录类和空白框的位置
                            is_end=false;
                            break;
                        }

                    }


                    
                    if(is_end==true||k==indexs.size()-1)
                    {
                       if(b_get==false)
                        break;
                       out<<"\n";
                       out2<<out2_num<<" "<<zh_dig[sum_]<<"\n";
                       sum_++;
                       
                       break;
                    }
                    
                   

                }
                }
                
                
                

                if(b_get==true)
                {
                    Mat tmp=src_bw(rects[sum]);
                    char s_name[16]={0};
                    sprintf(s_name,"./template/%c.bmp",'a'+sum_);
                    imwrite(s_name,tmp);
                }

            }

            sum+=1;
        }
    }

    out<<"end";
    out.close();
    out2<<"end";
    out2.close();

    ofstream out_12("out.txt");
    out_12<<sum_;
    out_12.close();



    // imshow("666",bw);
    // for(int z=0;z<indexs.size();z++)
    // {
    //     cout<<indexs[z]<<endl;
    // }




    return 0;
}
