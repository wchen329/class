j MAIN 
MULTI: #$s0 = $t0 * $t1
	addi $s0, $zero, 0
MULTI_B:
	add $s0, $s0, $t0
	addi $t1, $t1, -1
	bne $t1, $zero, MULTI_B
	jr $ra
MAIN:  #calculates $s0 <- 5 * 4 = 20
	addi $t0, $zero, 5
	addi $t1, $zero, 4
	jal MULTI
	sll $zero, $zero, 0
	sll $zero, $zero, 0
	.exit
