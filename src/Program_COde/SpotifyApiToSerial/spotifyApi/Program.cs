using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SpotifyAPI.Web; //Base Namespace
using SpotifyAPI.Web.Auth;
using SpotifyAPI.Web.Enums; //Enums
using SpotifyAPI.Web.Models; //Models for the JSON-responses



namespace spotifyApi
{
    class Program
    {
        private static SpotifyWebAPI _spotify;
        private static readonly string _clientId = ConfigurationManager.AppSettings["clientId"];
        private static readonly string _secretId = ConfigurationManager.AppSettings["secretId"];

        static async Task Main()
        {
            //var spotify = new SpotifyClient("BQCh683yxsiAb4BPb9VmdIl5KpmbTgVQ1xn5TLN5b6pPDFseYVknV5ZgJYBsQPEsXBTUunqwbPumVkwoFHXn33aEEaES77HIH6MbZnRV4bmtpkcbJjsLnvG9AMAF2_fEkz61EXqAH4t9NoaFWqxGcyOL4w8_");

            //
            //Console.WriteLine(track.Name);
            //Console.ReadLine();

            CredentialsAuth auth = new CredentialsAuth(_clientId, _secretId);
            Token token = await auth.GetToken();
            _spotify = new SpotifyWebAPI()
            {
                AccessToken = token.AccessToken,
                TokenType = token.TokenType
            };
            FullTrack track = _spotify.GetTrack("02itaCXOdC54J0ISjqqFAp?si=aErXwbbHQ-mwggedjmgUkg");
            
            Console.WriteLine(track.Name); //Yeay! We just printed a tracks name.
            Console.WriteLine(track.Popularity); //Yeay! We just printed a tracks name.
            Console.WriteLine(track.TrackNumber); //Yeay! We just printed a tracks name.
            Console.ReadLine();
        }
    }
}
