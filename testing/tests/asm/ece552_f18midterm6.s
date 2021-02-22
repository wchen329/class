# ECE 552
# based on Fall 2018 Midterm Problem 6
# Credit: Professor Joshua San Miguel @ UW-Madison

# Setup
addi $t0, $zero, 1000
addi $t1, $zero, 1
addi $s0, $zero, 1002
addi $s1, $zero, 1000
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0

# Execution a.k.a. The Midterm
I1:
add $t0, $t0, $t1
bne $t0, $s0, I1
lw $t0, 8($s1)
sw $t0, 12($s1)

# No ops to get until sw inst
# Reaches WB
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
.exit
