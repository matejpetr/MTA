using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace NewGUI
{
    public partial class Home : UserControl
    {
        private Form1 _rodic;
        public Home(Form1 rodic)
        {
            InitializeComponent();
            _rodic = rodic; 
        }

        private void Simulator_button_Click(object sender, EventArgs e)
        {
            _rodic.NahraditObsah(new Simulator(_rodic));
        }

        private void Sensor_button_Click(object sender, EventArgs e)
        {
            _rodic.NahraditObsah(new Senzory(_rodic));
        }

        private void Aktuatory_button_Click(object sender, EventArgs e)
        {
            _rodic.NahraditObsah(new Aktuatory(_rodic));
        }
    }
}
