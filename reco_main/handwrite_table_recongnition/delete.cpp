#include<iostream>
#include <unistd.h>
#include<dirent.h>
#include<stdio.h>
#include<vector>


using namespace std;
int main()
{

       DIR *dir=opendir("./savedir_digit");
       dirent* p=NULL;    
       while((p=readdir(dir))!=NULL)
       {
           if(p->d_name[0]=='.')
           {
               continue;
           }
           else
           {
               string str="./savedir_digit/"+string(p->d_name);
               unlink(str.c_str());
           }
       }
       closedir(dir);

       dir=opendir("./template");
       p=NULL;    
       while((p=readdir(dir))!=NULL)
       {
           if(p->d_name[0]=='.')
           {
               continue;
           }
           else
           {
               string str="./template/"+string(p->d_name);
               unlink(str.c_str());
           }
       }
       closedir(dir);

       dir=opendir("./table_img");
       p=NULL;    
       while((p=readdir(dir))!=NULL)
       {
           if(p->d_name[0]=='.')
           {
               continue;
           }
           else
           {
               string str="./table_img/"+string(p->d_name);
               unlink(str.c_str());
           }
       }
       closedir(dir);

       dir=opendir("./savedir_zh");
       p=NULL;    
       while((p=readdir(dir))!=NULL)
       {
           if(p->d_name[0]=='.')
           {
               continue;
           }
           else
           {
               string str="./savedir_zh/"+string(p->d_name);
               unlink(str.c_str());
           }
       }
       closedir(dir);


       dir=opendir("./savedir_zh0");
       p=NULL;    
       while((p=readdir(dir))!=NULL)
       {
           if(p->d_name[0]=='.')
           {
               continue;
           }
           else
           {
               string str="./savedir_zh0/"+string(p->d_name);
               unlink(str.c_str());
           }
       }
       closedir(dir);


       dir=opendir("./save_img");
       p=NULL;    
       while((p=readdir(dir))!=NULL)
       {
           if(p->d_name[0]=='.')
           {
               continue;
           }
           else
           {
               string str="./save_img/"+string(p->d_name);
               unlink(str.c_str());
           }
       }
       closedir(dir);





       return 0;
}