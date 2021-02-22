# Big Stall Forward
# This test case engages multiple forwarding paths and stalls
# It's a test for correctness
addi $s0, $zero, -1
sw $s0, 3000($s0)
lw $s1, 3000($s0)
sw $s1, 4000($s1)
lhu $s2, 3000($s0)
lhu $s3, 4000($s1)
beq $s2, $s3, SUCCESS
sll $s0, $s0, 0
sll $s0, $s0, 0
sll $s0, $s0, 0
sll $s0, $s0, 0
.exit
sll $s0, $s0, 0

SUCCESS:
addi $k0, $zero, 999
sll $s0, $s0, 0
sll $s0, $s0, 0
sll $s0, $s0, 0
sll $s0, $s0, 0
sll $s0, $s0, 0
.exit
