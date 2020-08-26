using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using CCWin;

namespace OCR_Windows
{
    public partial class FrmMain : Skin_Metro
    {
        public FrmMain()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            step1 s1 = new step1();
            s1.Show();
            this.Hide();
        }
    }
}
