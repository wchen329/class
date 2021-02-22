# Store Close
# Tests several adjacent store operations
addi $t0, $zero, 1
addi $t1, $zero, 2 
addi $t2, $zero, 3
addi $t3, $zero, 4
addi $t4, $zero, 5
addi $t5, $zero, 6

# Store these three using store operations
sb $t0, 900($zero)
sb $t1, 901($zero)
sh $t2, 902($zero)
sh $t3, 904($zero)
sw $t4, 906($zero)
sb $t5, 910($zero)

# Unload
lbu $s0, 900($zero)
lbu $s1, 901($zero)
lhu $s2, 902($zero)
lhu $s3, 904($zero)
lw $s4, 906($zero)
lw $s5, 910($zero)

sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
.exit
