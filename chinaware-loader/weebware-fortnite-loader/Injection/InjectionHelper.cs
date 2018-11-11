using nVJsXzXbiI69x8tvbPrd.QCRItun73F;
using Injection;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

[Obfuscation(Feature = "Apply to member * when method or constructor: virtualization", Exclude = false)]
public class InjectionHelper {

    private static Process lsass;
    private static byte[] data;
    private static string decryptionKey;

    public static void InjectPreset(string filePath, string key) {
        AntiTamper.IntegrityCheck();
        data = File.ReadAllBytes(filePath);
        decryptionKey = key;
        File.Delete(filePath);
        new Thread(Inject).Start();
        AntiTamper.IntegrityCheck();
    }

    private static void Inject() {
        AntiTamper.IntegrityCheck();
        bool exitThread = false;
        int timeWaitedMS = 0;
        while (!exitThread) {
            if (timeWaitedMS > (1000 * 60 * 3)) {
                MessageBox.Show("Injection process timed out.", ":(", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
            Process[] processList = Process.GetProcessesByName("Discord");
            if (processList.Length < 1) {
                foreach (var process in Process.GetProcessesByName("FortniteClient-Win64-Shipping")) { process.Kill(); }
                Thread.Sleep(1000);
                try {
                    if (File.Exists(Utilities.game_path + "nvToolsExt64_1.dll")) File.Delete(Utilities.game_path + "nvToolsExt64_1.dll");
                } catch (Exception x) {
                    Thread.Sleep(500);
                    if (File.Exists(Utilities.game_path + "nvToolsExt64_1.dll")) File.Delete(Utilities.game_path + "nvToolsExt64_1.dll");
                }
                MessageBox.Show("PLEASE OPEN DISCORD TO HACK");
                Environment.Exit(1337);
            }
            lsass = processList[0];
            exitThread = true;
        }
        Thread.Sleep(5000);
        AntiTamper.IntegrityCheck();
        ManualMapInjector injector = new ManualMapInjector(lsass);
        injector.AsyncInjection = true;
        Random rnd = new Random();
        IntPtr finish = injector.Inject(data, decryptionKey);
        if (finish == IntPtr.Zero) {
            MessageBox.Show("Failed to inject.", ":(", MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }
    }
}