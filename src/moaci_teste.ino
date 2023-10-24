#include "header.h"

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void setup()
{
  // Definindo portas de cada item
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTOM, INPUT_PULLUP);

  // Começando com led apagado
  digitalWrite(LED, LOW);

  // Inicia a comunicação com o serial
  Serial.begin(115200);

  // Configura o ESP32 como um ponto de acesso (Access Point)
  WiFi.mode(WIFI_AP);

  // Define o nome e senha da rede
  WiFi.softAP(apSSID, apPassword);
  Serial.println("Access Point criado. SSID: " + String(apSSID));

  // Inicializa o sistema de arquivos SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }

  // Definindo rotas para o site
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String response = SPIFFS.open("/index.html", "r").readString();
    response.replace("<%pessoas%>", listaPessoas());
    request->send(200, "text/html", response); });

  // Rota para o arquivo css
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  // Rota para o cadastro de novas pessoas
  server.on("/cadastro", HTTP_POST, [](AsyncWebServerRequest *request)
            {
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
    request->redirect("/"); });
  
  server.on("/moaci", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/moaci.png", "image/png");
  });

  server.onNotFound(notFound);

  server.begin();
}

void loop()
{
  // Definindo tempo de carregamento do esp
  unsigned long tempo_corrido = millis();

  if (pessoa_ativa)
  {
    if (tempo_corrido >= proximo_botao)
    {
      tone(BUZZER, 262, 1000);
      Serial.println("Terminou o tempo");
      Pessoa excluida = dequeue();
      pessoa_ativa = false;
      digitalWrite(LED, LOW);
      led_aceso = false;
      ultimo_tempo = tempo_corrido;
    }
  }

  if (numPessoas > 0)
  {
    Pessoa proximaPessoa = pessoas[primeiro_i];

    if (tempo_corrido >= proximo_botao)
    {
      if (tempo_corrido - ultimo_tempo <= tempo_limite)
      {
        if (tempo_corrido - ultimo_tempo <= 1000)
        {
          tone(BUZZER, 252);
        }
        else
        {
          noTone(BUZZER);
        }

        int estado_botao = digitalRead(BUTTOM);

        if (estado_botao == HIGH)
        {
          noTone(BUZZER);
          botao_clicado = true;
          pessoa_ativa = true;
        }
        else
        {
          if (!botao_clicado)
          {
            botao_clicado = false;
          }
        }
      }
      else if (tempo_corrido - ultimo_tempo >= tempo_limite && !botao_clicado)
      {
        String lista_pessoas = listaPessoas();
        Serial.println(lista_pessoas);
        Pessoa excluida = dequeue();
        Serial.println(excluida.nome + " excluído(a) por falta de comunicação... ");
        proximo_botao = tempo_corrido;
        ultimo_tempo = tempo_corrido;
      }
    }

    if (pessoa_ativa && !led_aceso)
    {
      Serial.println("Iniciando tempo de " + proximaPessoa.nome);
      digitalWrite(LED, HIGH);
      led_aceso = true;
      botao_clicado = false;
      proximo_botao = tempo_corrido + proximaPessoa.tempo * 30000;
      // proxima_pessoa = tempo_corrido + proximaPessoa.tempo * 60000;
    }
  }
  else
  {
    ultimo_tempo = tempo_corrido;
  }
}