namespace NewGUI
{
    partial class Aktuatory
    {
        /// <summary> 
        /// Vyžaduje se proměnná návrháře.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Uvolněte všechny používané prostředky.
        /// </summary>
        /// <param name="disposing">hodnota true, když by se měl spravovaný prostředek odstranit; jinak false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Kód vygenerovaný pomocí Návrháře komponent

        /// <summary> 
        /// Metoda vyžadovaná pro podporu Návrháře - neupravovat
        /// obsah této metody v editoru kódu.
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
            this.btnStart.Location = new System.Drawing.Point(42, 87);
            this.btnStart.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(86, 37);
            this.btnStart.TabIndex = 0;
            this.btnStart.Text = "START";
            this.btnStart.UseVisualStyleBackColor = true;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // btnStop
            // 
            this.btnStop.Location = new System.Drawing.Point(146, 87);
            this.btnStop.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(95, 37);
            this.btnStop.TabIndex = 0;
            this.btnStop.Text = "STOP";
            this.btnStop.UseVisualStyleBackColor = true;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // MainTextBox
            // 
            this.MainTextBox.Location = new System.Drawing.Point(42, 169);
            this.MainTextBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.MainTextBox.Name = "MainTextBox";
            this.MainTextBox.Size = new System.Drawing.Size(327, 20);
            this.MainTextBox.TabIndex = 1;
            this.MainTextBox.TextChanged += new System.EventHandler(this.MainTextBox_TextChanged);
            // 
            // AktBox
            // 
            this.AktBox.FormattingEnabled = true;
            this.AktBox.Location = new System.Drawing.Point(277, 66);
            this.AktBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.AktBox.Name = "AktBox";
            this.AktBox.Size = new System.Drawing.Size(92, 21);
            this.AktBox.TabIndex = 2;
            this.AktBox.SelectedIndexChanged += new System.EventHandler(this.AktBox_SelectedIndexChanged);
            // 
            // ModBox
            // 
            this.ModBox.FormattingEnabled = true;
            this.ModBox.Items.AddRange(new object[] {
            "CONFIG",
            "RESET"});
            this.ModBox.Location = new System.Drawing.Point(277, 25);
            this.ModBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.ModBox.Name = "ModBox";
            this.ModBox.Size = new System.Drawing.Size(92, 21);
            this.ModBox.TabIndex = 2;
            this.ModBox.SelectedIndexChanged += new System.EventHandler(this.ModBox_SelectedIndexChanged);
            // 
            // SeznamAkt
            // 
            this.SeznamAkt.AutoSize = true;
            this.SeznamAkt.Location = new System.Drawing.Point(275, 51);
            this.SeznamAkt.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.SeznamAkt.Name = "SeznamAkt";
            this.SeznamAkt.Size = new System.Drawing.Size(94, 13);
            this.SeznamAkt.TabIndex = 3;
            this.SeznamAkt.Text = "Seznam Aktuatoru";
            this.SeznamAkt.Click += new System.EventHandler(this.SeznamAkt_Click);
            // 
            // Mod
            // 
            this.Mod.AutoSize = true;
            this.Mod.Location = new System.Drawing.Point(275, 9);
            this.Mod.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.Mod.Name = "Mod";
            this.Mod.Size = new System.Drawing.Size(28, 13);
            this.Mod.TabIndex = 4;
            this.Mod.Text = "Mod";
            this.Mod.Click += new System.EventHandler(this.Mod_Click);
            // 
            // ComBox
            // 
            this.ComBox.FormattingEnabled = true;
            this.ComBox.Location = new System.Drawing.Point(42, 27);
            this.ComBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.ComBox.Name = "ComBox";
            this.ComBox.Size = new System.Drawing.Size(66, 21);
            this.ComBox.TabIndex = 2;
            // 
            // Port
            // 
            this.Port.AutoSize = true;
            this.Port.Location = new System.Drawing.Point(40, 11);
            this.Port.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.Port.Name = "Port";
            this.Port.Size = new System.Drawing.Size(26, 13);
            this.Port.TabIndex = 5;
            this.Port.Text = "Port";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(334, 99);
            this.textBox1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(35, 20);
            this.textBox1.TabIndex = 6;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(334, 121);
            this.textBox2.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(35, 20);
            this.textBox2.TabIndex = 6;
            this.textBox2.TextChanged += new System.EventHandler(this.textBox2_TextChanged);
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(334, 144);
            this.textBox3.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(35, 20);
            this.textBox3.TabIndex = 6;
            this.textBox3.TextChanged += new System.EventHandler(this.textBox3_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(275, 101);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(21, 13);
            this.label1.TabIndex = 7;
            this.label1.Text = "raz";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(275, 123);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(25, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "dva";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(275, 146);
            this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(15, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "tri";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(42, 49);
            this.btnConnect.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(60, 20);
            this.btnConnect.TabIndex = 0;
            this.btnConnect.Text = "Připojit";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(190, 17);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(27, 29);
            this.pictureBox1.TabIndex = 8;
            this.pictureBox1.TabStop = false;
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(309, 100);
            this.checkBox1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(66, 17);
            this.checkBox1.TabIndex = 9;
            this.checkBox1.Text = "všechny";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(122, 25);
            this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(64, 13);
            this.label4.TabIndex = 10;
            this.label4.Text = "Zvolte COM";
            // 
            // Aktuatory
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
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
            this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Name = "Aktuatory";
            this.Size = new System.Drawing.Size(507, 421);
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

