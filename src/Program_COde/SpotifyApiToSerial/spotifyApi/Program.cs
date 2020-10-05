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
        private static ImplicitGrantAuth auth;
        private static readonly string _clientId = ConfigurationManager.AppSettings["clientId"];
        private static readonly string _secretId = ConfigurationManager.AppSettings["secretId"];

        public static void Main()
        {
             auth = new ImplicitGrantAuth(
              _clientId,
              "http://localhost:4002/callback",
              "http://localhost:4002/callback",
              Scope.UserReadPrivate
            );
            auth.AuthReceived += (sender, payload) =>
           {
               auth.Stop(); // `sender` is also the auth instance
               _spotify = new SpotifyWebAPI()
               {
                   TokenType =  payload.TokenType,
                   AccessToken = payload.AccessToken
               };
               // Do requests with API client
               
           };
            auth.Start(); // Starts an internal HTTP Server
            auth.OpenBrowser();



            PrivateProfile profile = _spotify.GetPrivateProfile();
            if (profile.HasError())
            {
                Console.WriteLine("Error Status: " + profile.Error.Status);
                Console.WriteLine("Error Msg: " + profile.Error.Message);
            }


            FullTrack track = _spotify.GetTrack("3FFjdo3CSKqeGx3nlN0WWv?si=4xDdGmMaTJ-FpPDdQi8MRA");

            Console.WriteLine(track.Name); //Yeay! We just printed a tracks name.
            Console.WriteLine(track.Popularity); //Yeay! We just printed a tracks name.
            Console.WriteLine(track.TrackNumber); //Yeay! We just printed a tracks name.
            Console.ReadLine();
        }

        class ClientCredentials
        {
            //this works for geting on-user-related information e.g search for a Track.
            //private static SpotifyWebAPI _spotify;
            //private static readonly string _clientId = ConfigurationManager.AppSettings["clientId"];
            //private static readonly string _secretId = ConfigurationManager.AppSettings["secretId"];

            async Task Run()
            {
                //CredentialsAuth auth = new CredentialsAuth(_clientId, _secretId);
                //Token token = await auth.GetToken();
                //_spotify = new SpotifyWebAPI()
                //{
                //    AccessToken = token.AccessToken,
                //    TokenType = token.TokenType
                //};

                //FullTrack track = _spotify.GetTrack("02itaCXOdC54J0ISjqqFAp?si=aErXwbbHQ-mwggedjmgUkg");

                //Console.WriteLine(track.Name); //Yeay! We just printed a tracks name.
                //Console.WriteLine(track.Popularity); //Yeay! We just printed a tracks name.
                //Console.WriteLine(track.TrackNumber); //Yeay! We just printed a tracks name.
                //Console.ReadLine();

            }

        }
    }
}
