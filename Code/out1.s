.data
 _prompt: .asciiz "Enter an integer : "
_ret: .asciiz "\n"
.globl main
.text
 read: 
    li $v0, 4 
    la $a0, _prompt 
    syscall 
    li $v0, 5 
    syscall 
    jr $ra 

 write: 
    li $v0, 1 
    syscall 
    li $v0, 4 
    la $a0, _ret 
    syscall 
    move $v0, $0 
    jr $ra 

compare: 
addi $fp, $sp, 0
addi $sp, $sp, -4
move $t0, $a0 
sw $t0, -4($fp)
addi $sp, $sp, -4
move $t0, $a1 
sw $t0, -8($fp)
lw $t0, -4($fp)
move $a0, $t0 
addi $sp, $sp, -4 
sw $ra, 0($sp) 
jal write  
lw $ra, 0($sp) 
addi $sp, $sp, 4  
sw $t0, -4($fp)
lw $t0, -8($fp)
move $a0, $t0 
addi $sp, $sp, -4 
sw $ra, 0($sp) 
jal write  
lw $ra, 0($sp) 
addi $sp, $sp, 4  
sw $t0, -8($fp)
lw $t0, -4($fp)
lw $t1, -8($fp)
bgt $t0, $t1, label1
sw $t0, -4($fp)
sw $t1, -8($fp)
j label2
label1:
lw $t0, -4($fp)
move $v0, $t0
jr $ra 
sw $t0, -4($fp)
label2:
lw $t0, -4($fp)
lw $t1, -8($fp)
blt $t0, $t1, label3
sw $t0, -4($fp)
sw $t1, -8($fp)
j label4
label3:
lw $t0, -8($fp)
move $v0, $t0
jr $ra 
sw $t0, -8($fp)
label4:
li $t0, 0
move $v0, $t0
jr $ra 
newadd: 
addi $fp, $sp, 0
addi $sp, $sp, -4
move $t0, $a0 
sw $t0, -4($fp)
addi $sp, $sp, -4
move $t0, $a1 
sw $t0, -8($fp)
addi $sp, $sp, -4
lw $t1, -4($fp)
lw $t2, -8($fp)
add $t0, $t1, $t2
sw $t0, -12($fp)
sw $t1, -4($fp)
sw $t2, -8($fp)
lw $t0, -12($fp)
move $v0, $t0
jr $ra 
sw $t0, -12($fp)
main: 
addi $fp, $sp, 0
addi $sp, $sp, -40
addi $sp, $sp, -20
addi $sp, $sp, -4
li $t0, 0
sw $t0, -64($fp)
label5:
lw $t0, -64($fp)
li $t1, 10
blt $t0, $t1, label6
sw $t0, -64($fp)
j label7
label6:
addi $sp, $sp, -4
lw $t1, -64($fp)
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -68($fp)
sw $t1, -64($fp)
addi $sp, $sp, -4
addi $t1, $fp, -4
lw $t2, -68($fp)
sub $t0, $t1, $t2
sw $t0, -72($fp)
sw $t2, -68($fp)
addi $sp, $sp, -4
addi $sp, $sp, -4 
sw $ra, 0($sp) 
jal read 
lw $ra, 0($sp) 
addi $sp, $sp, 4 
move $t0, $v0 
sw $t0, -76($fp)
lw $t0, -72($fp)
lw $t1, -76($fp)
sw $t1, 0($t0)
sw $t0, -72($fp)
sw $t1, -76($fp)
addi $sp, $sp, -4
lw $t1, -64($fp)
addi $t0, $t1, 1
sw $t0, -80($fp)
sw $t1, -64($fp)
lw $t0, -64($fp)
lw $t1, -80($fp)
move $t0, $t1
sw $t0, -64($fp)
sw $t1, -80($fp)
j label5
label7:
lw $t0, -64($fp)
li $t0, 0
sw $t0, -64($fp)
addi $sp, $sp, -4
li $t0, 0
sw $t0, -84($fp)
label8:
lw $t0, -64($fp)
li $t1, 10
blt $t0, $t1, label9
sw $t0, -64($fp)
j label10
label9:
addi $sp, $sp, -4
lw $t1, -64($fp)
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -88($fp)
sw $t1, -64($fp)
addi $sp, $sp, -4
addi $t1, $fp, -4
lw $t2, -88($fp)
sub $t0, $t1, $t2
sw $t0, -92($fp)
sw $t2, -88($fp)
addi $sp, $sp, -4
lw $t1, -92($fp)
lw $t0, 0($t1)
sw $t0, -96($fp)
sw $t1, -92($fp)
addi $sp, $sp, -4
lw $t1, -64($fp)
addi $t0, $t1, 1
sw $t0, -100($fp)
sw $t1, -64($fp)
addi $sp, $sp, -4
lw $t1, -100($fp)
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -104($fp)
sw $t1, -100($fp)
addi $sp, $sp, -4
addi $t1, $fp, -4
lw $t2, -104($fp)
sub $t0, $t1, $t2
sw $t0, -108($fp)
sw $t2, -104($fp)
addi $sp, $sp, -4
lw $t1, -108($fp)
lw $t0, 0($t1)
sw $t0, -112($fp)
sw $t1, -108($fp)
addi $sp, $sp, -4
lw $t1, -84($fp)
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -116($fp)
sw $t1, -84($fp)
addi $sp, $sp, -4
addi $t1, $fp, -44
lw $t2, -116($fp)
sub $t0, $t1, $t2
sw $t0, -120($fp)
sw $t2, -116($fp)
lw $t0, -112($fp)
move $a1, $t0 
sw $t0, -112($fp)
lw $t0, -96($fp)
move $a0, $t0 
sw $t0, -96($fp)
addi $sp, $sp, -4
addi $sp, $sp, -8 
sw $ra, 0($sp) 
sw $fp, 4($sp) 
jal compare 
move $sp, $fp 
lw $ra, 0($sp) 
lw $fp, 4($sp) 
addi $sp, $sp, 8 
move $t0, $v0 
sw $t0, -124($fp)
lw $t0, -120($fp)
lw $t1, -124($fp)
sw $t1, 0($t0)
sw $t0, -120($fp)
sw $t1, -124($fp)
addi $sp, $sp, -4
lw $t1, -64($fp)
addi $t0, $t1, 2
sw $t0, -128($fp)
sw $t1, -64($fp)
lw $t0, -64($fp)
lw $t1, -128($fp)
move $t0, $t1
sw $t0, -64($fp)
sw $t1, -128($fp)
addi $sp, $sp, -4
lw $t1, -84($fp)
addi $t0, $t1, 1
sw $t0, -132($fp)
sw $t1, -84($fp)
lw $t0, -84($fp)
lw $t1, -132($fp)
move $t0, $t1
sw $t0, -84($fp)
sw $t1, -132($fp)
j label8
label10:
li $t0, 0
move $v0, $t0
jr $ra 
