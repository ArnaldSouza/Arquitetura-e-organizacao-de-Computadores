.data 

.text

MAIN:

	addi $t0, $zero, 5	# a = 5
	addi $t1, $zero, 10	# b = 10
	addi $t2, $zero, 30	# c = 30
	addi $t3, $zero, 20	# d = 20


SUB:
	sub $s0, $t0, $t1	# a - b	
	sub $s1, $t2, $t3	# c - d	


SUM:
	add $s2, $t0, $t1	# a + b
	add $s3, $s2, $s1	# a + b + (c - d)


FIM:
	addi $v0, $zero, 1	# o numero 1 é a chamada do sistema para imprimir um inteiro
	add $a0, $zero, $s3	# $a0 local onde vai colocar o que vai ser impresso, nesse caso $s3 - sum(a,b,c,d)
	syscall			# chama o sistema
	
	addi $v0, $zero, 10	# o numero 10 é a chamada do sistema para sair do programa
	sycall			# chama o sistema
	
	