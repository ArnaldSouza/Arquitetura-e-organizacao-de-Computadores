/* Simulador: Checkpoint 3
* Arnald Souza,                           2271913
* Carlos Eduardo da Silva Ribeiro,        2271931
* João Pedro Neigri Heleno,               2270323
* Sandro Pinheiro Christe,                2270404
*/

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

#define SIZE 256

//registradores
uint32_t r[32];
//ram 256
uint32_t ram[SIZE];
//demais variaveis
uint32_t codigo_operacao, registrador_fonte, registrador_temporario, registrador_destino, quantidade_deslocada, funcao,imediato, instrucao, endereco;
//instrucao e pc
uint32_t ic, pc;

int main(int argc, const char * argv[]){

  FILE *arq;
  int numeroBits, aux;

  r[0] = 0;//$zero -> r[0]

  if(argc != 2){
    printf("Erro: Número inválido de parâmetros!\n\n");
    exit(1);
  }

  arq = fopen(argv[1], "r+b");

  if (arq == NULL){
    printf("Problemas ao abrir arquivo\n");
    exit(1);
  }
  // em cada posição da ram 32 bits
  numeroBits = fread(ram, 4, 100, arq);// Leitura do arquivo com as instruções
  //program counter 0 a ultima instrução lida
  for(int pc = 0; pc < numeroBits; pc++){
    // ic recebe a intrução que o program counter está apontando
    ic = (uint32_t)ram[pc];
      //mascara de bits de big angel p/ little angel (incerto)
    instrucao = ((ic & 0xFF000000) >> 24 | (ic & 0x00FF0000) >> 8 | (ic & 0x0000FF00) << 8 |  (ic & 0x000000FF) << 24);

    ic = instrucao;

    // Recebe o OPCODE da instrução MIPS
    codigo_operacao = (ic & 0xfc000000) >> 26;
    // Recebe os registradores das instruções
    registrador_fonte = (ic & 0x3E00000) >> 21;
    //
    registrador_temporario = (ic & 0x1F0000) >> 16;
    //
    registrador_destino = (ic & 0xF800) >> 11;
    // Recebe o SHAMT e o Funct, são necessária em instruções do tipo R
    quantidade_deslocada = (ic & 0x7C0) >> 6;
    funcao = (ic & 0x3F);
    // Recebe o Imediato, necessário em instruções do tipo I
    imediato = (ic & 0xffff);
    // Recebe o Endereço, para fazer o salto incondicional
    endereco = (ic & 0x3FFFFFF);
    //printf("\nopcode %01x | rs %01x | rt %01x | rd %01x | shamt %01x | funct %01x\n\n", codigo_operacao, registrador_fonte,registrador_temporario,registrador_destino,quantidade_deslocada,funcao);

    switch(codigo_operacao){

      case 0x0:
        switch(funcao){

          case 0x20: // ADD
            r[registrador_destino] = r[registrador_temporario] + r[registrador_fonte];
          break;

          case 0x22:// SUB
            r[registrador_destino] = r[registrador_temporario] - r[registrador_fonte];
          break;

          case 0x18:// MULT
            r[registrador_destino] = r[registrador_temporario] * r[registrador_fonte];
          break;

          case 0x1a:// DIV
            r[registrador_destino] = r[registrador_temporario] / r[registrador_fonte];
          break;

          case 0x24:// AND
            r[registrador_destino] = (r[registrador_temporario] & r[registrador_fonte]);
          break;

          case 0x25:// OR
            r[registrador_destino] = (r[registrador_temporario] | r[registrador_fonte]);
          break;

          case 0x26:// XOR
            r[registrador_destino] = (r[registrador_temporario] ^ r[registrador_fonte]);
          break;

          case 0x27:// NOR
            r[registrador_destino] = ~(r[registrador_temporario] | r[registrador_fonte]);
          break;

          case 0x2a:// SLT
            if(r[registrador_temporario] < r[registrador_fonte]){
              r[registrador_destino] = 0x01;
            }else{
              r[registrador_destino] = 0x00;
            }
          break;
          //SHIFT LEFT 01001 -> 10010
          case 0x00:// SLL
            r[registrador_destino] = r[registrador_temporario] << quantidade_deslocada;
          break;
          //SHIFT  01001 -> 00100
          case 0x02:// SRL
            r[registrador_destino] = (r[registrador_temporario]) >> quantidade_deslocada;
          break;

          case 0x03:// SRA
            aux = r[registrador_temporario];
            if (aux < 0) {
              r[registrador_destino] = r[registrador_temporario]  >> quantidade_deslocada | ~(~0U >> quantidade_deslocada);
            }else {
              r[registrador_destino] = r[registrador_temporario]  >> quantidade_deslocada;
            }
          break;
          // JUMP REGISTER (registradores ARMAZE UM PC ESPECIFICO)
          case 0x08:// JR

            pc = r[31];// r[31] -> ra
          break;

          case 0x0c:// SYSCALL
            switch (r[registrador_destino]) {
              //print inteiro
              case 0x01:
                printf("%d\n", r[4]);
              break;
              //fecha programa
              case 0x0a:
                printf("Fechando o Programa!\n");
                exit(0);
              break;
            }
          break;
            //caso seja uma intrução inválida
          default:
            printf("OPCODE INVÁLIDO\n");
          break;
        }
      break;
      // desvio incondicional -> pula para um enderço especifico
      case 0x02://OP_CODE 2 J
        pc = (endereco - 1);
      break;
      // desvio incondicional -> armazena o pc atual e pula para um enderço especifico
      case 0x03://OP_CODE 3 JAL
        r[31] = pc;// r[31] -> ra
        pc = (endereco - 1);
      break;
      // desvio condicional -> pula para um enderço especifico IF temporario e fonte iguais
      case 0x04://OP_CODE 4 BEQ
        if(r[registrador_temporario] == r[registrador_fonte]){
          pc = pc + (imediato-1);
        }
      break;
      // desvio condicional -> pula para um enderço especifico IF temporario e fonte diferentes
      case 0x05://OP_CODE 5 BNE
        if(r[registrador_temporario] != r[registrador_fonte]){
          pc = pc + (imediato-1);
        }
      break;
      // desvio condicional -> pula para um enderço especifico IF registrador fonte > 0
      case 0x07://OP_CODE 7 BGTZ
        if(r[registrador_fonte] > 0){
          pc = pc + (imediato-1);
        }
      break;

      case 0x08://OP_CODE 8 ADDI
        r[registrador_fonte] = r[registrador_temporario] + imediato;
      break;

      case 0x0a://OP_CODE 10 SLTI
          if(r[registrador_temporario] < imediato){
          r[registrador_fonte] = 0x01;
        }else{
          r[registrador_fonte] = 0x00;
        }
      break;

      case 0x0c://OP_CODE 12 ANDI
        r[registrador_fonte] = (r[registrador_temporario] & imediato);
      break;

      case 0x0d://OP_CODE 13 ORI
        r[registrador_fonte] = (r[registrador_temporario] | imediato);
      break;

      case 0x0e://OP_CODE 14 XORI
        r[registrador_fonte] = (r[registrador_temporario] ^ imediato);
      break;
      //ele busca dentro da pilha e armazena no registrador temporário
      case 0x23://OP_CODE LW
        r[registrador_fonte] = SIZE;
        imediato = imediato * -1;

        if(r[registrador_fonte] + imediato >= r[registrador_fonte] || r[registrador_fonte] + imediato <= numeroBits){
          printf("Stack Overflow!\n");
        }else{
          r[registrador_temporario] = ram[r[registrador_fonte] + imediato];
        }
      break;
      //store word -> armazena um valor inteiro na pilha é passado de cima p/ baixo
      case 0x2b://OP_CODE SW
        r[registrador_fonte] = SIZE;
        imediato = imediato * -1;

        if(r[registrador_fonte] + imediato >= r[registrador_fonte] || r[registrador_fonte] + imediato <= numeroBits){
          printf("Stack Overflow!\n");
        }else{
          ram[r[registrador_fonte] + imediato] = r[registrador_temporario];
        }
      break;
      //se o op_code estiver inavlido
      default:
        printf("OPCODE INVÁLIDO\n");
      break;
    }
  }
  return 0;

}//main
