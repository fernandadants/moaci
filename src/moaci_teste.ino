#include "header.h"

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void setup()
{
  // Definindo portas de cada item
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTOM, INPUT_PULLUP);

  // Começando com relé sem energia
  digitalWrite(RELAY, LOW);

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

  // Se existir uma pessoa inicialmente ativa e chegar o tempo da próxima pessoa da fila
  if (pessoa_ativa)
  {
    if (tempo_corrido >= proximo_botao)
    {
      tone(BUZZER, 262, 1000);
      Serial.println("Terminou o tempo");
      Pessoa excluida = dequeue();
      pessoa_ativa = false;
      digitalWrite(RELAY, LOW);
      relay_on = false;
      ultimo_tempo = tempo_corrido;
    }
  }

  //Se o numero de pessoas for maior que 0
  if (numPessoas > 0)
  {
    //Cria um objeto do tipo pessoa que está no primeiro indice.
    Pessoa proximaPessoa = pessoas[primeiro_i];

    //Se chegar o tempo de ler o proximo botao
    if (tempo_corrido >= proximo_botao)
    {
      //Se o tempo estiver dentro do tempo limite
      if (tempo_corrido - ultimo_tempo <= tempo_limite)
      {
        //Tocar o buzzer por 1s
        if (tempo_corrido - ultimo_tempo <= 1000)
        {
          tone(BUZZER, 252);
        }
        else
        {
          noTone(BUZZER);
        }

        //Ler o estado do botão
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
        //Se passar o tempo limite e o botão não for acionado
        String lista_pessoas = listaPessoas();
        Pessoa excluida = dequeue();
        Serial.println(excluida.nome + " excluído(a) por falta de comunicação... ");
        proximo_botao = tempo_corrido;
        ultimo_tempo = tempo_corrido;
      }
    }

    if (pessoa_ativa && !relay_on)
    {
      //Se a pessoa estiver ativa, mas o relé ainda não estiver acionado
      Serial.println("Iniciando tempo de " + proximaPessoa.nome);
      digitalWrite(RELAY, HIGH);
      relay_on = true;
      botao_clicado = false;
      proximo_botao = tempo_corrido + proximaPessoa.tempo * 30000;
    }
  }
  else
  {
    //Atualizando estágios de tempo
    ultimo_tempo = tempo_corrido;
  }
}