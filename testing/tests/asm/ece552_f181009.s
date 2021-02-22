# ECE 552
# based on In-Class Exercise 10/09/2018
# Credit: Professor Joshua San Miguel @ UW-Madison

# Extra dependency to induce initial stall
lw $s1, 0($zero)

# The "Exercise"
or $s3, $s1, $s0
lw $s2, 64($s3)
lw $s3, 0($s2)
ori  $s4, $s3, 4

# No ops
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
.exit
