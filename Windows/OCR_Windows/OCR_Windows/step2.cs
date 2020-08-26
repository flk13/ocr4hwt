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
    public partial class step2 : Skin_Metro
    {
        public string tmfile;
        public step2(string tmpfile)
        {
            InitializeComponent();
            tmfile = tmpfile;
        }

        private void step2_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            step3 s3=new step3(tmfile);
            s3.Show();
            this.Close();
        }

        private void step2_FormClosed(object sender, FormClosedEventArgs e)
        {
            System.Environment.Exit(0);
        }
    }
}
