using SafeRequest.NET;
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

public class Networking {

    public static Response authenticate(string license_key) {
        SafeRequest.NET.SafeRequest sr = new SafeRequest.NET.SafeRequest("4xsuMiR8BW7doioV");

        NameValueCollection values = new NameValueCollection();
        values["license"] = license_key;
        values["hwid"] = Utilities.getHWID();

        return sr.Request("http://chinaenhance.xyz/authenticate.php", values);
    }

    public static void Alert(string ExitReason) {
        WebClient web = new WebClient();
        web.Proxy = null;
        web.Headers.Add("user-agent", "chinaenhance");
        NameValueCollection values = new NameValueCollection();
        values["gay"] = ExitReason;
        byte[] responseArray = web.UploadValues("http://chinaenhance.xyz/alert.php", values);
    }
}

