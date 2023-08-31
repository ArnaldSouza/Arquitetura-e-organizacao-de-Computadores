.data

 MSG1: 
	.asciiz "Programa para calcular a soma de 10+9+8+7...0, recursivamente\n\n"
 MSG2:
	.asciiz "A soma é:"


.text


MAIN:
	la $a0, mensagem1		# imprime o conteúdo de MSG1
	addi $v0,$zero, 4		# o numero 4 é a chamada do sistema para imprimir uma string										#Salva em v0 o valor 4 (para imprimir string)
	syscall				# chama o sistema
	li $a0, 10			# n = 10
	jal SOMA			# faz o pulo para a label SOMA								
	j FIM				# pula para a label FIM


SOMA:
	addi $sp, $sp, -8		# aloca na pilha espaço de 8 bytes para armazenar os valores dos registradores
	sw $a0, 4($sp)			# salva o conteudo de $a0 dentro da pilha, com deslocamento 4
	sw $ra, 0($sp)			# salva o conteudo de $ra dentro da pilha, sem deslocamento

	slt $t0, $zero, $a0		# compara se $a0 > 0
	bne $t0, $zero, IF		# se $a0 <= 0 for verdade vai para a label IF 	

	li $v0, 0			# return 0		
	addi $sp, $sp, 8       		# retira 2 itens da pilha
	jr $ra				#retorna para quem chamou


IF:
	
	addi $a0, $a0, -1		# aloca na pilha espaço de 1 byte para armazenar os valores dos registradores
	jal SOMA			# pula para SOMA

	lw $a0, 4($sp)			# restaura o valor de $a0
	lw $ra, 0($sp)			# restaura ra
	add $sp, $sp, 8			# ajusta stack para retirar 2 itens

	add $v0, $v0, $a0		# retorna o valor de $v0

	jr $ra				# retorna para quem chamou


FIM:
	move $a1, $v0			# armazena o valor que vai ser impresso em $v0
	
	la $a0, MSG2			# (load address), carrega o endereço de MSG2 dentro de $a0
	li $v0, 4			# faz a chamada nº 4 que é para imprimir uma string
	syscall				# chama o sistema

	move $a0, $v0			# armazena o valor que vai ser impresso em $a1
	li $v0, 1			# faz a chamada do sistema para imprimir um inteiro
	syscall				# chama o sistema

	li $v0, 10		  	#faz a chamada do sistema para sair do programa
    	syscall		 	  	#chama o sistema
	
	




















	