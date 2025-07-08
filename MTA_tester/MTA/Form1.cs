using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Windows.Forms.DataVisualization.Charting;
using System.Diagnostics;
using System.Text.RegularExpressions;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace GUI
{
    public partial class Form1 : Form
    {
        private Stopwatch _stopwatch = new Stopwatch();
        private SerialPort serialPort;
        private bool isSendingRequest = false;
        public string request;
        private int sampleCount = 0;

        public Form1()
        {
            InitializeComponent();
            InitializeChart();
            serialPort = new SerialPort("COM6", 115200, Parity.None, 8, StopBits.One)
            {
                NewLine = "\n",
                ReadTimeout = 10,
                WriteTimeout = 10
            };
            serialPort.DataReceived += SerialPort_DataReceived;

        }
        private void InitializeChart()
        {
            chart1.Series.Clear();
            Series series = new Series("TemperatureSeries")
            {
                ChartType = SeriesChartType.Line,  // Typ grafu: čárový
                XValueType = ChartValueType.Int32,  // Osu X budeme interpretovat jako číslo (index vzorku)
                YValueType = ChartValueType.Double  // Osu Y budeme interpretovat jako číslo (teplota)
            };
            chart1.Series.Add(series);

            // Nastavení os
            chart1.ChartAreas[0].AxisX.Title = "Počet vzorků";
            chart1.ChartAreas[0].AxisY.Title = "Teplota (°C)";
            chart1.ChartAreas[0].AxisY.Maximum = 44;
            chart1.ChartAreas[0].AxisX.Maximum = 999999;
            chart1.Series["TemperatureSeries"].BorderWidth = 2;
            chart1.Series["TemperatureSeries"].Color = Color.Black;

            chart1.ChartAreas[0].AxisX.MajorGrid.LineWidth = 0;  // Nastaví tloušťku čáry mřížky na 2
            chart1.ChartAreas[0].AxisY.MajorGrid.LineWidth = 0;  // Nastaví tloušťku čáry mřížky na 2

        }
        private void buttonStart_Click(object sender, EventArgs e)
        {
            if (!serialPort.IsOpen)
            {
                try
                {
                    serialPort.Open();
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Chyba při otevírání portu: {ex.Message}");
                    return;
                }
            }
            request = "?type=" + comboBox2.Text + "&id=" + comboBox1.Text;

            textBox1.Text = request;
            StartSendingRequest();
        }

        private async void buttonStop_Click(object sender, EventArgs e)
        {
            StopSendingRequest();
            await Task.Delay(100);
            textBox2.AppendText("Měření pozastaveno.\r\n");
        }

        private void StartSendingRequest()
        {
            isSendingRequest = true;
            SendRequest();
        }

        private void StopSendingRequest()
        {
            isSendingRequest = false;
        }

        private async void SendRequest()
        {
            if (!serialPort.IsOpen || !isSendingRequest)
                return;

            string dlyText = "";

            if (comboBox2.InvokeRequired)
            {
                comboBox2.Invoke(new Action(() =>
                {
                    dlyText = comboBox2.Text;
                }));
            }
            else
            {
                dlyText = comboBox2.Text;
            }

            if (int.TryParse(dlyText, out int delay))
            {
                await Task.Delay(delay);
            }
            else
            {
                await Task.Delay(100); // fallback
            }

            serialPort.WriteLine(request);
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                string data = serialPort.ReadExisting();

                if (!string.IsNullOrEmpty(data) && data.StartsWith("?type="))
                {
                    this.Invoke((MethodInvoker)delegate
                    {
                        ParseAndDisplayData(data);
                    });

                    if (isSendingRequest)
                    {
                        SendRequest();
                    }
                }
            }
            catch
            {
                // Ignorujeme chyby
            }
        }

        private void ProcessIncomingData(string data)
        {
            if (data.StartsWith("?type="))
            {

                if (isSendingRequest)
                {
                    SendRequest();
                }
            }
        }



        private void ParseAndDisplayData(string data)
        {
            data = data.Trim();

            if (data.StartsWith("?"))
                data = data.Substring(1);

            var parameters = data.Split('&')
                                 .Select(part => part.Split('='))
                                 .Where(pair => pair.Length == 2)
                                 .ToDictionary(pair => pair[0], pair => pair[1]);

            var dataForGraph = parameters
                                 .Where(kvp => kvp.Key != "type" && kvp.Key != "id")
                                 .ToDictionary(kvp => kvp.Key, kvp => kvp.Value);

            foreach (var kvp in dataForGraph)
            {
                string variableName = kvp.Key;
                string stringValue = kvp.Value;

                if (double.TryParse(stringValue, System.Globalization.NumberStyles.Any, System.Globalization.CultureInfo.InvariantCulture, out double numericValue))
                {
                    this.Invoke(new Action(() =>
                    {
                        // Kontrola, jestli série už existuje, pokud ne, vytvoříme ji.
                        if (!chart1.Series.IsUniqueName(variableName))
                        {
                            // Série už existuje — nic
                        }
                        else
                        {
                            Series newSeries = new Series(variableName)
                            {
                                ChartType = SeriesChartType.Line,
                                BorderWidth = 2
                            };
                            Random rnd = new Random();
                            newSeries.Color = Color.FromArgb(rnd.Next(256), rnd.Next(256), rnd.Next(256)); // náhodná barva
                            chart1.Series.Add(newSeries);
                        }

                        if (chart1.Series[variableName].Points.Count > 50)
                        {
                            chart1.Series[variableName].Points.RemoveAt(0);
                        }

                        chart1.Series[variableName].Points.AddXY(sampleCount, numericValue);
                        chart1.ChartAreas[0].AxisX.Minimum = Math.Max(0, sampleCount - 10);
                        chart1.ChartAreas[0].AxisX.Maximum = sampleCount;
                    }));

                    AppendTextBox($"{variableName} = {numericValue} \r\n");
                }
                else
                {
                    AppendTextBox($"Nečíselná hodnota v {variableName}: {stringValue}\r\n");
                }
            }

            sampleCount++;
        }

        private void AppendTextBox(string text)
        {
            if (textBox2.InvokeRequired)
            {
                textBox2.Invoke(new Action(() =>
                {
                    textBox2.AppendText(text);
                }));
            }
            else
            {
                textBox2.AppendText(text);
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Close();
            }
        }

        
    }
}