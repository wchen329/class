SYM:
	addi $s0, $s0, 1
	addi $s1, $zero, 1000
	bne $s0, $s1, SYM
.exit
