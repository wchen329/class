# Forwarding Control
# Tests out various formats of EX-ID forwarding and stalls used when a Load-to-use into a branch occurs
# As well as branch prediction flushing

addi $t0, $zero, 1
sw $t0, 3000($zero)
addi $s1, $zero, 50
bne $zero, $s1, FIRST 
ori $k0, $zero, -1
.exit
sll $zero, $zero, 0

FIRST:
add $s2, $zero, $s1
SECOND:
bne $s2, $zero, LOOP
addi $s3, $zero, 1
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
.exit
sll $zero, $zero, 0

LOOP:
sub $s2, $s2, $t0
j SECOND

ori $k0, $zero, -1
.exit
