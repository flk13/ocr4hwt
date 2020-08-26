namespace OCR_Windows
{
    partial class step2
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.v1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.v2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.v3 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.专业班级 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.v4 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.邮箱 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.兴趣爱好 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.label3 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Font = new System.Drawing.Font("宋体", 24F);
            this.label1.Location = new System.Drawing.Point(252, 30);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(319, 33);
            this.label1.TabIndex = 0;
            this.label1.Text = "Step2：设计你的表格";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.Color.Transparent;
            this.label2.Font = new System.Drawing.Font("宋体", 16F);
            this.label2.Location = new System.Drawing.Point(29, 99);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(362, 22);
            this.label2.TabIndex = 1;
            this.label2.Text = "根据你的图片：我们得到以下信息：";
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToOrderColumns = true;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.v1,
            this.v2,
            this.v3,
            this.专业班级,
            this.v4,
            this.邮箱,
            this.兴趣爱好});
            this.dataGridView1.Location = new System.Drawing.Point(12, 205);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowTemplate.Height = 23;
            this.dataGridView1.Size = new System.Drawing.Size(869, 449);
            this.dataGridView1.TabIndex = 2;
            // 
            // v1
            // 
            this.v1.HeaderText = "姓名";
            this.v1.Name = "v1";
            // 
            // v2
            // 
            this.v2.HeaderText = "年级";
            this.v2.Name = "v2";
            // 
            // v3
            // 
            this.v3.HeaderText = "学号";
            this.v3.Name = "v3";
            // 
            // 专业班级
            // 
            this.专业班级.HeaderText = "专业班级";
            this.专业班级.Name = "专业班级";
            // 
            // v4
            // 
            this.v4.HeaderText = "电话";
            this.v4.Name = "v4";
            // 
            // 邮箱
            // 
            this.邮箱.HeaderText = "邮箱";
            this.邮箱.Name = "邮箱";
            // 
            // 兴趣爱好
            // 
            this.兴趣爱好.HeaderText = "兴趣爱好";
            this.兴趣爱好.Name = "兴趣爱好";
            this.兴趣爱好.Width = 300;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.Color.Transparent;
            this.label3.Font = new System.Drawing.Font("宋体", 16F);
            this.label3.Location = new System.Drawing.Point(29, 164);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(362, 22);
            this.label3.TabIndex = 3;
            this.label3.Text = "请根据你的实际需要进行排序和修改";
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.Transparent;
            this.button1.Location = new System.Drawing.Point(713, 163);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(133, 31);
            this.button1.TabIndex = 4;
            this.button1.Text = "我已完成！下一步";
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // step2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(893, 666);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "step2";
            this.Text = "Step2";
            this.Load += new System.EventHandler(this.step2_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.DataGridViewTextBoxColumn v1;
        private System.Windows.Forms.DataGridViewTextBoxColumn v2;
        private System.Windows.Forms.DataGridViewTextBoxColumn v3;
        private System.Windows.Forms.DataGridViewTextBoxColumn 专业班级;
        private System.Windows.Forms.DataGridViewTextBoxColumn v4;
        private System.Windows.Forms.DataGridViewTextBoxColumn 邮箱;
        private System.Windows.Forms.DataGridViewTextBoxColumn 兴趣爱好;
    }
}