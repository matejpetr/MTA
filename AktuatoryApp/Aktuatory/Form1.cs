using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Text.RegularExpressions;
using System.IO.Ports;
using System.Windows.Markup;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace Aktuatory
{
    public partial class Aktuator : Form
    {
        private Timer comPortWatcherTimer; // Kontrola přitomnosti COM zařízení
        private SerialPort serialPort; // Definice způsobu komunikace 
        private DataTable excelTable; // Deklarace excel tabulky 
        private List<string> lastKnownPorts = new List<string>(); // Deklarace posledního známého portu COM
        private string BasePath = Directory.GetParent(Application.StartupPath).Parent.Parent.FullName; // cesta ke programu ve kterém se nachází, parent = vždy o složku výše
        private Timer delayedSendTimer; // Timer pro jednorázové zpožděné odeslání



        public Aktuator() // Konstruktor 
        {
            InitializeComponent();
            LoadCsvData();

            // Inicializace SerialPort 
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

            // Kontrola COM Portu 
            comPortWatcherTimer = new Timer();
            comPortWatcherTimer.Interval = 500;
            comPortWatcherTimer.Tick += ComPortWatcherTimer_Tick;
            comPortWatcherTimer.Start();

            //skrytí textboxů a labelů
            textBox1.Visible = false;
            textBox2.Visible = false;
            textBox3.Visible = false;

            label1.Visible = false;
            label2.Visible = false;
            label3.Visible = false;

            // Default pictureBox 
            string idlePath = Path.Combine(BasePath, "red_state.png");
            pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
            pictureBox1.Image = Image.FromFile(idlePath);

            // Timer pro jednorázové odložené odeslání
            delayedSendTimer = new Timer();
            delayedSendTimer.Interval = 1000; // zpoždění v milisekundách (1 s)
            delayedSendTimer.Tick += DelayedSendTimer_Tick;

            // Default checkbox
            checkBox1.Visible = false;
            checkBox1.Checked = false;

            // Default zobrazení
            SetControlButtonsEnabled(false);

        }

        private void ComPortWatcherTimer_Tick(object sender, EventArgs e)
        {
            var currentPorts = SerialPort.GetPortNames().ToList();
            if (!currentPorts.SequenceEqual(lastKnownPorts)) // podmínka pokud si nejsou rovny aktuální porty a naposledy známé
            {
                string selected = ComBox.SelectedItem as string;
                ComBox.Items.Clear();
                ComBox.Items.AddRange(currentPorts.ToArray());

                if (selected != null && currentPorts.Contains(selected))
                {
                    ComBox.SelectedItem = selected;
                }
                else if (currentPorts.Count > 0)
                {
                    ComBox.SelectedIndex = 0;
                }
                lastKnownPorts = currentPorts;
            }
        }


        private void LoadCsvData()
        {
            try
            {
                //string solutionDirectory = Directory.GetParent(Application.StartupPath).Parent.Parent.FullName; //Application.StartupPath = cesta kde se daný program nachází, GetParent = vrací o složku zpět, každý parent posune o složku zpět
                string csvPath = Path.Combine(BasePath, "MTA_Aktuátory.csv"); // cesta pro csv file, Path.Combine = sloučení cest

                if (!File.Exists(csvPath))
                {
                    MessageBox.Show($"Soubor MTA_Senzory.csv nebyl nalezen ve složce projektu: {BasePath}");
                    return;
                }

                excelTable = new DataTable();
                AktBox.Items.Clear();
                int count = 0;

                try
                {
                    using (var reader = new StreamReader(csvPath, Encoding.Default)) // Vytvoření objektu pro čtení csv souboru
                    {
                        bool headerRead = false; //Vynulování čtení 
                        string[] headers = null;

                        while (!reader.EndOfStream)
                        {
                            string line = reader.ReadLine()?.TrimEnd('\r', '\n');
                           

                            if (string.IsNullOrWhiteSpace(line)) continue; //přeskočí na konec
                            // automatická detekce oddělovače
                            string[] values;
                            if (line.Contains(';')) // pokud řádek obsahuje ";", oodstranní středník
                                values = line.Split(';');
                            else if (line.Contains('\t')) // pokud obsahuje tabulator "\t" odstranní tabulator
                                values = line.Split('\t');
                            else                          // jinak odstranní čárku ","
                                values = line.Split(',');

                            if (!headerRead)
                            {
                                headers = values.Select(h => h.Trim().Trim('"')).ToArray(); // Všechny znaky v poli values se odstranní prázdné místo a odstranní uvozovky, následně vrátí zpět do pole "headers"
                                foreach (var header in headers)
                                    excelTable.Columns.Add(header);
                                headerRead = true;
                                continue; //přeskočí na konec 
                            }
                            if (values.Length < headers.Length)
                            {
                                Array.Resize(ref values, headers.Length); //pokud headers bude mít více hodnot než values, rozšíří values
                            }
                            excelTable.Rows.Add(values);

                            string alias = values.Length > 2 ? values[2].Trim().Trim('"') : "";
                            if (!string.IsNullOrWhiteSpace(alias))
                            {
                                AktBox.Items.Add(alias);
                                count++;
                            }
                        }
                        

                    }
                }
                catch (IOException)
                {
                    throw new ApplicationException("Soubor MTA_Aktuatory.csv je otevřený v jiném programu. Zavřete ho a spusťte aplikaci znovu");
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }

        }

        private void Aktuator_Closing(object sender, FormClosedEventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Close();
            }
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (btnConnect.Text == "Připojit")
            {
                if (ComBox.SelectedItem == null)
                {
                    MessageBox.Show("Vyberte COM port.");
                    return;
                }

                try
                {
                    serialPort.PortName = ComBox.SelectedItem.ToString();
                    serialPort.Open();
                    btnConnect.Text = "Odpojit";
                    string idlePath = Path.Combine(BasePath, "orange.png");
                    pictureBox1.Image = Image.FromFile(idlePath);

                    SetControlButtonsEnabled(true);
                    label4.Text = "Připojeno";
                    //BarvaKruh = Color.Orange;
                    //pnlStatus.Invalidate();
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
                label4.Text = "Odpojeno";
                //BarvaKruh = Color.Red;
                //pnlStatus.Invalidate();
                string idlePath = Path.Combine(BasePath, "red_state.png");
                pictureBox1.Image = Image.FromFile(idlePath);

                SetControlButtonsEnabled(false);
            }
        }

        private void ShowTextBoxesForRequest(string request)
        {
            // Reset všech textboxů a labelů
            textBox1.Visible = false;
            textBox2.Visible = false;
            textBox3.Visible = false;

            label1.Visible = false;
            label2.Visible = false;
            label3.Visible = false;

            // Vytáhneme parametry za "id="
            var match = Regex.Match(request, @"\bid=[^&]*&(.+)");
            if (!match.Success)
                return;

            var paramString = match.Groups[1].Value;
            var parameters = paramString.Split('&');

            for (int i = 0; i < parameters.Length && i < 3; i++)
            {
                string[] keyValue = parameters[i].Split('=');
                if (keyValue.Length < 2) continue;

                string key = keyValue[0].Trim();

                if (i == 0)
                {
                    label1.Text = key;
                    label1.Visible = true;
                    textBox1.Visible = true;
                }
                else if (i == 1)
                {
                    label2.Text = key;
                    label2.Visible = true;
                    textBox2.Visible = true;
                }
                else if (i == 2)
                {
                    label3.Text = key;
                    label3.Visible = true;
                    textBox3.Visible = true;
                }
            }
        }





        private string UpdateRequestWithTextBoxValues(string originalRequest)
        {
            var pattern = @"(?<key>[^&=?]+)=(?<value>[^&]*)";
            var matches = Regex.Matches(originalRequest, pattern);

            var dict = new Dictionary<string, string>();
            foreach (Match match in matches)
            {
                dict[match.Groups["key"].Value] = match.Groups["value"].Value;
            }

            if (textBox1.Visible && label1.Visible)
                dict[label1.Text] = textBox1.Text;
            if (textBox2.Visible && label2.Visible)
                dict[label2.Text] = textBox2.Text;
            if (textBox3.Visible && label3.Visible)
                dict[label3.Text] = textBox3.Text;

            string basePart = originalRequest.Split('?')[0];
            string typeAndId = Regex.Match(originalRequest, @"\?[^&]+&[^&]+").Value;

            var newParams = dict
                .Where(kvp => !typeAndId.Contains($"{kvp.Key}="))
                .Select(kvp => $"{kvp.Key}={kvp.Value}");

            return basePart + typeAndId + (newParams.Any() ? "&" + string.Join("&", newParams) : "");
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            if (!serialPort.IsOpen)
            {
                MessageBox.Show("Nejste připojen k žádnému COM portu.");
                return;
            }

            string selectedMod = ModBox.SelectedItem?.ToString();
            string selectedAlias = AktBox.SelectedItem?.ToString();

            // Při RESET režimu
            if (selectedMod == "RESET")
            {
                // Zakázat textBoxy a skrytí
                textBox1.Visible = false;
                textBox2.Visible = false;
                textBox3.Visible = false;

                label1.Visible = false;
                label2.Visible = false;
                label3.Visible = false;

                // Sestavení requestu podle checkboxu
                string idPart = checkBox1.Checked ? "*" : selectedAlias;

                if (string.IsNullOrEmpty(idPart))
                {
                    MessageBox.Show("Není vybrán aktuátor.");
                    return;
                }

                string request = $"?type=RESET&id={idPart}";

                try
                {
                    serialPort.WriteLine(request);
                    MainTextBox.Clear();
                    MainTextBox.AppendText(request + Environment.NewLine);

                    string runningPath = Path.Combine(BasePath, "green.png");
                    pictureBox1.Image = Image.FromFile(runningPath);
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Chyba při odesílání: {ex.Message}");
                }

                return; // Ukončit, protože jsme provedli RESET
            }

            // Při CONFIG režimu
            if (string.IsNullOrEmpty(selectedAlias))
            {
                MessageBox.Show("Vyberte aktuátor.");
                return;
            }

            string idlePath = Path.Combine(BasePath, "green.png");
            pictureBox1.Image = Image.FromFile(idlePath);

            var row = excelTable.AsEnumerable()
                                .FirstOrDefault(r => r.Field<string>("Alias (type)") == selectedAlias);

            if (row == null)
            {
                MessageBox.Show("Alias nebyl nalezen v CSV.");
                return;
            }

            string requestOriginal = row.Field<string>("Request");
            string requestFinal = Regex.Replace(requestOriginal, @"type=[^&]+", $"type={selectedMod}");
            requestFinal = UpdateRequestWithTextBoxValues(requestFinal);

            try
            {
                serialPort.WriteLine(requestFinal);
                MainTextBox.Clear();
                MainTextBox.AppendText(requestFinal + Environment.NewLine);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Chyba při odesílání: {ex.Message}");
            }
        }

        private void DelayedSendTimer_Tick(object sender, EventArgs e)
        {
            delayedSendTimer.Stop(); // Jednorázové odeslání

            if (!serialPort.IsOpen)
                return;

            string selectedAlias = AktBox.SelectedItem?.ToString();
            string selectedMod = ModBox.SelectedItem?.ToString();

            if (string.IsNullOrEmpty(selectedAlias) || string.IsNullOrEmpty(selectedMod))
                return;

            var row = excelTable.AsEnumerable()
                                .FirstOrDefault(r => r.Field<string>("Alias¨(type)") == selectedAlias);

            if (row == null)
                return;

            string request = row.Field<string>("Request");
            request = Regex.Replace(request, @"type=[^&]+", $"type={selectedMod}");
            request = UpdateRequestWithTextBoxValues(request);

            try
            {
                serialPort.WriteLine(request);

                MainTextBox.Clear();
                MainTextBox.AppendText($"Odesláno po zpoždění: {request}{Environment.NewLine}");
            }
            catch (Exception ex)
            {
                MainTextBox.Clear();
                MainTextBox.AppendText($"Chyba při odesílání: {ex.Message}{Environment.NewLine}");
            }
        }

        private void btnStop_Click(object sender, EventArgs e)

        {
            if (delayedSendTimer.Enabled)
            {
                delayedSendTimer.Stop();
                MainTextBox.AppendText("Odeslání bylo zrušeno tlačítkem STOP.\r\n");
            }
            string idlePath = Path.Combine(BasePath, "orange.png");
            pictureBox1.Image = Image.FromFile(idlePath);
        }

        private void ModBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            string selectedMod = ModBox.SelectedItem?.ToString();

            if (selectedMod == "RESET")
            {
                checkBox1.Visible = true;

                // schovat textboxy
                textBox1.Visible = false;
                textBox2.Visible = false;
                textBox3.Visible = false;

                label1.Visible = false;
                label2.Visible = false;
                label3.Visible = false;
            }
            else
            {
                checkBox1.Visible = false;
                checkBox1.Checked = false;
                AktBox.Enabled = true;
            }
        }


        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
            {
                AktBox.Enabled = false;
            }
            else
            {
                AktBox.Enabled = true;
            }
        }

        private void AktBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (AktBox.SelectedItem == null || excelTable == null)
                return;

            string selectedAlias = AktBox.SelectedItem.ToString();

            // Oprava názvu sloupce (pokud je správně: "Alias (type)")
            var row = excelTable.AsEnumerable()
                                .FirstOrDefault(r => r.Field<string>("Alias (type)") == selectedAlias);

            if (row == null)
                return;

            string request = row.Field<string>("Request");
            if (ModBox.Text == "CONFIG")
            { 
            ShowTextBoxesForRequest(request);
            }
        }

        private void SetControlButtonsEnabled(bool enabled)
        {
            btnStart.Enabled = enabled;
            btnStop.Enabled = enabled;
            ModBox.Enabled = enabled;
            AktBox.Enabled = enabled;
            checkBox1.Enabled = enabled;
            textBox1.Enabled = enabled;
            textBox2.Enabled = enabled;
            textBox3.Enabled = enabled;
        }

    
    }
}
