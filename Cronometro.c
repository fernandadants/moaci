#include <stdio.h>
#include <unistd.h>

// funcao pra retirar o enter de onde quer que ele venha
void enter(char *blah, int tam) {

  for (size_t i = 0; i < tam; i++) {

    if (*blah == '\n') {

      *blah = '\0';
      break;
    }
    ++blah;
  }
}

void counter(char name[], int tam, int time) {

  // tam o numero de vezes que vai ser rodado o contador\
  // time o tempo que deseja rodar no contador (minutos)
  // name o nome do usuario

  int contador = 0;

  for (size_t i = 0; i < tam; i++) {

    time = 5;
    //time = time*60;

    while (contador != time) {
      printf("%d\n", contador);
      sleep(1);
      contador++;

      if (contador + 1 == time) {
        printf("Time's up %s!\n", name);
        break;
        //  maneira de indicar que o tempo do contador do microondas acabou
      }
    }
  }
}

// comeco da funcao main
int main(void) {

  char nome[100] = "giulia";

  int button = 1;

  // quando botao == 1 ele comeca o contador com os dados inseridos
  while (1) {

    if (button == 1) {
      printf("pode comecar \n");
      counter(nome, 2, 5);
    }
  }

  return 0;
}
