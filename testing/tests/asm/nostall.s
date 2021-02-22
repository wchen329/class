# "No stall"
# This test cases tries to catch several instructions that
# may accidentally cause stalls but do not require them

# Load to use, separated by no-op
addi $v0, $zero, 700
sw $v0, 700($zero)
addi $s0, $zero, 500
sw $s0, 200($zero)
lw $s3, 200($zero)
sll $zero, $zero, 0
lw $s1, 200($s3)

# Stores followed directly by a load
addi $a0, $zero, 15

sb $a0, 1000($a0)
lbu $t0, 1000($a0)
sh $a0, 3000($a0)
lhu $t1, 3000($a0)
sw $a0, 5000($a0)
lw $t2, 5000($a0)

sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
.exit
