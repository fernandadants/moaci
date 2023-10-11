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
        <h1>Bem vindo ao MOACI!</h1>
        <img src="microondasteste.jpg" width="64x64">
    
    
        <div>
    
          <p>Informe o meu seu nome: </p>
          <label for="nome">Nome(Letras iniciais do seu nome):</label>
          <input type="text" id="nome" name="nome" required minlength="4" maxlength="8" size="10" />
          <button id="botao" onclick="">Clique aqui</button>
        </div>

        </body>
      <html>
    )===="; 
    request->send_P(200, "text/html", index_html); // Envia a resposta para o usuário
  });

  server.begin(); // inciar servidor

}
void loop() {}