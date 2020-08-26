using CCWin;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace OCR_Windows
{
    public partial class step3 : Skin_Metro
    {
        public string tmpfilepath,result;
        public step3(string tmppath)
        {
            InitializeComponent();
            tmpfilepath = tmppath;
        }
                         //图片文件夹地址
        private List<string> imagePathList = new List<string>(); //获取列表图片路径
        private int index;
        public string projctid;

        public static string GetHttpResponse(string url, int Timeout)
        {
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
            request.Method = "GET";
            request.ContentType = "text/html;charset=UTF-8";
            request.UserAgent = null;
            request.Timeout = Timeout;

            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            Stream myResponseStream = response.GetResponseStream();
            StreamReader myStreamReader = new StreamReader(myResponseStream, Encoding.GetEncoding("utf-8"));
            string retString = myStreamReader.ReadToEnd();
            myStreamReader.Close();
            myResponseStream.Close();

            return retString;
        }
        private void button2_Click(object sender, EventArgs e)
        {
            FrmMain2 f = new FrmMain2();
            f.Show();
            System.Threading.Thread.Sleep(5000);
            f.Hide();
            string url = "http://127.0.0.1:8000/shibie/?id=" + projctid;
            result = GetHttpResponse(url, 30000);
            Console.WriteLine(result);
            if (result=="erro")
            {
                MessageBox.Show("请先上传");
                return;
            }


            final f1 = new final(result);
            f1.Show();
            Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                addpicture();
                skinButton1.Enabled = true;
            }
            catch (Exception msg)
            {
                //报错提示 未将对象引用设置到对象的实例
                throw msg;
            }

        }
        private void addpicture()
        {
            OpenFileDialog openfile = new OpenFileDialog();
            openfile.Title = "请选择要插入的图片";
            openfile.Filter = "图片|*.jpg;*.bmp;*.gif";
            openfile.CheckFileExists = true;
            openfile.CheckPathExists = true;
            openfile.Multiselect = true;
            if (openfile.ShowDialog() == DialogResult.OK && (openfile.FileName != ""))
            {

                string[] filePath = openfile.FileNames;
                imageList1.ColorDepth = ColorDepth.Depth24Bit;
                imageList1.ImageSize = new Size(128, 128);
                for (int i = 0; i < filePath.Length; ++i)
                {
                    imagePathList.Add(filePath[i]);
                    //图片加载到ImageList控件和imageList图片列表
                    FileStream fs = new FileStream(filePath[i], FileMode.Open, FileAccess.Read);
                    int byteLength = (int)fs.Length;
                    byte[] fileBytes = new byte[byteLength];
                    fs.Read(fileBytes, 0, byteLength);
                    fs.Close();
                    this.imageList1.Images.Add(Image.FromStream(new MemoryStream(fileBytes)));
                    //this.imageList1.Images.Add(Image.FromFile(filePath[i]));
                }
                //记录图片源路径 双击显示图片时使用

                this.listView1.Items.Clear();
                this.listView1.LargeImageList = this.imageList1;
                this.listView1.View = View.LargeIcon;        //大图标显示  

                //开始绑定
                this.listView1.BeginUpdate();
                //增加图片至ListView控件中
                for (int i = 0; i < imageList1.Images.Count; i++)
                {
                    ListViewItem lvi = new ListViewItem();
                    lvi.ImageIndex = i;
                    lvi.Text = "pic" + i;
                    this.listView1.Items.Add(lvi);
                }

                this.listView1.EndUpdate();

            }

            openfile.Dispose();

        }

        private void listView1_DoubleClick(object sender, EventArgs e)
        {
            if (this.listView1.SelectedItems.Count == 0)
                return;
            //采用索引方式 imagePathList记录图片真实路径
            index = this.listView1.SelectedItems[0].Index;
            //显示图片

            FileStream fs = new FileStream(imagePathList[index], FileMode.Open, FileAccess.Read);
            int byteLength = (int)fs.Length;
            byte[] fileBytes = new byte[byteLength];
            fs.Read(fileBytes, 0, byteLength);
            fs.Close();
            this.pictureBox1.Image = Image.FromStream(new MemoryStream(fileBytes));


            //this.pictureBox1.Image = Image.FromFile(imagePathList[index]);
            //图片被拉伸或收缩适合pictureBox大小
            pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
        }

        private void listView1_MouseClick(object sender, MouseEventArgs e)
        {
            if (listView1.SelectedItems.Count > 0)
            {
                if (e.Button == MouseButtons.Right)
                {
                    if (this.listView1.SelectedItems.Count > 0)
                    {
                        this.contextMenuStrip1.Show(this, e.Location);
                    }
                    else
                    {
                        this.contextMenuStrip1.Hide();
                    }
                }
            }
        }

        private void 删除ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int i = listView1.SelectedItems.Count - 1; i >= 0; i--)
            {
                ListViewItem item = listView1.SelectedItems[i];
                listView1.Items.Remove(item);
                imagePathList.RemoveAt(item.ImageIndex);
            }

        }

        private void 添加图片ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            addpicture();
        }

        private void step3_FormClosed(object sender, FormClosedEventArgs e)
        {
            System.Environment.Exit(0);
        }
        /// <summary>
        /// 批量上传图片
        /// </summary>
        /// <param name="srcurl">服务器路径</param>
        /// <param name="imagesPath">图片文件夹路径</param>
        /// <param name="files">图片名称</param>
        public void UpLoadFile(string srcurl)
        {
            List<string> files = imagePathList;
            int count = 1;
            projctid = DateTime.Now.ToString("yyyyMMddHHmmss");
            foreach (string imageName in files)
            {
                string name = imageName;
                string url = srcurl;

                string base64string = ImgToBase64String(name);


                base64string = ("id=" + projctid + "&img=" + base64string).Replace("+","%2B");

                //Console.WriteLine(base64string);
                byte[] byteArray = System.Text.Encoding.UTF8.GetBytes(base64string);
                
                HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
                request.ContentType = "application/x-www-form-urlencoded";
                request.Method = "POST";
                Encoding encoding = Encoding.UTF8;
                request.ContentLength = byteArray.Length;
                Stream requestStream = request.GetRequestStream();
                requestStream.Write(byteArray, 0, byteArray.Length);
                requestStream.Close();


                HttpWebResponse response = (HttpWebResponse)request.GetResponse();
                StreamReader streamReader = new StreamReader(response.GetResponseStream(), encoding);
                string retString = streamReader.ReadToEnd();
                streamReader.Close();

                //Console.WriteLine(retString);

            }

            


        }

        private void skinButton1_Click(object sender, EventArgs e)
        {
            string url = "http://127.0.0.1:8000/uploadimg/";
            UpLoadFile(url);
            string name = tmpfilepath;
            string urltmp = "http://127.0.0.1:8000/uploadtem/";

            string base64string = ImgToBase64String(name);


            base64string = ("id=" + projctid + "&img=" + base64string).Replace("+", "%2B");

            //Console.WriteLine(base64string);
            byte[] byteArray = System.Text.Encoding.UTF8.GetBytes(base64string);

            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(urltmp);
            request.ContentType = "application/x-www-form-urlencoded";
            request.Method = "POST";
            Encoding encoding = Encoding.UTF8;
            request.ContentLength = byteArray.Length;
            Stream requestStream = request.GetRequestStream();
            requestStream.Write(byteArray, 0, byteArray.Length);
            
            requestStream.Close();


            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            StreamReader streamReader = new StreamReader(response.GetResponseStream(), encoding);
            string retString = streamReader.ReadToEnd();
            streamReader.Close();

            MessageBox.Show("上传成功，点击下一步开始识别！");
            button2.Enabled = true;

        }

        protected string ImgToBase64String(string Imagefilename)
        {
            try
            {
                Bitmap bmp = new Bitmap(Imagefilename);

                MemoryStream ms = new MemoryStream();
                bmp.Save(ms, System.Drawing.Imaging.ImageFormat.Jpeg);
                byte[] arr = new byte[ms.Length];
                ms.Position = 0;
                ms.Read(arr, 0, (int)ms.Length);
                ms.Close();
                return Convert.ToBase64String(arr);
            }
            catch (Exception ex)
            {
                return null;
            }
        }

        private void step3_Load(object sender, EventArgs e)
        {
            
        }
    }

}
