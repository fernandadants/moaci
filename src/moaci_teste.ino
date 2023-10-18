#include "header.h"

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  //Definindo portas de cada item
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTOM, INPUT_PULLUP);

  //Começando com led apagado
  digitalWrite(LED, LOW);

  //Inicia a comunicação com o serial
  Serial.begin(115200);
  
  // Configura o ESP32 como um ponto de acesso (Access Point)
  WiFi.mode(WIFI_AP);

  // Define o nome e senha da rede
  WiFi.softAP(apSSID, apPassword);
  Serial.println("Access Point criado. SSID: " + String(apSSID));

  // Inicializa o sistema de arquivos SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }

  // Definindo rotas para o site
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String response = SPIFFS.open("/index.html", "r").readString();
    response.replace("<%pessoas%>", listaPessoas());
    request->send(200, "text/html", response);
  });

  // Rota para o arquivo css
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Rota para o cadastro de novas pessoas
  server.on("/cadastro", HTTP_POST, [](AsyncWebServerRequest *request){
    String nome = request->arg("nome");
    int tempo = request->arg("tempo").toInt();

    // Verifica se o array de pessoas ainda tem espaço
    if (numPessoas < maxPessoas) {
      //Adicionando as novas pessoas ao final da fila
      Pessoa p;
      p.nome = nome;
      p.tempo = tempo;
      enqueue(p);
    }

    // Redireciona de volta para a página principal
    request->redirect("/");
  });

  server.onNotFound(notFound);

  server.begin();
}

void loop() {
  //Definindo tempo de carregamento do esp
  unsigned long currentTime = millis();

  // Verificar se o LED deve ser ligado
  if (numPessoas > 0 && !led_aceso) {
    Pessoa proximaPessoa = pessoas[0];
    if (currentTime - ultimo_tempo >= proximaPessoa.tempo * 60000) {
      digitalWrite(LED, HIGH); // Ligar o LED 
      led_aceso = true; // Definir o estado do LED como ligado
      // Marcar o tempo em que o LED ligou
      ultimo_tempo = currentTime;
      // Calcular o tempo para chamar a próxima pessoa
      proxima_chamada = currentTime + proximaPessoa.tempo * 60000;
    }
  }

  // Verificando se é hora de chamar a próxima pessoa
  if (numPessoas > 0 && currentTime >= proxima_chamada) {
    Pessoa chamada = dequeue();
    ultimo_tempo = currentTime; // Marcar o tempo em que o LED ligou
    digitalWrite(LED, LOW); // Desligar o LED
    led_aceso = false; // Definir o estado do LED como desligado
    Serial.println("Chamada: " + chamada.nome);
  }
}