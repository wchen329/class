# Fake Hazard R
# Similar to "Fake Hazard" test case
# But this time, to R inst add
lw $s0, 400($zero)
add $zero, $s0, $s0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
.exit
