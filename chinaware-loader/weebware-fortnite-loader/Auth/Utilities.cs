using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Management;
using System.Reflection;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;


class Utilities {

    public static string game_path = @"C:\Program Files\Epic Games\Fortnite\Engine\Binaries\ThirdParty\PhysX3\Win64\VS2015\";

    private static string cpuID() {
        string cpuInfo = "";
        ManagementClass managClass = new ManagementClass("win32_processor");

        ManagementObjectCollection managCollec = managClass.GetInstances();

        foreach (ManagementObject managObj in managCollec) {
            cpuInfo = managObj.Properties["Revision"].Value.ToString() + managObj.Properties["processorID"].Value.ToString();
            break;
        }
        return cpuInfo;
    }

    [Obfuscation(Feature = "inline", Exclude = false)]
    public static void cleanup() {
        foreach (var process in Process.GetProcessesByName("FortniteClient-Win64-Shipping")) { process.Kill(); }
        Thread.Sleep(1000);
        try {
            if (File.Exists(game_path)) File.Delete(game_path + "nvToolsExt64_1.dll");
        } catch (Exception x) {
            Thread.Sleep(500);
            if (File.Exists(game_path)) File.Delete(game_path + "nvToolsExt64_1.dll");
        }
    }

    public static string getHWID() {
        string original;
        string to_build = "";
        try {
            original = cpuID() + Environment.MachineName;
        } catch (Exception) {
            original = Environment.MachineName;
        }
        for (int i = 0; i < original.Length; i++) {
            to_build += (int)original[i];
        }
        return to_build;
    }

    public static byte[] decryptToBytes(string filePath, string password) {
        try {
            byte[] passwordBytes = System.Text.Encoding.UTF8.GetBytes(password);
            byte[] salt = new byte[32];
            FileStream fsCrypt = new FileStream(filePath, FileMode.Open);
            fsCrypt.Read(salt, 0, salt.Length);
            RijndaelManaged AES = new RijndaelManaged();
            AES.KeySize = 256;
            AES.BlockSize = 128;
            var key = new Rfc2898DeriveBytes(passwordBytes, salt, 50000);
            AES.Key = key.GetBytes(AES.KeySize / 8);
            AES.IV = key.GetBytes(AES.BlockSize / 8);
            AES.Padding = PaddingMode.PKCS7;
            AES.Mode = CipherMode.CFB;

            CryptoStream cs = new CryptoStream(fsCrypt, AES.CreateDecryptor(), CryptoStreamMode.Read);
            MemoryStream ms = new MemoryStream();
            int read;
            byte[] buffer = new byte[1048576];
            while ((read = cs.Read(buffer, 0, buffer.Length)) > 0) ms.Write(buffer, 0, read);
            fsCrypt.Close();
            ms.Close();
            return ms.ToArray();
        } catch (Exception) {
            MessageBox.Show("Unable to decrypt.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
        return null;
    }

    private const string initVector = "rtUAIkm859Vxvmrg";
    private const int keysize = 256;
    public static string DecryptString(string cipherText, string passPhrase) {
        byte[] initVectorBytes = Encoding.UTF8.GetBytes(initVector);
        byte[] cipherTextBytes = Convert.FromBase64String(cipherText);
        PasswordDeriveBytes password = new PasswordDeriveBytes(passPhrase, null);
        byte[] keyBytes = password.GetBytes(keysize / 8);
        RijndaelManaged symmetricKey = new RijndaelManaged();
        symmetricKey.Mode = CipherMode.CBC;
        ICryptoTransform decryptor = symmetricKey.CreateDecryptor(keyBytes, initVectorBytes);
        MemoryStream memoryStream = new MemoryStream(cipherTextBytes);
        CryptoStream cryptoStream = new CryptoStream(memoryStream, decryptor, CryptoStreamMode.Read);
        byte[] plainTextBytes = new byte[cipherTextBytes.Length];
        int decryptedByteCount = cryptoStream.Read(plainTextBytes, 0, plainTextBytes.Length);
        memoryStream.Close();
        cryptoStream.Close();
        return Encoding.UTF8.GetString(plainTextBytes, 0, decryptedByteCount);
    }
}
