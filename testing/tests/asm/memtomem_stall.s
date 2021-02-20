# Mem to Mem
# This test case engages multiple forwarding paths and stalls with a twist-
# Mem to mem is replaced with a stall
# One stall is are expected.

.cpuopts PATH_MEM_MEM=STALL
addi $v0, $zero, -3000
sw $v0, 9000($zero)
lw $v1, 9000($zero)
sw $v1, 10000($zero)
lw $s0, 10000($zero)
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
.exit
