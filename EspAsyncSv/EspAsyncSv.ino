// Inclusão das bibliotecas
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

const char* id = "MoaciNet";    // nome
const char* password = "moaci1234"; // senha


void setup() {

  WiFi.begin(id, password); // iniciando a rede

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request){
    AsyncWebServerResponse * response = request ->beginResponse(200, "text/plain", "Olá usuário!");   /// interação básica para saber se está funcionando
    response->addHeader("Server", "ESP Async Web Server");
    request->send(response); // podemos redirecionar o usuário para a pagina desejada com o request->redirect(link)
  });

  server.begin(); // inciar servidor

}
void loop() {}