#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

const char* apSSID = "MoaciNet"; // Nome do Access Point
const char* apPassword = "moaci1234"; // Senha do Access Point

struct Pessoa {
  String nome; //nome do usuário
  int tempo; //tempo em minutos
};

const int maxPessoas = 10; // Número máximo de pessoas a serem armazenadas
Pessoa pessoas[maxPessoas]; // Array de structs Pessoa
int numPessoas = 0; // Contador de pessoas armazenadas