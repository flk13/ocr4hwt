using CCWin;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace OCR_Windows
{
    public partial class final : Skin_Metro
    {
        public string result;
        public final(string res)
        {
            
            InitializeComponent();
            result = res;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            SaveFileDialog kk = new SaveFileDialog();

            kk.Title = "保存EXECL文件";

            kk.Filter = "EXECL文件(*.xls) |*.xls |所有文件(*.*) |*.*";

            kk.FilterIndex = 1;

            if (kk.ShowDialog() == DialogResult.OK)

            {

                string FileName = kk.FileName ;

                if (File.Exists(FileName))

                    File.Delete(FileName);

                FileStream objFileStream;

                StreamWriter objStreamWriter;

                string strLine = "";

                objFileStream = new FileStream(FileName, FileMode.OpenOrCreate, FileAccess.Write);

                objStreamWriter = new StreamWriter(objFileStream, System.Text.Encoding.Unicode);

                for (int i = 0; i < dataGridView1.Columns.Count; i++)

                {

                    if (dataGridView1.Columns[i].Visible == true)

                    {

                        strLine = strLine + dataGridView1.Columns[i].HeaderText.ToString() + Convert.ToChar(9);

                    }

                }

                objStreamWriter.WriteLine(strLine);

                strLine = "";



                for (int i = 0; i < dataGridView1.Rows.Count; i++)

                {

                    if (dataGridView1.Columns[0].Visible == true)

                    {

                        if (dataGridView1.Rows[i].Cells[0].Value == null)

                            strLine = strLine + " " + Convert.ToChar(9);

                        else

                            strLine = strLine + dataGridView1.Rows[i].Cells[0].Value.ToString() + Convert.ToChar(9);

                    }

                    for (int j = 1; j < dataGridView1.Columns.Count; j++)

                    {

                        if (dataGridView1.Columns[j].Visible == true)

                        {

                            if (dataGridView1.Rows[i].Cells[j].Value == null)

                                strLine = strLine + " " + Convert.ToChar(9);

                            else

                            {

                                string rowstr = "";

                                rowstr = dataGridView1.Rows[i].Cells[j].Value.ToString();

                                if (rowstr.IndexOf("\r\n") > 0)

                                    rowstr = rowstr.Replace("\r\n", " ");

                                if (rowstr.IndexOf("\t") > 0)

                                    rowstr = rowstr.Replace("\t", " ");

                                strLine = strLine + rowstr + Convert.ToChar(9);

                            }

                        }

                    }

                    objStreamWriter.WriteLine(strLine);

                    strLine = "";

                }

                objStreamWriter.Close();

                objFileStream.Close();

                MessageBox.Show(this, "保存EXCEL成功", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);

            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            System.Environment.Exit(0);
        }

        private void final_FormClosed(object sender, FormClosedEventArgs e)
        {
            System.Environment.Exit(0);
        }

        private void final_Load(object sender, EventArgs e)
        {
            string[] sarrayresl = result.Split('\n');
            int count = 0;
            foreach(string est in sarrayresl)
            {
                if(count==0)
                {
                    string[] headercell = est.Split(' ');
                    foreach(string hed in headercell)
                    {
                        DataGridViewTextBoxColumn acCode = new DataGridViewTextBoxColumn();
                        acCode.Name = "colunm";
                        acCode.HeaderText = hed;
                        acCode.AutoSizeMode=DataGridViewAutoSizeColumnMode.Fill;
                        dataGridView1.Columns.Add(acCode);
                        
                    }
                }
                else
                {
                    string[] rowstring = est.Split(' ');
                    int row_count = 0;
                    DataGridViewRow dr = new DataGridViewRow();
                    dr.CreateCells(dataGridView1);
                    foreach(string row in rowstring)
                    {

                        dr.Cells[row_count++].Value = row;
                        
                    }
                    dataGridView1.Rows.Add(dr);
                }
                count++;

            }
            /*int index = this.dataGridView1.Rows.Add();
            this.dataGridView1.Rows[index].Cells[0].Value = "张三";
            this.dataGridView1.Rows[index].Cells[1].Value = "2016";
            this.dataGridView1.Rows[index].Cells[2].Value = "2016177808";
            this.dataGridView1.Rows[index].Cells[3].Value = "网络工程";
            this.dataGridView1.Rows[index].Cells[4].Value = "17781259989";
            this.dataGridView1.Rows[index].Cells[5].Value = "123@qq.com";
            this.dataGridView1.Rows[index].Cells[6].Value = "篮球、羽毛球";
            int index2 = this.dataGridView1.Rows.Add();
            this.dataGridView1.Rows[index2].Cells[0].Value = "李四";
            this.dataGridView1.Rows[index2].Cells[1].Value = "2017";
            this.dataGridView1.Rows[index2].Cells[2].Value = "2017117824";
            this.dataGridView1.Rows[index2].Cells[3].Value = "交运";
            this.dataGridView1.Rows[index2].Cells[4].Value = "18508281321";
            this.dataGridView1.Rows[index2].Cells[5].Value = "123@sina.com";
            this.dataGridView1.Rows[index2].Cells[6].Value = "打游戏";*/
        }
    }
}
