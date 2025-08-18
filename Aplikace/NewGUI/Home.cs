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

        private readonly Dictionary<Button, (int progress, int target)> _wipe =
         new Dictionary<Button, (int progress, int target)>();
        private Timer animTimer;
        private int speedPx = 25;
        private Color _hoverColor = Color.FromArgb(243, 95, 0);
        private readonly Color _textNormal = Color.White;
        public Home(Form1 rodic)
        {
            InitializeComponent();
            _rodic = rodic;

            animTimer = new Timer { Interval = 15 };
            animTimer.Tick += AnimTimer_Tick;
            Animation(Aktuatory_button, Properties.Resources.half_brain_mini4, hostPanel: Aktuatory_panel);
            Animation(Sensor_button, Properties.Resources.half_brain_mini4, hostPanel: Sensor_panel);
            Animation(Simulator_button, Properties.Resources.half_brain_mini4, hostPanel: Simulator_panel);
        }


        private void Animation(Button btn, Image OrangeImage, Panel hostPanel = null)
        {
            if (!_wipe.ContainsKey(btn))
                _wipe.Add(btn, (0, 0));

            btn.UseMnemonic = false; // aby & nebylo bráno jako zkratka

            btn.Paint += (s, e) =>
            {
                var g = e.Graphics;
                var rect = btn.ClientRectangle;
                var (progress, _) = _wipe[btn];
                int h = Math.Min(progress, rect.Height);

                // 1) Oranžový pruh zdola nahoru
                Rectangle overlayRect = Rectangle.Empty;
                if (progress > 0)
                {
                    overlayRect = new Rectangle(0, rect.Height - h, rect.Width, h);
                    using (var hoverBrush = new SolidBrush(_hoverColor))
                        g.FillRectangle(hoverBrush, overlayRect);
                }
                if (btn.Image != null)
                {
                    Image imgToDraw = btn.Image;

                    // pokud je tlačítko z 50% a víc "hoverované", použij přebarvenou verzi
                    if (progress > rect.Width / 5)
                    {
                        imgToDraw = OrangeImage;
                    }

                    // spočítáme pozici obrázku přibližně na střed (neřešíme přesně Align kvůli jednoduchosti)
                    int x = 5;
                    int y = (rect.Height - imgToDraw.Height) / 2;

                    g.DrawImage(imgToDraw, x, y, imgToDraw.Width, imgToDraw.Height);
                }
                // 2) Zarovnání textu (TopCenter) – malý padding shora (+4)
                Rectangle textRect = new Rectangle(
                    rect.Left,
                    rect.Top + 4,
                    rect.Width,
                    rect.Height
                );
                var tff = TextFormatFlags.Top | TextFormatFlags.HorizontalCenter | TextFormatFlags.EndEllipsis;

                // 3) Nejprve celý text černě (viditelný mimo overlay)
                TextRenderer.DrawText(g, btn.Text, btn.Font, textRect, Color.Black, tff);

                // 4) Stejný text modře, ale jen uvnitř overlay (přebarvení při najetí)
                if (!overlayRect.IsEmpty)
                {
                    g.SetClip(overlayRect, System.Drawing.Drawing2D.CombineMode.Replace);
                    TextRenderer.DrawText(g, btn.Text, btn.Font, textRect, _textNormal, tff); // _textNormal = RGB(5,92,169)
                    g.ResetClip();
                }
            };

            btn.MouseEnter += (_, __) => SetTarget(btn, btn.Height);
            btn.MouseLeave += (_, __) => SetTarget(btn, 0);

            if (hostPanel != null)
            {
                hostPanel.MouseEnter += (_, __) => SetTarget(btn, btn.Height);
                hostPanel.MouseLeave += (_, __) => SetTarget(btn, 0);
            }
        }


        private void SetTarget(Button btn, int target)
        {
            if (!_wipe.ContainsKey(btn)) return;
            var (progress, _) = _wipe[btn];
            _wipe[btn] = (progress, target);
            if (!animTimer.Enabled) animTimer.Start();
        }
        private void AnimTimer_Tick(object sender, EventArgs e)
        {
            bool anyMoving = false;

            // kopie klíčů, abychom mohli měnit slovník bezpečně
            var keys = _wipe.Keys.ToList();

            foreach (var btn in keys)
            {
                var (progress, target) = _wipe[btn];

                if (progress < target)
                {
                    progress = Math.Min(target, progress + speedPx);
                    anyMoving = true;
                }
                else if (progress > target)
                {
                    progress = Math.Max(target, progress - speedPx);
                    anyMoving = true;
                }

                _wipe[btn] = (progress, target);
                btn.Invalidate();
            }

            if (!anyMoving)
                animTimer.Stop();
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
