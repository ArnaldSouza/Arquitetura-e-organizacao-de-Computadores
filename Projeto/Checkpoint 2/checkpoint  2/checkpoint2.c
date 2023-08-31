/* Simulador: Checkpoint 2
* Arnald Souza,                           2271913
* Carlos Eduardo da Silva Ribeiro,        2271931
* João Pedro Neigri Heleno,               2270323
* Sandro Pinheiro Christe,                2270404
*/

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

#define SIZE 256

uint32_t r[32];
uint32_t ram[SIZE];
uint32_t codigo_operacao, registrador_fonte, registrador_temporario, registrador_destino, quantidade_deslocada, funcao,imediato;

uint32_t ic, pc;

int main(int argc, const char * argv[]){

  FILE *arq;
  int numeroBits, aux;
  uint32_t instrucao;

  if(argc != 2){
    printf("Erro: Número inválido de parâmetros!\n\n");
    exit(1);
  }

  arq = fopen(argv[1], "r+b");

  if (arq == NULL){
    printf("Problemas ao abrir arquivo\n");
    exit(1);
  }

  numeroBits = fread(ram, 4, 100, arq);

  for(int i = 0; i < numeroBits; i++){

    pc = i;

    ic = (uint32_t)ram[i];

    instrucao = ((ic & 0xFF000000) >> 24 | (ic & 0x00FF0000) >> 8 | (ic & 0x0000FF00) << 8 |  (ic & 0x000000FF) << 24);

    ic = instrucao;

// Recebe o OPCODE da instrução MIPS
    codigo_operacao = (ic & 0xfc000000) >> 26;
// Recebe os registradores das instruções
    registrador_fonte = (ic & 0x3E00000) >> 21;
    registrador_temporario = (ic & 0x1F0000) >> 16;
    registrador_destino = (ic & 0xF800) >> 11;
// Recebe o SHAMT e o Funct, são necessária em instruções do tipo R
    quantidade_deslocada = (ic & 0x7C0) >> 6;
    funcao = (ic & 0x3F);
// Recebe o Imediato, necessário em instruções do tipo I
    imediato = (ic & 0xffff);

    //printf("\nopcode %01x | rs %01x | rt %01x | rd %01x | shamt %01x | funct %01x\n", codigo_operacao, registrador_fonte,registrador_temporario,registrador_destino,quantidade_deslocada,funcao);


    switch(codigo_operacao){

      case 0x0:

        switch(funcao){

          case 0x20: // ADD
            //r[registrador_temporario] = 3;
            //r[registrador_fonte] = 2;

            r[registrador_destino] = r[registrador_temporario] + r[registrador_fonte];
            printf("ADD: Registrador destino é igual à %d\n" , r[registrador_destino]);
          break;

          case 0x22:// SUB
            //r[registrador_temporario] = 3;
            //r[registrador_fonte] = 2;
            r[registrador_destino] = r[registrador_temporario] - r[registrador_fonte];
            printf("SUB: Registrador destino é igual à %d\n" , r[registrador_destino]);
          break;

          case 0x18:// MULT
            //r[registrador_temporario] = 3;
            //r[registrador_fonte] = 2;
            r[registrador_destino] = r[registrador_temporario] * r[registrador_fonte];
            printf("MULT: Registrador destino é igual à %d \n" , r[registrador_destino]);
          break;

          case 0x1a:// DIV
            //r[registrador_temporario] = 6;
            //r[registrador_fonte] = 2;
            r[registrador_destino] = r[registrador_temporario] / r[registrador_fonte];
            printf("DIV: Registrador destino é igual à %d \n" , r[registrador_destino]);
          break;

          case 0x24:// AND
            //r[registrador_temporario] = 7;
            //r[registrador_fonte] = 8;
            r[registrador_destino] = (r[registrador_temporario] & r[registrador_fonte]);
            printf("AND: Registrador destino é igual à %d \n" , r[registrador_destino]);
          break;

          case 0x25:// OR
            //r[registrador_temporario] = 7;
            //r[registrador_fonte] = 8;
            r[registrador_destino] = (r[registrador_temporario] | r[registrador_fonte]);
            printf("OR: Registrador destino é igual à %d \n" , r[registrador_destino]);
          break;

          case 0x26:// XOR
            //r[registrador_temporario] = 7;
            //r[registrador_fonte] = 8;
            r[registrador_destino] = (r[registrador_temporario] ^ r[registrador_fonte]);
            printf("XOR: Registrador destino é igual à %d \n" , r[registrador_destino]);
          break;

          case 0x27:// NOR
            //r[registrador_temporario] = 9;
            //r[registrador_fonte] = 10;
            r[registrador_destino] = ~(r[registrador_temporario] | r[registrador_fonte]);
            printf("NOR: Registrador destino é igual à %d \n" , r[registrador_destino]);
          break;

          case 0x2a:// SLT
            //r[registrador_temporario] = 10;
            //r[registrador_fonte] = 9;
            if(r[registrador_temporario] < r[registrador_fonte]){
              r[registrador_destino] = 0x01;
              printf("Registrador destino é igual à %d \n" , r[registrador_destino]);
            }else{
              r[registrador_destino] = 0x00;
              printf("SLT: Registrador destino é igual à %d \n" , r[registrador_destino]);
            }
          break;

          case 0x00:// SLL
            //r[registrador_temporario] = 15;
            r[registrador_destino] = r[registrador_temporario] << quantidade_deslocada;
            printf("SLL: Registrador destino é igual à %d \n" , r[registrador_destino]);
          break;

          case 0x02:// SRL
            //r[registrador_temporario] = 240;
            r[registrador_destino] = (r[registrador_temporario]) >> quantidade_deslocada;
            printf("SRL: Registrador destino é igual à %d \n" , r[registrador_destino]);
          break;

          case 0x03:// SRA
            //r[registrador_temporario] = 37;
            aux = r[registrador_temporario];
            if (aux < 0) {
              r[registrador_destino] = r[registrador_temporario]  >> quantidade_deslocada | ~(~0U >> quantidade_deslocada);
            }else {
              r[registrador_destino] = r[registrador_temporario]  >> quantidade_deslocada;
            }
            printf("SRA: Registrador destino é igual à %d \n" , r[registrador_destino]);
          break;
        }

      break;

      case 0x08://OP_CODE 8 ADDI
        //r[registrador_temporario] = 1;
        r[registrador_fonte] = r[registrador_temporario] + imediato;
        printf("ADDI: Registrador destino é igual à %d \n" , r[registrador_fonte] );
      break;

      case 0x0c://OP_CODE 12 ANDI
        //r[registrador_temporario] = 8;
        r[registrador_fonte] = (r[registrador_temporario] & imediato);
        printf("ANDI: Registrador destino é igual à %d \n", r[registrador_fonte]);
      break;

      case 0x0d://OP_CODE 13 ORI
        //r[registrador_temporario] = 16;
        r[registrador_fonte] = (r[registrador_temporario] | imediato);
        printf("ORI: Registrador destino é igual à %d \n" , r[registrador_fonte]);
      break;

      case 0x0e://OP_CODE 14 XORI
        //r[registrador_temporario] = 7;
        r[registrador_fonte] = (r[registrador_temporario] ^ imediato);
        printf("XORI: Registrador destino é igual à %d \n" , r[registrador_fonte]);
      break;

      case 0x23://OP_CODE LW
      printf("%i", registrador_fonte);

        r[registrador_fonte] = SIZE;
        imediato = imediato * -1;

        if(r[registrador_fonte] + imediato >= r[registrador_fonte] || r[registrador_fonte] + imediato <= numeroBits){
          printf("Stack Overflow!\n");
        }else{
          r[registrador_temporario] = ram[r[registrador_fonte] + imediato];
          printf("LW: registrador destino é igual a %d\n", r[registrador_temporario]);
        }
      break;

      case 0x2b://OP_CODE SW
      printf("%i", registrador_fonte);
        r[registrador_fonte] = SIZE;
        imediato = imediato * -1;

        if(r[registrador_fonte] + imediato >= r[registrador_fonte] || r[registrador_fonte] + imediato <= numeroBits){
          printf("Stack Overflow!\n");
        }else{
          ram[r[registrador_fonte] + imediato] = r[registrador_temporario];
          printf("SW: a posição na RAM é igual a destino é igual a %d\n", ram[r[registrador_fonte] + imediato]);
        }
      break;
    }
  }
  return 0;
}//main
