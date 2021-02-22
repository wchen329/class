addi $zero, $zero, 50 # make sure zero register is read only
addi $at, $zero, 1
addi $v0, $zero, 2
addi $v1, $zero, 3
addi $a0, $zero, 4
addi $a1, $zero, 5
addi $a2, $zero, 6
addi $a3, $zero, 7 
addi $t0, $zero, 8
addi $t1, $zero, 9 
addi $t2, $zero, 10
addi $t3, $zero, 11
addi $t4, $zero, 12
addi $t5, $zero, 13
addi $t6, $zero, 14
addi $t7, $zero, 15
addi $s0, $zero, 16
addi $s1, $zero, 17
addi $s2, $zero, 18
addi $s3, $zero, 19
addi $s4, $zero, 20
addi $s5, $zero, 21
addi $s6, $zero, 22
addi $s7, $zero, 23
addi $t8, $zero, 24
addi $t9, $zero, 25
addi $k0, $zero, 26
addi $k1, $zero, 27
addi $gp, $zero, 28
addi $sp, $zero, 29
addi $fp, $zero, 30
addi $ra, $zero, 31

# No ops for pipelined register test
sll $zero $zero, 0
sll $zero $zero, 0
sll $zero $zero, 0
sll $zero $zero, 0
.exit
