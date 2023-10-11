// Inclusão das bibliotecas
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

const char* id = "MoaciNet";    // nome
const char* password = "moaci1234"; // senha


void setup() {



  WiFi.mode(WIFI_AP);
  WiFi.softAP(id, password); // iniciando a rede

  
  // Se cria o url do MOACI
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request){
     const char index_html[] PROGMEM = R"====(
      <html>
        <head>
          <meta charset='utf-8'/>
        </head>
        <body>
          <h1>
            MOACI
          </h1>
          
          <p>
            Este é o MOACI 
          </p>
          <div>
          <h1>Clique no botão abaixo e altere o seu estilo:</h1>
          <button id="novaCor" onClick="cliqueBtn()">Clique aqui</button>
          </div>

          <figure>
            <img src='microondastest.jpg' width='300px'>
            <figcaption>
              Arduino versão Uno. Créditos: Wikipedia
            </figcaption>
          </figure>
        
          <p>
            Fonte: <a href='https://pt.wikipedia.org/wiki/Arduino' target='_blank'>https://pt.wikipedia.org/wiki/Arduino</a>
          </p>
        </body>
      <html>
    )===="; 
    request->send_P(200, "text/html", index_html); // Envia a resposta para o usuário
  });

  server.begin(); // inciar servidor

}
void loop() {}