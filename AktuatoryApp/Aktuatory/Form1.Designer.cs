namespace Aktuatory
{
    partial class Aktuator
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
            this.btnStart = new System.Windows.Forms.Button();
            this.btnStop = new System.Windows.Forms.Button();
            this.MainTextBox = new System.Windows.Forms.TextBox();
            this.AktBox = new System.Windows.Forms.ComboBox();
            this.ModBox = new System.Windows.Forms.ComboBox();
            this.SeznamAkt = new System.Windows.Forms.Label();
            this.Mod = new System.Windows.Forms.Label();
            this.ComBox = new System.Windows.Forms.ComboBox();
            this.Port = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.btnConnect = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.label4 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // btnStart
            // 
            this.btnStart.Location = new System.Drawing.Point(56, 107);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(115, 46);
            this.btnStart.TabIndex = 0;
            this.btnStart.Text = "START";
            this.btnStart.UseVisualStyleBackColor = true;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // btnStop
            // 
            this.btnStop.Location = new System.Drawing.Point(195, 107);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(127, 46);
            this.btnStop.TabIndex = 0;
            this.btnStop.Text = "STOP";
            this.btnStop.UseVisualStyleBackColor = true;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // MainTextBox
            // 
            this.MainTextBox.Location = new System.Drawing.Point(56, 206);
            this.MainTextBox.Name = "MainTextBox";
            this.MainTextBox.Size = new System.Drawing.Size(409, 22);
            this.MainTextBox.TabIndex = 1;
            // 
            // AktBox
            // 
            this.AktBox.FormattingEnabled = true;
            this.AktBox.Location = new System.Drawing.Point(344, 79);
            this.AktBox.Name = "AktBox";
            this.AktBox.Size = new System.Drawing.Size(121, 24);
            this.AktBox.TabIndex = 2;
            this.AktBox.SelectedIndexChanged += new System.EventHandler(this.AktBox_SelectedIndexChanged);
            // 
            // ModBox
            // 
            this.ModBox.FormattingEnabled = true;
            this.ModBox.Items.AddRange(new object[] {
            "CONFIG",
            "RESET"});
            this.ModBox.Location = new System.Drawing.Point(344, 28);
            this.ModBox.Name = "ModBox";
            this.ModBox.Size = new System.Drawing.Size(121, 24);
            this.ModBox.TabIndex = 2;
            this.ModBox.SelectedIndexChanged += new System.EventHandler(this.ModBox_SelectedIndexChanged);
            // 
            // SeznamAkt
            // 
            this.SeznamAkt.AutoSize = true;
            this.SeznamAkt.Location = new System.Drawing.Point(341, 60);
            this.SeznamAkt.Name = "SeznamAkt";
            this.SeznamAkt.Size = new System.Drawing.Size(115, 16);
            this.SeznamAkt.TabIndex = 3;
            this.SeznamAkt.Text = "Seznam Aktuatoru";
            // 
            // Mod
            // 
            this.Mod.AutoSize = true;
            this.Mod.Location = new System.Drawing.Point(341, 9);
            this.Mod.Name = "Mod";
            this.Mod.Size = new System.Drawing.Size(34, 16);
            this.Mod.TabIndex = 4;
            this.Mod.Text = "Mod";
            // 
            // ComBox
            // 
            this.ComBox.FormattingEnabled = true;
            this.ComBox.Location = new System.Drawing.Point(56, 33);
            this.ComBox.Name = "ComBox";
            this.ComBox.Size = new System.Drawing.Size(87, 24);
            this.ComBox.TabIndex = 2;
            // 
            // Port
            // 
            this.Port.AutoSize = true;
            this.Port.Location = new System.Drawing.Point(53, 14);
            this.Port.Name = "Port";
            this.Port.Size = new System.Drawing.Size(31, 16);
            this.Port.TabIndex = 5;
            this.Port.Text = "Port";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(420, 119);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(45, 22);
            this.textBox1.TabIndex = 6;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(420, 147);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(45, 22);
            this.textBox2.TabIndex = 6;
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(420, 175);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(45, 22);
            this.textBox3.TabIndex = 6;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(341, 122);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(25, 16);
            this.label1.TabIndex = 7;
            this.label1.Text = "raz";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(341, 149);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(30, 16);
            this.label2.TabIndex = 7;
            this.label2.Text = "dva";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(341, 177);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(17, 16);
            this.label3.TabIndex = 7;
            this.label3.Text = "tri";
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(56, 60);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(80, 24);
            this.btnConnect.TabIndex = 0;
            this.btnConnect.Text = "Připojit";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(245, 21);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(36, 36);
            this.pictureBox1.TabIndex = 8;
            this.pictureBox1.TabStop = false;
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(386, 121);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(79, 20);
            this.checkBox1.TabIndex = 9;
            this.checkBox1.Text = "všechny";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(162, 31);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(77, 16);
            this.label4.TabIndex = 10;
            this.label4.Text = "Zvolte COM";
            // 
            // Aktuator
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(491, 466);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox3);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.Port);
            this.Controls.Add(this.Mod);
            this.Controls.Add(this.SeznamAkt);
            this.Controls.Add(this.ComBox);
            this.Controls.Add(this.ModBox);
            this.Controls.Add(this.AktBox);
            this.Controls.Add(this.MainTextBox);
            this.Controls.Add(this.btnStop);
            this.Controls.Add(this.btnConnect);
            this.Controls.Add(this.btnStart);
            this.Name = "Aktuator";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.Button btnStop;
        private System.Windows.Forms.TextBox MainTextBox;
        private System.Windows.Forms.ComboBox AktBox;
        private System.Windows.Forms.ComboBox ModBox;
        private System.Windows.Forms.Label SeznamAkt;
        private System.Windows.Forms.Label Mod;
        private System.Windows.Forms.ComboBox ComBox;
        private System.Windows.Forms.Label Port;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.Label label4;
    }
}