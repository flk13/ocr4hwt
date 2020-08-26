using CCWin;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;



namespace OCR_Windows
{

    public partial class step1 : Skin_Metro
    {
        public string tempfile;
        public step1()
        {
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                //step2 s2 = new step2(tempfile);
                //s2.Show();
                step3 s3 = new step3(tempfile);
                s3.Show();
                this.Close();
            }
            else
            {
                MessageBox.Show("请先选择图片");
            }

            
        }

        private void step1_Load(object sender, EventArgs e)
        {
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog openfile = new OpenFileDialog();
            openfile.Title = "请选择要插入的图片";
            openfile.Filter = "图片|*.jpg;*.bmp;*.gif" ;
            openfile.CheckFileExists = true;
            openfile.CheckPathExists = true;
            openfile.Multiselect = false;
            if (openfile.ShowDialog() == DialogResult.OK && (openfile.FileName != ""))
            {

                pictureBox1.ImageLocation = openfile.FileName;
                tempfile = openfile.FileName;
                pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
            }

            openfile.Dispose();

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}
