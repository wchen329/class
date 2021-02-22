# Forwarding Basic
# Tests two forwarding paths (data hazards only) with varying instruction formats, no stalls should occur
# Paths: EX-EX, MEM-EX

# EX-EX I -> I
addi $t0, $zero, 100
addi $s0, $t0, 50

# MEM-EX I -> I
addi $t0, $zero, 200
sll $zero, $zero, 0
addi $s1, $t0, 50

# EX-EX I -> R
addi $t1, $zero, 10
add $s2, $t0, $t1

# MEM-EX I -> R
addi $t1, $zero, 20
sll $zero, $zero, 0
add $s3, $t0, $t1

# EX-EX and MEM-EX Case I & I -> R
addi $t0, $zero, 600
addi $t1, $zero, 800
add $s4, $t0, $t1

# EX-EX Case I & R -> I
addi $s5, $s4, 200

# EX-EX and MEM-EX Case R & R -> R
add $s6, $s5, $s4

sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0

.exit
