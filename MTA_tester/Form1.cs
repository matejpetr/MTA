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
    public partial class MTA : Form
    {
        // Inicializace pomocných proměnných
        private SerialPort serialPort;                      // Objekt pro sériovou komunikaci
        private bool isSendingRequest = false;              // Flag pro řízení posílání požadavků
        public string request;                              // Text aktuálního požadavku
        private int sampleCount = 0;                        // Počet načtených vzorků
        private string lastUsedID = null;                   // ID posledně použitého zařízení
        private Random rnd = new Random();                  // Generátor náhodných barev
        private Timer comPortWatcherTimer;
        private List<string> lastKnownPorts = new List<string>();
        public MTA()
        {
            InitializeComponent();
            InitializeChart(); // Inicializace grafu

            // Nastavení sériového portu (výchozí hodnota)
            serialPort = new SerialPort()
            {
                BaudRate = 115200,
                Parity = Parity.None,
                DataBits = 8,
                StopBits = StopBits.One,
                NewLine = "\n",
                ReadTimeout = 10,
                WriteTimeout = 10
            };
            //serialPort.DataReceived += SerialPort_DataReceived;

            // Výchozí hodnoty pro comboBoxy
            comboBoxTIMER.SelectedIndex = 1;
            comboBox2.SelectedIndex = 0;

            comPortWatcherTimer = new Timer();
            comPortWatcherTimer.Interval = 500; // kontrola každých 500 ms
            comPortWatcherTimer.Tick += ComPortWatcherTimer_Tick;
            comPortWatcherTimer.Start();

        }


        // Nastaví výchozí podobu grafu.
        private void InitializeChart()
        {
            chart1.Series.Clear();
            //MTA ! MTA! MTA! MTA! MTA! MTA!  

            // Přidání výchozí série (nepoužívá se přímo, ale inicializuje graf)
            Series series = new Series("measuring")
            {
                ChartType = SeriesChartType.Line,
                XValueType = ChartValueType.Int32,
                YValueType = ChartValueType.Double,
                IsVisibleInLegend = false
            };
            chart1.Series.Add(series);

            // Vzhled osy X
            chart1.ChartAreas[0].AxisX.Title = "Počet vzorků";
            chart1.ChartAreas[0].AxisX.LineWidth = 2;
            chart1.ChartAreas[0].AxisX.LineColor = Color.Black;
            chart1.ChartAreas[0].AxisX.MajorGrid.LineColor = Color.LightGray;
            chart1.ChartAreas[0].AxisX.MajorGrid.LineWidth = 1;

            // Vzhled osy Y
            chart1.ChartAreas[0].AxisY.LineWidth = 2;
            chart1.ChartAreas[0].AxisY.LineColor = Color.Black;
            chart1.ChartAreas[0].AxisY.MajorGrid.LineColor = Color.LightGray;
            chart1.ChartAreas[0].AxisY.MajorGrid.LineWidth = 1;

            chart1.Series["measuring"].BorderWidth = 2;
            chart1.Series["measuring"].Color = Color.Black;
        }

        // Tlačítko START - spustí komunikaci s Test-standem a grafování.

        private void ComPortWatcherTimer_Tick(object sender, EventArgs e)
        {
            var currentPorts = SerialPort.GetPortNames().ToList();

            if (!currentPorts.SequenceEqual(lastKnownPorts))
            {
                // Změna — aktualizuj ComboBoxCOM
                string selected = comboBoxCOM.SelectedItem as string;

                comboBoxCOM.Items.Clear();
                comboBoxCOM.Items.AddRange(currentPorts.ToArray());

                if (selected != null && currentPorts.Contains(selected))
                {
                    comboBoxCOM.SelectedItem = selected;
                }
                else if (currentPorts.Count > 0)
                {
                    comboBoxCOM.SelectedIndex = 0;
                }

                lastKnownPorts = currentPorts;
            }
        }



        private void buttonStart_Click(object sender, EventArgs e)
        {
            string selectedPort = comboBoxCOM.Text?.Trim();
            string currentID = comboBox1.Text?.Trim();
            string currentType = comboBox2.Text?.Trim();

            // Ověření výběru COM portu
            if (string.IsNullOrWhiteSpace(selectedPort))
            {
                MessageBox.Show("Prosím vyber COM port.");
                return;
            }

            // Kontrola vybraného typu
            if (string.IsNullOrWhiteSpace(currentType))
            {
                MessageBox.Show("Prosím vyber typ měření.");
                return;
            }

            // Sestavení dotazu podle typu
            if (currentType.Equals("INIT", StringComparison.OrdinalIgnoreCase))
            {
                request = "?type=" + currentType;
            }
            else
            {
                // Ověření ID jen pokud nejde o INIT
                if (string.IsNullOrWhiteSpace(currentID))
                {
                    MessageBox.Show("Prosím zadej nebo vyber ID zařízení.");
                    return;
                }

                request = "?type=" + currentType + "&id=" + currentID;
            }


            if (string.IsNullOrWhiteSpace(currentType))
            {
                MessageBox.Show("Prosím vyber typ měření.");
                return;
            }

            // Pokus o otevření portu
            if (!serialPort.IsOpen)
            {
                try
                {
                    serialPort = new SerialPort(selectedPort, 115200, Parity.None, 8, StopBits.One)
                    {
                        NewLine = "\n",
                        ReadTimeout = 10,
                        WriteTimeout = 10
                    };
                    serialPort.DataReceived += SerialPort_DataReceived;
                    serialPort.Open();
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Chyba při otevírání portu: {ex.Message}");
                    return;
                }
            }

            // Reset grafu při změně ID
            if (currentID != lastUsedID)
            {
                ResetChart();
                lastUsedID = currentID;
            }

            // Vytvoření požadavku

            textBox1.Text = request;

            // Spuštění komunikace
            StartSendingRequest();

            // Zablokování vstupů
            comboBox1.Enabled = false;
            comboBox2.Enabled = false;
            comboBoxCOM.Enabled = false;
            comboBoxTIMER.Enabled = false;
        }



        // Tlačítko STOP - ukončí měření a odemkne prvky GUI.

        private async void buttonStop_Click(object sender, EventArgs e)
        {
            comboBox1.Enabled = true;
            comboBox2.Enabled = true;
            comboBoxCOM.Enabled = true;
            comboBoxTIMER.Enabled = true;

            StopSendingRequest();
            await Task.Delay(100);
            textBox2.AppendText("Měření pozastaveno.\r\n");
        }

        private void StartSendingRequest()
        {
            // Zjisti, zda se má jednat o cyklické posílání
            if (request.StartsWith("?type=update", StringComparison.OrdinalIgnoreCase))
            {
                isSendingRequest = true;
                SendRequest(); // spustí cyklus
            }
            else
            {
            

                if (serialPort != null && serialPort.IsOpen)
                {
                    serialPort.WriteLine(request);
                }
                else
                {
                    AppendTextBox("Port není otevřen – požadavek se neodešle.\r\n");
                }
                isSendingRequest = true;
                SendRequest();
                    // vypne cyklus

            }
        }



        private void StopSendingRequest()
        {
            isSendingRequest = false;
        }


        // Odeslání požadavku zařízení s definovaným zpožděním.

        private async void SendRequest()
        {
            if (!serialPort.IsOpen || !isSendingRequest)
                return;

            string dlyText = "";

            // Vláknově bezpečné čtení z GUI
            if (comboBoxTIMER.InvokeRequired)
            {
                comboBoxTIMER.Invoke(new Action(() =>
                {
                    dlyText = comboBoxTIMER.Text;
                }));
            }
            else
            {
                dlyText = comboBoxTIMER.Text;
            }

            if (int.TryParse(dlyText, out int delay))
            {
                await Task.Delay(delay);
            }
            else
            {
                AppendTextBox("Chybná hodnota zpoždění. Používá se výchozí zpoždění 100 ms.\r\n");
                await Task.Delay(100); // Výchozí hodnota
            }

            serialPort.WriteLine(request);
        }



        // Událost spuštěná při příjmu dat ze sériového portu.

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
                else //if (comboBox2.Text.Equals("INIT", StringComparison.OrdinalIgnoreCase))
                {
                    this.Invoke((MethodInvoker)delegate
                    {
                        ParseInitMessage(data);
                    });
                    
                }
            }
            catch
            {

            }
        }


        // Zpracuje přijatá data a zobrazí je v grafu a textovém výstupu.

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

            List<string> valueTexts = new List<string>();

            foreach (var kvp in dataForGraph)
            {
                string variableName = kvp.Key;
                string stringValue = kvp.Value;

                if (double.TryParse(stringValue, System.Globalization.NumberStyles.Any, System.Globalization.CultureInfo.InvariantCulture, out double numericValue))
                {
                    this.Invoke(new Action(() =>
                    {
                        // Vytvoření nové série, pokud ještě neexistuje
                        if (!chart1.Series.IsUniqueName(variableName))
                        {
                            // Série již existuje
                        }
                        else
                        {
                            Series newSeries = new Series(variableName)
                            {
                                ChartType = SeriesChartType.Line,
                                BorderWidth = 2,
                                Color = Color.FromArgb(rnd.Next(256), rnd.Next(256), rnd.Next(256))
                            };
                            chart1.Series.Add(newSeries);
                        }

                        // Udržení limitu na 50 bodů na sérii
                        if (chart1.Series[variableName].Points.Count > 50)
                        {
                            chart1.Series[variableName].Points.RemoveAt(0);
                        }

                        chart1.Series[variableName].Points.AddXY(sampleCount, numericValue);

                        // Dynamické posouvání X osy
                        chart1.ChartAreas[0].AxisX.Minimum = Math.Max(0, sampleCount - 10);
                        chart1.ChartAreas[0].AxisX.Maximum = sampleCount;
                        chart1.ChartAreas[0].RecalculateAxesScale();

                        // Popis osy Y dle počtu měřených veličin
                        chart1.ChartAreas[0].AxisY.Title = dataForGraph.Count > 1 ? "Values" : variableName.ToUpper();
                    }));

                    valueTexts.Add($"{variableName} = {numericValue}");
                }
                else
                {
                    valueTexts.Add($"{variableName}: {stringValue}");
                }
            }

            if (valueTexts.Count > 0)
            {
                AppendTextBox(string.Join(", ", valueTexts) + "\r\n");
                sampleCount++;
            }
        }


        private void ParseInitMessage(string data)
        {
            string rawData = data.Trim();

            if (rawData.StartsWith("?"))
            {
                rawData = rawData.Substring(1); // odstraní '?'
            }

            string[] sensorEntries = rawData.Split(',');

            StringBuilder result = new StringBuilder();

            foreach (string entry in sensorEntries)
            {
                string[] parts = entry.Split(':');
                if (parts.Length == 2)
                {
                    string id = parts[0];
                    string type = parts[1];
                    result.AppendLine($"{type} ({id})");
                }
            }

            // Výstup do TextBoxu:
            AktivBox.Text = result.ToString();


        }

        /// Vlákna-bezpečný zápis do textového pole.

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
        /// Resetuje graf pro nové ID.
       
        private void ResetChart()
        {
            sampleCount = 0;
            chart1.Series.Clear();
            InitializeChart();
        }


        /// Ukončení programu – uzavření portu.

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Close();
            }
        }

    }
}
