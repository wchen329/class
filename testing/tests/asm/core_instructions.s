# Core Instructions
# -
# This test tests each arithmetic (i.e. all except loads, stores, branches or jumps)
# instruction in the MIPS core instruction set

# Adds and Subtractions
addi $s0, $s0, 100 # $s0 = 100
addiu $s1, $s0, 200 # $s1 = 300
add $t0, $s0, $s1 # $to = $s0 + $s1 (400)
addu $t9, $s1, $s0 # $t9 = 400
sub $at, $s1, $s0 # $at = 200
subu $a3, $at, $s0 # $a3 = 100

# Ors and Ands (Basic Logic)
ori $v0, $zero, 777 # $v0 = 777
or $v1, $v0, $zero # $v1 = 777
nor $a0 $a1, $a2 # $a0 = -1 (signed)
andi $a1, $a0, 2 # $a1 = 2
and $a2, $a0, $v1 # $a2 = 777

# Shifts
sll $s1, $a1, 1 # $s1 = 4
srl $s2, $a1, 1 # $s2 = 1

# Stores
sb $a0, 1024($zero)
sh $a0, 2048($zero)
sw $a0, 3072($zero)

# Loads
lbu $t1, 1024($zero)
lhu $t2, 2048($zero)
lw $t3, 3072($zero)

# Branches and CV Setting
slt $s3, $s1, $s2 # $s3 = 0
slti $s4, $s1, 100 # $s4 = 1
sltiu $s5, $zero, -1 # $s5 = 1 - Note in unsigned -1 is the largest integer
sltu $s6, $zero, $a0 # $s6 = 1 - same as previous

beq $v0, $v0, SKIP_1
ori $k0, $zero, -1 # error condition

SKIP_1:
bne $s3, $s5, SKIP_2
ori $k0, $zero, -1

TMP:
jr $ra
ori $k0, $zero, -1

SKIP_2:
j SKIP_3
ori $k0, $zero, -1

SKIP_3:
jal TMP
sll $zero, $zero, 0 # No-op
sll $zero, $zero, 0 # No-op
sll $zero, $zero, 0 # No-op
sll $zero, $zero, 0 # No-op
.exit
