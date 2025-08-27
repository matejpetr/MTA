﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ListView;

namespace NewGUI
{
    public partial class Home : UserControl
    {
        private Form1 _rodic;

        public Home(Form1 rodic)
        {
            InitializeComponent();
            _rodic = rodic;

            // Předpoklad: máš tři host panely (A … B … C) v Designeru:
            // Aktuatory_panel, Sensor_panel, Simulator_panel
            // Vyčisti je a vlož HoverTile s Dock=Fill
            AddTile(Aktuatory_panel,
                title: "Aktuátory",
                normal: Properties.Resources.half_brain_mini3,
                hover:  Properties.Resources.half_brain_mini4,   // můžeš dát přebarvenou verzi
                detail: "Ovládání aktivních prvků přes seriovou komunikaci",
                onActivate: (_, __) => _rodic.NahraditObsah(new Aktuatory(_rodic)));

            AddTile(Sensor_panel,
                title: "Senzory",
                normal: Properties.Resources.half_brain_mini3,
                hover:  Properties.Resources.half_brain_mini4,
                detail: "Měření pomocí senzorů přes seriovou komunikaci",
                onActivate: (_, __) => _rodic.NahraditObsah(new Senzory(_rodic)));

            AddTile(Simulator_panel,
                title: "Simulator",
                normal: Properties.Resources.half_brain_mini3,
                hover:  Properties.Resources.half_brain_mini4,
                detail: "Virtuální prostředí pro testování senzorů a aktuátorů.",
                onActivate: (_, __) => _rodic.NahraditObsah(new Simulator(_rodic)));
        }

        private static void AddTile(Panel host, string title, Image normal, Image hover, string detail, EventHandler onActivate)
        {
            host.Controls.Clear();

            var tile = new Tlacitka
            {
                Dock = DockStyle.Fill,
                Title = title,
                NormalImage = normal,
                HoverImage = hover,
                DetailText = detail,
                ExpandedHeight = 150 // výšku můžeš doladit
            };
            tile.Activated += onActivate;

            host.Controls.Add(tile);
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
