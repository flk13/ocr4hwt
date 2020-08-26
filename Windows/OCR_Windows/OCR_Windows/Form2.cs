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
    public partial class FrmMain2 : Form
    {
        public FrmMain2()
        {
            //InitializeComponent();
            this.ShowIcon = false;
            this.FormBorderStyle = FormBorderStyle.FixedDialog;
            this.StartPosition = FormStartPosition.CenterScreen;
            this.ClientSize = new Size(220, 40);
            this.ControlBox = false;
            this.ShowInTaskbar = false;
            this.TopMost = true;
            this.Paint += new PaintEventHandler(WaitDialogPaint);
            this.ControlBox = false;
        }
        public string sValue = "正在识别 请稍等.......";
        private Cursor currentCursor;
        private Form FatherForm;




        public void Show(string sValue)
        {
            this.sValue = sValue;
            this.Show();

        }
        public void Show()
        {
            currentCursor = Cursor.Current;
            Cursor.Current = Cursors.WaitCursor;
            base.Show();
            this.Refresh();
        }
        public void Refresh(string sValue)
        {
            this.sValue = sValue;
            this.Refresh();
        }
        public void Close()
        {
            Cursor.Current = currentCursor;
            base.Close();
        }
        private void WaitDialogPaint(object sender, PaintEventArgs e)
        {
            Rectangle r = e.ClipRectangle;
            r.Inflate(-1, -1);
            StringFormat sf = new StringFormat();
            sf.Alignment = StringAlignment.Center;
            sf.LineAlignment = StringAlignment.Center;
            ControlPaint.DrawBorder3D(e.Graphics, r, Border3DStyle.RaisedInner);
            e.Graphics.DrawString(sValue, new Font("Arial", 9, FontStyle.Regular), SystemBrushes.WindowText, r, sf);
        }
    }
}
