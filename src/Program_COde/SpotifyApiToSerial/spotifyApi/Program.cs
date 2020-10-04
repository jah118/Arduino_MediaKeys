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
        private static string _clientId = "38dbac438ad440dc8bdb82ac3516a9d6";
        private static string _secretId = "ef4b8f7b5467462cae12de4d0da2e018";

        public Program()
        {

        }

        static async void Main(string[] args)
        {
            AuthorizationCodeAuth auth = new AuthorizationCodeAuth(
            _clientId,
            _secretId,
            "http://localhost:4002",
            "http://localhost:4002",
            Scope.PlaylistReadPrivate | Scope.PlaylistReadCollaborative
            );

            auth.AuthReceived += async (sender, payload) =>
            {
                auth.Stop();
                Token token = await auth.ExchangeCode(payload.Code);
                SpotifyWebAPI api = new SpotifyWebAPI()
                {
                    TokenType = token.TokenType,
                    AccessToken = token.AccessToken
                };
                // Do requests with API client
            };
            auth.Start(); // Starts an internal HTTP Server
            auth.OpenBrowser();
            // CredentialsAuth auth = new CredentialsAuth("38dbac438ad440dc8bdb82ac3516a9d6", "ef4b8f7b5467462cae12de4d0da2e018");


        }


    }
}
