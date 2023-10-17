#include <stdio.h>
#define TAM 100

// define a struct
typedef struct {

  char name[TAM][TAM];
  int min;

} user, *ptruser;

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


int main(void) {
  
  int num;
  
  ptruser u;
  user user[TAM];
  
  
  for(size_t i = 0; i < TAM; i++){
    // por hora acrescente o nome do usuario por escrita, depois será implementado
    // via html
    
    printf("Nome do usuario\n";
    fgets(&user[i].name[i][TAM], TAM, stdin);
    enter(&user[i].name[i][TAM], TAM);
    
    user[i].min = 5;
    //user[i].min = user[0].min*60;
    num = 0;
    
    while (num != user[i].min){
      printf("%d\n", num);
      sleep(1);
      num++;

      if (num + 1 == user[i].min){
        printf("Time's up %s!\n",&user[i].name[i][TAM]); //  maneira de indicar que o tempo do contador do microondas acabou
        break;
      }
    }
    
  }
  
  return 0;
}