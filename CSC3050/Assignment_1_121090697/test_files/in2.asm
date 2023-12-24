.text
fibonacci:
addi $sp, $sp, -12
sw $ra, 8($sp)
sw $s0, 4($sp)
sw $s1, 0($sp)
add $s0, $a0, $zero
addi $v0, $zero, 1
slti $t7, $s0, 2
bne $t7, $zero, fibonacciExit
addi $a0, $s0, -1
jal fibonacci
add $s1, $zero, $v0
addi $a0, $s0, -2
jal fibonacci
add $v0, $s1, $v0
fibonacciExit:
lw $ra, 8($sp)
lw $s0, 4($sp)
lw $s1, 0($sp)
addi $sp, $sp, 12
jr $ra
