# Fake Hazards
# Tests a few so called "fake hazards"
# i.e. certain cases which may get picked up as hazards by hazard detection but aren't really
# No stalls are expected

# WAW
addi $s0, $s0, 100
addi $s0, $zero, 100
ori $s1, $zero, 50
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sw $s0, 100($zero)
sw $s1, 104($zero)

# WAW Loads, after Following Stores
lw $s2, 104($zero)
lw $s2, 100($zero)

# Arithmetic and Writing to the Zero Register
addi $zero, $zero, 500
sub $zero, $zero, $zero
lw $zero, 104($zero)
add $zero, $zero, $zero
sll $zero, $zero, 0
addu $zero, $zero, $zero

# Check to make sure corrupting random forward didn't happen...
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
.exit
