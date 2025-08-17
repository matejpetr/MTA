namespace NewGUI
{
    partial class Home
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.Simulator_button = new System.Windows.Forms.Button();
            this.panel2 = new System.Windows.Forms.Panel();
            this.Sensor_button = new System.Windows.Forms.Button();
            this.panel3 = new System.Windows.Forms.Panel();
            this.Aktuatory_button = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.Simulator_button);
            this.panel1.Location = new System.Drawing.Point(140, 37);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(235, 134);
            this.panel1.TabIndex = 1;
            // 
            // Simulator_button
            // 
            this.Simulator_button.Image = global::NewGUI.Properties.Resources.esp_simulator_mini;
            this.Simulator_button.Location = new System.Drawing.Point(-63, -18);
            this.Simulator_button.Name = "Simulator_button";
            this.Simulator_button.Size = new System.Drawing.Size(368, 169);
            this.Simulator_button.TabIndex = 1;
            this.Simulator_button.UseVisualStyleBackColor = true;
            this.Simulator_button.Click += new System.EventHandler(this.Simulator_button_Click);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.Sensor_button);
            this.panel2.Location = new System.Drawing.Point(437, 37);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(235, 134);
            this.panel2.TabIndex = 2;
            // 
            // Sensor_button
            // 
            this.Sensor_button.Image = global::NewGUI.Properties.Resources.Senzory_mini;
            this.Sensor_button.Location = new System.Drawing.Point(-84, -50);
            this.Sensor_button.Name = "Sensor_button";
            this.Sensor_button.Size = new System.Drawing.Size(404, 212);
            this.Sensor_button.TabIndex = 1;
            this.Sensor_button.UseVisualStyleBackColor = true;
            this.Sensor_button.Click += new System.EventHandler(this.Sensor_button_Click);
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.Aktuatory_button);
            this.panel3.Location = new System.Drawing.Point(267, 194);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(235, 134);
            this.panel3.TabIndex = 3;
            // 
            // Aktuatory_button
            // 
            this.Aktuatory_button.Image = global::NewGUI.Properties.Resources.Aktuatory_mini;
            this.Aktuatory_button.Location = new System.Drawing.Point(-63, -18);
            this.Aktuatory_button.Name = "Aktuatory_button";
            this.Aktuatory_button.Size = new System.Drawing.Size(368, 169);
            this.Aktuatory_button.TabIndex = 1;
            this.Aktuatory_button.UseVisualStyleBackColor = true;
            this.Aktuatory_button.Click += new System.EventHandler(this.Aktuatory_button_Click);
            // 
            // Home
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Name = "Home";
            this.Size = new System.Drawing.Size(798, 381);
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel3.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button Simulator_button;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Button Sensor_button;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Button Aktuatory_button;
    }
}
