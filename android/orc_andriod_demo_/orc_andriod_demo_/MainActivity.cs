using System.Collections.Generic;
using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Widget;
using Android.Provider;
using Android.Content.PM;
using Android.Graphics;
using Android.Net;
using Java.IO;
using Android.Support.V4.Content;
using Newtonsoft.Json;
using ocr1;
using System.Net;
using System.Text;
using System.IO;
using System;

namespace orc_andriod_demo_
{
    [Activity(Label = "ch2003MainActivity" ,MainLauncher = true)]
    public class MainActivity : Activity
    {
        private Java.IO.File file;
        string dirPath;
        string filePath;
        private ImageView imageView1;
        //private VideoView videoView1;
        private TextView textView1;
        private MediaType mediaType;

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            SetContentView(Resource.Layout.Main);
            textView1 = FindViewById<TextView>(Resource.Id.textView1);
            imageView1 = FindViewById<ImageView>(Resource.Id.ch2003_imageView1);
            //videoView1 = FindViewById<VideoView>(Resource.Id.ch2003_videoView1);
            Button btn1 = FindViewById<Button>(Resource.Id.ch2003_btn1);
            Button button2 = FindViewById<Button>(Resource.Id.button2);
            //Button btn2 = FindViewById<Button>(Resource.Id.ch2003_btn2);

            //判断相机是否可用（是否有一个App正在准备拍照）
            if (IsThereAnAppToTakePictures())
            {
                dirPath = $"{Android.OS.Environment.ExternalStorageDirectory.Path}/{Android.OS.Environment.DirectoryPictures}/MyDemosPictures";
                if (!System.IO.Directory.Exists(dirPath))
                {
                    System.IO.Directory.CreateDirectory(dirPath);
                }
                btn1.Click += delegate
                {
                    //拍照
                    mediaType = MediaType.Image;
                    Intent imageIntent = new Intent(MediaStore.ActionImageCapture);
                    filePath = dirPath + $"/PP{System.DateTime.Now:yyyyMMdd_hhmmss}.jpg";
                    file = new Java.IO.File(filePath);
                    //imageIntent.PutExtra(MediaStore.ExtraOutput, Uri.FromFile(file));
                    //StartActivityForResult(imageIntent, 100);
                    //File cameraPhoto = new File(filePath);
                    Intent takePhotoIntent = new Intent(MediaStore.ActionImageCapture);
                    Android.Net.Uri photoUri = FileProvider.GetUriForFile(
                                this,
                                "orc_andriod_demo_.demo.fileprovider",
                                file);
                    takePhotoIntent.PutExtra(MediaStore.ExtraOutput, photoUri);
                    StartActivityForResult(takePhotoIntent, 100);
                    
                };
                
            }
            else
            {
                //如果相机不可用，禁用按钮
                btn1.Text = "相机不可用，无法拍照";
                //btn2.Text = "相机不可用，无法录制视频";
                //btn1.Enabled = btn2.Enabled = false;
            }

            button2.Click += delegate
            {
                if(filePath==null)
                    button2.Text="请先选择照片";
                else
                {
                    string posturl = "http://recognition.image.myqcloud.com/ocr/handwriting";
                    var retjson = UseImage(posturl, filePath);
                    JsToData(retjson);
                }
            };
        }

        /// <summary>
        /// 判断相机是否可用（是否有一个App正在准备拍照）
        /// </summary>
        /// <returns></returns>
        private bool IsThereAnAppToTakePictures()
        {
            Intent intent = new Intent(MediaStore.ActionImageCapture);
            IList<ResolveInfo> availableActivities = PackageManager.QueryIntentActivities(intent, PackageInfoFlags.MatchDefaultOnly);
            return availableActivities != null && availableActivities.Count > 0;
        }

        protected override void OnActivityResult(int requestCode, [GeneratedEnum] Result resultCode, Intent data)
        {
            base.OnActivityResult(requestCode, resultCode, data);

            // 使其可以在图库中可用，即浏览图库时能看到相机拍摄的照片
            Intent mediaScanIntent = new Intent(Intent.ActionMediaScannerScanFile);
            Android.Net.Uri contentUri = Android.Net.Uri.FromFile(file);
            mediaScanIntent.SetData(contentUri);
            SendBroadcast(mediaScanIntent);

            if (mediaType == MediaType.Image)
            {
                imageView1.Visibility = Android.Views.ViewStates.Visible;
                //videoView1.Visibility = Android.Views.ViewStates.Gone;
                //辅助程序（ch2003BitmapHelpers.cs）会自动缩放该图片以适应屏幕大小。
                int width = Resources.DisplayMetrics.WidthPixels;
                int height = imageView1.Height;
                using (Bitmap bitmap = ch2003BitmapHelpers.LoadAndResizeBitmap(filePath, width, height))
                {
                    ch2003BitmapHelpers.RecycleBitmap(imageView1);
                    imageView1.SetImageBitmap(bitmap);
                    imageView1.RefreshDrawableState();
                }
                textView1.Text = "拍照结果：\n" + filePath;
            }
            
        }
        static string UseImage(string url, string path)
        {
            HttpWebRequest webReq = (HttpWebRequest)WebRequest.Create(new System.Uri(url));
            Stream memStream = new MemoryStream();
            webReq.Method = "POST";
            string boundary = "--------------" + DateTime.Now.Ticks.ToString("x");// 边界符  
            webReq.ContentType = "multipart/form-data; boundary=" + boundary;
            byte[] enter = Encoding.ASCII.GetBytes("\r\n");  //换行
            memStream.Write(enter, 0, enter.Length);
            Dictionary<string, string> dic = new Dictionary<string, string>()
            {
                {"appid",config.appid} ,
                {"bucket",config.bucket}
            };
            //写入文本字段
            string inputPartHeaderFormat = "--" + boundary + "\r\n" + "Content-Disposition:form-data;name=\"{0}\";" + "\r\n\r\n{1}\r\n";
            foreach (var kv in dic)
            {
                string inputPartHeader = string.Format(inputPartHeaderFormat, kv.Key, kv.Value);
                var inputPartHeaderBytes = Encoding.ASCII.GetBytes(inputPartHeader);
                memStream.Write(inputPartHeaderBytes, 0, inputPartHeaderBytes.Length);
            }
            var fileStream = new FileStream(path, FileMode.Open, FileAccess.Read);
            // 写入文件 
            
            //压缩图片
            string imagePartHeader = "--" + boundary + "\r\n" +
                                     "Content-Disposition: form-data; name=\"{0}\"; filename=\"{1}\"\r\n" +
                                     "Content-Type: image/jpeg\r\n\r\n";
            var header = string.Format(imagePartHeader, "image", "1.jpg");
            var headerbytes = Encoding.UTF8.GetBytes(header);
            memStream.Write(headerbytes, 0, headerbytes.Length);
            var buffer = new byte[1024];
            int bytesRead;
            while ((bytesRead = fileStream.Read(buffer, 0, buffer.Length)) != 0)
            {
                memStream.Write(buffer, 0, bytesRead);
            }
            // 最后的结束符  
            byte[] endBoundary = Encoding.ASCII.GetBytes("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" + "\r\n" + boundary + "--\r\n");
            memStream.Write(endBoundary, 0, endBoundary.Length);
            webReq.ContentLength = memStream.Length;
            webReq.Headers.Add(HttpRequestHeader.Authorization, OCRApi.HmacSha1Sign());
            webReq.Host = config.Host;
            var requestStream = webReq.GetRequestStream();
            memStream.Position = 0;
            memStream.CopyTo(requestStream);
            HttpWebResponse response = (HttpWebResponse)webReq.GetResponse();
            StreamReader sr = new StreamReader(response.GetResponseStream(), Encoding.UTF8);
            var ret = sr.ReadToEnd();
            sr.Close();
            response.Close();
            requestStream.Close();
            memStream.Close();
            return ret;
        }

        void JsToData(string jsd)
        {
            string data = "";
            jsonparser.RootObject rb = JsonConvert.DeserializeObject<jsonparser.RootObject>(jsd);
            foreach (jsonparser.Items ep in rb.data.items)
            {
                data = data + "\n" + ep.itemstring;
            }
            data = data.Remove(0, 1);
            textView1.Text = data;
        }

        
    }
}