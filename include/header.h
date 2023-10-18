#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#define LED 33
#define BUZZER 35
#define BUTTOM 27

const char* apSSID = "MoaciNet"; // Nome do Access Point
const char* apPassword = "moaci1234"; // Senha do Access Point

struct Pessoa {
  String nome; //nome do usuário
  int tempo; //tempo em minutos
};

const int maxPessoas = 10; // Número máximo de pessoas a serem armazenadas
Pessoa pessoas[maxPessoas]; // Array de structs Pessoa
int numPessoas = 0; // Contador de pessoas armazenadas

int primeiro_i = 0; // Índice do primeiro elemento na fila
int ultimo_i = 0; // Índice do último elemento na fila

bool led_aceso = false;
unsigned long ultimo_tempo = 0;
unsigned long proxima_chamada = 0;
unsigned long tempo_limite = 1 * 60000;
unsigned long tempo_botao = 0;

bool estado_botao = false;

//Adiciona a pessoa no fim da lista
void enqueue(const Pessoa &p) {
  if (numPessoas < maxPessoas) {
    pessoas[ultimo_i] = p;
    ultimo_i = (ultimo_i + 1) % maxPessoas; // Circular, para reutilizar espaço
    numPessoas++;
  }
}

//Retorna a pessoa mais antiga da lista
Pessoa dequeue() {
  if (numPessoas > 0) {
    Pessoa p = pessoas[primeiro_i];
    primeiro_i = (primeiro_i + 1) % maxPessoas; // Circular, para reutilizar espaço
    numPessoas--;
    return p;
  }
  // Retorne uma pessoa vazia
  Pessoa vazia;
  vazia.nome = "";
  vazia.tempo = 0;
  return vazia;
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