using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace NewGUI
{
    public partial class Simulator : UserControl
    {

        private SerialPort serialPort;
        private Timer simulationTimer;
        private string selectedSensor = "";
        private DataTable excelTable;
        private Random random = new Random();
        private Timer comPortWatcherTimer;
        private List<string> lastKnownPorts = new List<string>();
        private bool simulationRunning = false;



        public Simulator(Form1 rodic)
        {

            InitializeComponent();
            LoadCsvData();

            // Inicializace SerialPort (zpočátku žádný COM port)
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

            // Timer pro simulaci
            simulationTimer = new Timer();
            simulationTimer.Interval = 1000; // 1 sekunda
            simulationTimer.Tick += SimulationTimer_Tick;

            // Inicializace UI prvků
            Load += Form1_Load;

            // Kontrola COM portu
            comPortWatcherTimer = new Timer();
            comPortWatcherTimer.Interval = 500; // kontrola každých 500 ms
            comPortWatcherTimer.Tick += ComPortWatcherTimer_Tick;
            comPortWatcherTimer.Start();

        }


        private void Form1_Load(object sender, EventArgs e)
        {
            // Vyčistí předchozí položky
            comBox.Items.Clear();

            // Zjistí dostupné COM porty a přidá do ComboBoxu
            string[] ports = SerialPort.GetPortNames();
            foreach (string port in ports)
            {
                comBox.Items.Add(port);
            }

            if (comBox.Items.Count > 0)
                comBox.SelectedIndex = 0;

        }

        private void LoadCsvData()
        {
            try
            {
                string solutionDirectory = Directory.GetParent(Application.StartupPath).Parent.Parent.FullName;
                string csvPath = Path.Combine(solutionDirectory, "MTA_Senzory.csv");

                if (!File.Exists(csvPath))
                {
                    MessageBox.Show($"Soubor MTA_Senzory.csv nebyl nalezen ve složce projektu: {solutionDirectory}");
                    return;
                }

                excelTable = new DataTable();
                sensorBox.Items.Clear();
                int count = 0;

                try
                {
                    using (var reader = new StreamReader(csvPath, Encoding.Default))

                    //using (var stream = new FileStream(csvPath, FileMode.Open, FileAccess.Read, FileShare.None))
                    {
                        bool headerRead = false;
                        string[] headers = null;

                        while (!reader.EndOfStream)
                        {
                            string line = reader.ReadLine();
                            if (string.IsNullOrWhiteSpace(line)) continue;

                            // automatická detekce oddělovače
                            string[] values;
                            if (line.Contains(';'))
                                values = line.Split(';');
                            else if (line.Contains('\t'))
                                values = line.Split('\t');
                            else
                                values = line.Split(',');

                            if (!headerRead)
                            {
                                headers = values.Select(h => h.Trim().Trim('"')).ToArray();
                                foreach (var header in headers)
                                    excelTable.Columns.Add(header);
                                headerRead = true;
                                continue;
                            }

                            // Pokud řádek má méně hodnot než header, doplníme prázdné
                            if (values.Length < headers.Length)
                            {
                                Array.Resize(ref values, headers.Length);
                            }

                            excelTable.Rows.Add(values);

                            // Naplnění ComboBoxu
                            string alias = values.Length > 2 ? values[2].Trim().Trim('"') : "";
                            if (!string.IsNullOrWhiteSpace(alias))
                            {
                                sensorBox.Items.Add(alias);
                                count++;
                            }
                        }
                    }
                }
                catch (IOException)
                {
                    throw new ApplicationException("Soubor MTA_Senzory.csv je otevřený v jiném programu. Zavřete ho a spusťte aplikaci znovu.");

                }

                //MessageBox.Show($"Načteno senzorů: {count}");
            }
            catch (Exception ex)
            {
                //MessageBox.Show($"Chyba při načítání CSV: {ex.Message}");
                throw;

            }
        }
        private DataRow NajdiSensor(string sensorAlias)
        {
            return excelTable.AsEnumerable()
                .FirstOrDefault(row => row["Alias (type)"].ToString() == sensorAlias);
        }

        private string VygenerujHodnotu(string type)
        {
            if (type.Contains("bool"))
            {
                return random.Next(0, 2).ToString();
            }
            else if (type.Contains("float"))
            {
                double value = 20 + random.NextDouble() * 10;
                return value.ToString("F2");
            }
            else if (type.Contains("int"))
            {
                int value = random.Next(0, 100);
                return value.ToString();
            }
            else if (type.Contains("string"))
            {
                // fallback pro polarity nebo direction
                if (type.Contains("polarity"))
                {
                    string[] options = { "North", "South", "East", "West" };
                    return options[random.Next(options.Length)];
                }
                else if (type.Contains("direction"))
                {
                    string[] options = { "Up", "Down", "Left", "Right" };
                    return options[random.Next(options.Length)];
                }
                else
                {
                    string[] defaultOptions = { "ON", "OFF" };
                    return defaultOptions[random.Next(defaultOptions.Length)];
                }
            }
            return "N/A";
        }


        private string VytvorResponse(string sensorAlias)
        {
            DataRow sensorRow = NajdiSensor(sensorAlias);
            if (sensorRow == null) return "";

            string responseTemplate = sensorRow["Response"].ToString();
            string keywords = sensorRow["Keywords - values"].ToString();

            if (string.IsNullOrEmpty(responseTemplate) || string.IsNullOrEmpty(keywords))
                return "";

            string result = responseTemplate;

            // Připrav slovník key:type z Keywords
            Dictionary<string, string> keyTypeMap = new Dictionary<string, string>();
            string[] keywordPairs = keywords.Split(',');
            foreach (var pair in keywordPairs)
            {
                string[] parts = pair.Split(':');
                if (parts.Length == 2)
                {
                    string key = parts[0].Trim();
                    string type = pair.Trim();
                    keyTypeMap[key] = type;
                }
            }

            // Všechny parametry v Response
            var matches = Regex.Matches(responseTemplate, @"(\w+)=([^&]*)");
            foreach (Match match in matches)
            {
                string keyInResponse = match.Groups[1].Value; // např. temp
                if (keyTypeMap.TryGetValue(keyInResponse, out string type))
                {
                    string newValue = VygenerujHodnotu(type);

                    string pattern = $@"({keyInResponse}=)[^&]*";
                    result = Regex.Replace(result, pattern, m => m.Groups[1].Value + newValue);

                }
            }

            return result;
        }

        private void SimulationTimer_Tick(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(selectedSensor))
                return;

            // Vytvoř dynamický Response
            string responseToSend = VytvorResponse(selectedSensor);

            // Vypíšeme do textBoxu (každý řádek)
            textBox.AppendText(responseToSend + Environment.NewLine);

            if (serialPort.IsOpen)
            {
                try
                {
                    serialPort.WriteLine(responseToSend);
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Chyba při odesílání dat: {ex.Message}");
                }
            }
        }


        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Close();
            }
        }

        private void ComPortWatcherTimer_Tick(object sender, EventArgs e)
        {
            var currentPorts = SerialPort.GetPortNames().ToList();

            if (!currentPorts.SequenceEqual(lastKnownPorts))
            {
                // Změna — aktualizuj ComboBox
                string selected = comBox.SelectedItem as string;

                comBox.Items.Clear();
                comBox.Items.AddRange(currentPorts.ToArray());

                if (selected != null && currentPorts.Contains(selected))
                {
                    comBox.SelectedItem = selected;
                }
                else if (currentPorts.Count > 0)
                {
                    comBox.SelectedIndex = 0;
                }

                lastKnownPorts = currentPorts;
            }
        }


        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (btnConnect.Text == "Připojit")
            {
                if (comBox.SelectedItem == null)
                {
                    MessageBox.Show("Vyberte COM port.");
                    return;
                }

                try
                {
                    serialPort.PortName = comBox.SelectedItem.ToString();
                    serialPort.Open();
                    btnConnect.Text = "Odpojit";
                    comBox.Enabled = false;
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Chyba při připojení: {ex.Message}");
                }

            }
            else
            {
                if (serialPort.IsOpen)
                {
                    serialPort.Close();
                }
                btnConnect.Text = "Připojit";
                comBox.Enabled = true;
            }
        }
        private void btnStartStop_Click(object sender, EventArgs e)
        {

            if (!simulationRunning)
            {
                if (!serialPort.IsOpen)
                {
                    MessageBox.Show("Nejprve připojte zařízení.");
                    return;
                }
                if (sensorBox.SelectedItem == null)
                {
                    MessageBox.Show("Vyberte senzor.");
                    return;
                }

                selectedSensor = sensorBox.SelectedItem.ToString();
                simulationTimer.Start();
                simulationRunning = true;
                btnStartStop.Text = ("Stopnout Simulaci");
            }
            else
            {
                simulationRunning = false;
                simulationTimer.Stop();
                btnStartStop.Text = ("Spustit Simulaci");
                selectedSensor = "";
                textBox.Text += "Simulace byla zastavena";

            }
        }

    }
}
