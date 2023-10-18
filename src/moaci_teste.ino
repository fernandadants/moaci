#include "header.h"

AsyncWebServer server(80);

/*const char* html = R"rawliteral(
<!DOCTYPE HTML><html><head><title>Cadastro de Pessoas</title></head>
<body>
  <h2>Cadastro de Pessoas</h2>
  <form action="/cadastro" method="post">
    Nome: <input type="text" name="nome"><br>
    Tempo: <input type="number" name="tempo"><br>
    <input type="submit" value="Cadastrar">
  </form>
  <h2>Pessoas Cadastradas</h2>
  %pessoas%
</body></html>)rawliteral";*/

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String listaPessoas() {
  String lista = "<tbody>";
  for (int i = 0; i < numPessoas; i++) {
    lista += "<tr>";
    lista += "<td>" + String(i) + "</td>";
    lista += "<td>" + pessoas[i].nome + "</td>";
    lista += "<td>0" + String(pessoas[i].tempo) + ":00</td>";
    lista += "/<tr>";
  }
  lista += "</tbody>";
  return lista;
}

void setup() {
  Serial.begin(115200);
  
  // Configura o ESP32 como um ponto de acesso (Access Point)
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID, apPassword);
  Serial.println("Access Point criado. SSID: " + String(apSSID));

  // Inicializa o sistema de arquivos SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }

  //Rodando site
  /*server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String response = html;
    response.replace("%pessoas%", listaPessoas());
    request->send(200, "text/html", response);
  });*/

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String response = SPIFFS.open("/index.html", "r").readString();
    response.replace("<%pessoas%>", listaPessoas());
    request->send(200, "text/html", response);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });


  server.on("/cadastro", HTTP_POST, [](AsyncWebServerRequest *request){
    String nome = request->arg("nome");
    int tempo = request->arg("tempo").toInt();

    // Verifica se o array de pessoas ainda tem espaço
    if (numPessoas < maxPessoas) {
      // Armazena os dados da pessoa no array
      pessoas[numPessoas].nome = nome;
      pessoas[numPessoas].tempo = tempo;
      numPessoas++;
    }

    // Redireciona de volta para a página principal
    request->redirect("/");
  });

  server.onNotFound(notFound);

  server.begin();
}

void loop() {
  // Obtenha e mostre o endereço IP do Access Point continuamente no loop
  //Serial.print("Endereço IP do Access Point: ");
  //Serial.println(WiFi.softAPIP());
  //delay(10000); // Atraso de 10 segundos entre as exibições
}