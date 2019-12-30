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

process: 
addi $fp, $sp, 0
addi $sp, $sp, -4
move $t0, $a0 
sw $t0, -4($fp)
addi $sp, $sp, -4
li $t0, 3
sw $t0, -8($fp)
addi $sp, $sp, -4
li $t0, 31
sw $t0, -12($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
addi $t0, $t1, 5
sw $t0, -16($fp)
sw $t1, -12($fp)
lw $t0, -8($fp)
lw $t1, -16($fp)
move $t0, $t1
sw $t0, -8($fp)
sw $t1, -16($fp)
addi $sp, $sp, -4
lw $t1, -4($fp)
li $t2, 321
mul $t0, $t1, $t2
sw $t0, -20($fp)
sw $t1, -4($fp)
addi $sp, $sp, -4
lw $t1, -20($fp)
li $t2, 2
mul $t0, $t1, $t2
sw $t0, -24($fp)
sw $t1, -20($fp)
addi $sp, $sp, -4
lw $t1, -4($fp)
lw $t2, -8($fp)
mul $t0, $t1, $t2
sw $t0, -28($fp)
sw $t1, -4($fp)
sw $t2, -8($fp)
addi $sp, $sp, -4
lw $t1, -24($fp)
lw $t2, -28($fp)
add $t0, $t1, $t2
sw $t0, -32($fp)
sw $t1, -24($fp)
sw $t2, -28($fp)
addi $sp, $sp, -4
lw $t1, -32($fp)
lw $t2, -4($fp)
sub $t0, $t1, $t2
sw $t0, -36($fp)
sw $t1, -32($fp)
sw $t2, -4($fp)
addi $sp, $sp, -4
lw $t1, -8($fp)
lw $t2, -4($fp)
mul $t0, $t1, $t2
sw $t0, -40($fp)
sw $t1, -8($fp)
sw $t2, -4($fp)
addi $sp, $sp, -4
lw $t1, -36($fp)
lw $t2, -40($fp)
add $t0, $t1, $t2
sw $t0, -44($fp)
sw $t1, -36($fp)
sw $t2, -40($fp)
addi $sp, $sp, -4
lw $t1, -8($fp)
lw $t2, -8($fp)
mul $t0, $t1, $t2
sw $t0, -48($fp)
sw $t1, -8($fp)
sw $t2, -8($fp)
addi $sp, $sp, -4
lw $t1, -44($fp)
lw $t2, -48($fp)
add $t0, $t1, $t2
sw $t0, -52($fp)
sw $t1, -44($fp)
sw $t2, -48($fp)
addi $sp, $sp, -4
lw $t1, -52($fp)
lw $t2, -4($fp)
add $t0, $t1, $t2
sw $t0, -56($fp)
sw $t1, -52($fp)
sw $t2, -4($fp)
addi $sp, $sp, -4
lw $t1, -56($fp)
lw $t2, -4($fp)
add $t0, $t1, $t2
sw $t0, -60($fp)
sw $t1, -56($fp)
sw $t2, -4($fp)
addi $sp, $sp, -4
lw $t1, -60($fp)
addi $t0, $t1, -23
sw $t0, -64($fp)
sw $t1, -60($fp)
addi $sp, $sp, -4
lw $t1, -64($fp)
addi $t0, $t1, 45
sw $t0, -68($fp)
sw $t1, -64($fp)
lw $t0, -8($fp)
lw $t1, -68($fp)
move $t0, $t1
sw $t0, -8($fp)
sw $t1, -68($fp)
addi $sp, $sp, -4
lw $t1, -8($fp)
li $t2, 3
div $t1, $t2
mflo $t0
sw $t0, -72($fp)
sw $t1, -8($fp)
addi $sp, $sp, -4
lw $t1, -72($fp)
addi $t0, $t1, 336
sw $t0, -76($fp)
sw $t1, -72($fp)
addi $sp, $sp, -4
lw $t1, -4($fp)
li $t2, 12
mul $t0, $t1, $t2
sw $t0, -80($fp)
sw $t1, -4($fp)
addi $sp, $sp, -4
lw $t1, -80($fp)
li $t2, 4
div $t1, $t2
mflo $t0
sw $t0, -84($fp)
sw $t1, -80($fp)
addi $sp, $sp, -4
lw $t1, -76($fp)
lw $t2, -84($fp)
sub $t0, $t1, $t2
sw $t0, -88($fp)
sw $t1, -76($fp)
sw $t2, -84($fp)
addi $sp, $sp, -4
lw $t1, -88($fp)
addi $t0, $t1, -60
sw $t0, -92($fp)
sw $t1, -88($fp)
addi $sp, $sp, -4
lw $t1, -8($fp)
li $t2, 12
div $t1, $t2
mflo $t0
sw $t0, -96($fp)
sw $t1, -8($fp)
addi $sp, $sp, -4
lw $t1, -96($fp)
li $t2, 24
mul $t0, $t1, $t2
sw $t0, -100($fp)
sw $t1, -96($fp)
addi $sp, $sp, -4
lw $t1, -92($fp)
lw $t2, -100($fp)
add $t0, $t1, $t2
sw $t0, -104($fp)
sw $t1, -92($fp)
sw $t2, -100($fp)
addi $sp, $sp, -4
lw $t1, -104($fp)
addi $t0, $t1, 36
sw $t0, -108($fp)
sw $t1, -104($fp)
addi $sp, $sp, -4
lw $t1, -108($fp)
addi $t0, $t1, 1
sw $t0, -112($fp)
sw $t1, -108($fp)
lw $t0, -8($fp)
lw $t1, -112($fp)
move $t0, $t1
sw $t0, -8($fp)
sw $t1, -112($fp)
addi $sp, $sp, -4
lw $t1, -4($fp)
addi $t0, $t1, 24
sw $t0, -116($fp)
sw $t1, -4($fp)
addi $sp, $sp, -4
lw $t1, -116($fp)
addi $t0, $t1, 1
sw $t0, -120($fp)
sw $t1, -116($fp)
lw $t0, -8($fp)
lw $t1, -120($fp)
move $t0, $t1
sw $t0, -8($fp)
sw $t1, -120($fp)
lw $t0, -8($fp)
move $v0, $t0
jr $ra 
sw $t0, -8($fp)
main: 
addi $fp, $sp, 0
addi $sp, $sp, -4
li $t0, 16
sw $t0, -4($fp)
addi $sp, $sp, -4
lw $t1, -4($fp)
addi $t0, $t1, -3
sw $t0, -8($fp)
sw $t1, -4($fp)
addi $sp, $sp, -4
lw $t1, -8($fp)
move $t0, $t1
sw $t0, -12($fp)
sw $t1, -8($fp)
addi $sp, $sp, -4
li $t1, 35
li $t2, 2
div $t1, $t2
mflo $t0
sw $t0, -16($fp)
addi $sp, $sp, -4
lw $t1, -16($fp)
addi $t0, $t1, 3
sw $t0, -20($fp)
sw $t1, -16($fp)
addi $sp, $sp, -4
lw $t1, -20($fp)
move $t0, $t1
sw $t0, -24($fp)
sw $t1, -20($fp)
addi $sp, $sp, -4
li $t0, 15
sw $t0, -28($fp)
addi $sp, $sp, -4
lw $t1, -28($fp)
addi $t0, $t1, -0
sw $t0, -32($fp)
sw $t1, -28($fp)
addi $sp, $sp, -4
lw $t1, -32($fp)
move $t0, $t1
sw $t0, -36($fp)
sw $t1, -32($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
lw $t2, -24($fp)
add $t0, $t1, $t2
sw $t0, -40($fp)
sw $t1, -12($fp)
sw $t2, -24($fp)
addi $sp, $sp, -4
lw $t1, -40($fp)
lw $t2, -36($fp)
add $t0, $t1, $t2
sw $t0, -44($fp)
sw $t1, -40($fp)
sw $t2, -36($fp)
addi $sp, $sp, -4
lw $t1, -44($fp)
move $t0, $t1
sw $t0, -48($fp)
sw $t1, -44($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
lw $t2, -24($fp)
mul $t0, $t1, $t2
sw $t0, -52($fp)
sw $t1, -12($fp)
sw $t2, -24($fp)
addi $sp, $sp, -4
lw $t1, -36($fp)
li $t2, 2
div $t1, $t2
mflo $t0
sw $t0, -56($fp)
sw $t1, -36($fp)
addi $sp, $sp, -4
lw $t1, -52($fp)
lw $t2, -56($fp)
add $t0, $t1, $t2
sw $t0, -60($fp)
sw $t1, -52($fp)
sw $t2, -56($fp)
addi $sp, $sp, -4
lw $t1, -60($fp)
move $t0, $t1
sw $t0, -64($fp)
sw $t1, -60($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
lw $t2, -24($fp)
sub $t0, $t1, $t2
sw $t0, -68($fp)
sw $t1, -12($fp)
sw $t2, -24($fp)
addi $sp, $sp, -4
lw $t1, -68($fp)
lw $t2, -36($fp)
sub $t0, $t1, $t2
sw $t0, -72($fp)
sw $t1, -68($fp)
sw $t2, -36($fp)
addi $sp, $sp, -4
lw $t1, -72($fp)
move $t0, $t1
sw $t0, -76($fp)
sw $t1, -72($fp)
addi $sp, $sp, -4
li $t0, 42
sw $t0, -80($fp)
addi $sp, $sp, -4
li $t0, 0
sw $t0, -84($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
lw $t2, -24($fp)
add $t0, $t1, $t2
sw $t0, -88($fp)
sw $t1, -12($fp)
sw $t2, -24($fp)
addi $sp, $sp, -4
lw $t1, -88($fp)
lw $t2, -36($fp)
add $t0, $t1, $t2
sw $t0, -92($fp)
sw $t1, -88($fp)
sw $t2, -36($fp)
addi $sp, $sp, -4
lw $t1, -92($fp)
addi $t0, $t1, 2000
sw $t0, -96($fp)
sw $t1, -92($fp)
addi $sp, $sp, -4
lw $t1, -96($fp)
lw $t2, -76($fp)
sub $t0, $t1, $t2
sw $t0, -100($fp)
sw $t1, -96($fp)
sw $t2, -76($fp)
lw $t0, -76($fp)
lw $t1, -100($fp)
move $t0, $t1
sw $t0, -76($fp)
sw $t1, -100($fp)
label1:
addi $sp, $sp, -4
lw $t1, -12($fp)
lw $t2, -24($fp)
add $t0, $t1, $t2
sw $t0, -104($fp)
sw $t1, -12($fp)
sw $t2, -24($fp)
lw $t0, -104($fp)
lw $t1, -76($fp)
blt $t0, $t1, label2
sw $t0, -104($fp)
sw $t1, -76($fp)
j label3
label2:
addi $sp, $sp, -4
lw $t1, -84($fp)
li $t2, 12
mul $t0, $t1, $t2
sw $t0, -108($fp)
sw $t1, -84($fp)
addi $sp, $sp, -4
lw $t1, -80($fp)
lw $t2, -108($fp)
add $t0, $t1, $t2
sw $t0, -112($fp)
sw $t1, -80($fp)
sw $t2, -108($fp)
addi $sp, $sp, -4
lw $t1, -112($fp)
addi $t0, $t1, 4
sw $t0, -116($fp)
sw $t1, -112($fp)
addi $sp, $sp, -4
lw $t1, -116($fp)
addi $t0, $t1, 5
sw $t0, -120($fp)
sw $t1, -116($fp)
addi $sp, $sp, -4
lw $t1, -120($fp)
addi $t0, $t1, 2
sw $t0, -124($fp)
sw $t1, -120($fp)
lw $t0, -80($fp)
lw $t1, -124($fp)
move $t0, $t1
sw $t0, -80($fp)
sw $t1, -124($fp)
addi $sp, $sp, -4
lw $t1, -76($fp)
move $t0, $t1
sw $t0, -128($fp)
sw $t1, -76($fp)
lw $t0, -128($fp)
move $a0, $t0 
sw $t0, -128($fp)
addi $sp, $sp, -4
addi $sp, $sp, -8 
sw $ra, 0($sp) 
sw $fp, 4($sp) 
jal process 
move $sp, $fp 
lw $ra, 0($sp) 
lw $fp, 4($sp) 
addi $sp, $sp, 8 
move $t0, $v0 
sw $t0, -132($fp)
addi $sp, $sp, -4
li $t1, 2
lw $t2, -12($fp)
mul $t0, $t1, $t2
sw $t0, -136($fp)
sw $t2, -12($fp)
addi $sp, $sp, -4
lw $t1, -132($fp)
lw $t2, -136($fp)
add $t0, $t1, $t2
sw $t0, -140($fp)
sw $t1, -132($fp)
sw $t2, -136($fp)
addi $sp, $sp, -4
lw $t1, -140($fp)
lw $t2, -76($fp)
sub $t0, $t1, $t2
sw $t0, -144($fp)
sw $t1, -140($fp)
sw $t2, -76($fp)
addi $sp, $sp, -4
lw $t1, -36($fp)
lw $t2, -48($fp)
mul $t0, $t1, $t2
sw $t0, -148($fp)
sw $t1, -36($fp)
sw $t2, -48($fp)
addi $sp, $sp, -4
lw $t1, -144($fp)
lw $t2, -148($fp)
add $t0, $t1, $t2
sw $t0, -152($fp)
sw $t1, -144($fp)
sw $t2, -148($fp)
addi $sp, $sp, -4
lw $t1, -152($fp)
move $t0, $t1
sw $t0, -156($fp)
sw $t1, -152($fp)
addi $sp, $sp, -4
lw $t1, -84($fp)
lw $t2, -84($fp)
add $t0, $t1, $t2
sw $t0, -160($fp)
sw $t1, -84($fp)
sw $t2, -84($fp)
lw $t0, -84($fp)
lw $t1, -160($fp)
move $t0, $t1
sw $t0, -84($fp)
sw $t1, -160($fp)
addi $sp, $sp, -4
lw $t1, -84($fp)
lw $t2, -84($fp)
add $t0, $t1, $t2
sw $t0, -164($fp)
sw $t1, -84($fp)
sw $t2, -84($fp)
lw $t0, -84($fp)
lw $t1, -164($fp)
move $t0, $t1
sw $t0, -84($fp)
sw $t1, -164($fp)
addi $sp, $sp, -4
lw $t1, -84($fp)
lw $t2, -84($fp)
add $t0, $t1, $t2
sw $t0, -168($fp)
sw $t1, -84($fp)
sw $t2, -84($fp)
lw $t0, -84($fp)
lw $t1, -168($fp)
move $t0, $t1
sw $t0, -84($fp)
sw $t1, -168($fp)
addi $sp, $sp, -4
lw $t1, -84($fp)
lw $t2, -84($fp)
add $t0, $t1, $t2
sw $t0, -172($fp)
sw $t1, -84($fp)
sw $t2, -84($fp)
lw $t0, -84($fp)
lw $t1, -172($fp)
move $t0, $t1
sw $t0, -84($fp)
sw $t1, -172($fp)
addi $sp, $sp, -4
lw $t1, -84($fp)
lw $t2, -84($fp)
add $t0, $t1, $t2
sw $t0, -176($fp)
sw $t1, -84($fp)
sw $t2, -84($fp)
lw $t0, -84($fp)
lw $t1, -176($fp)
move $t0, $t1
sw $t0, -84($fp)
sw $t1, -176($fp)
addi $sp, $sp, -4
lw $t1, -84($fp)
addi $t0, $t1, 3
sw $t0, -180($fp)
sw $t1, -84($fp)
addi $sp, $sp, -4
lw $t1, -180($fp)
move $t0, $t1
sw $t0, -184($fp)
sw $t1, -180($fp)
addi $sp, $sp, -4
lw $t1, -184($fp)
addi $t0, $t1, -1
sw $t0, -188($fp)
sw $t1, -184($fp)
lw $t0, -184($fp)
lw $t1, -188($fp)
move $t0, $t1
sw $t0, -184($fp)
sw $t1, -188($fp)
addi $sp, $sp, -4
lw $t1, -184($fp)
addi $t0, $t1, 3
sw $t0, -192($fp)
sw $t1, -184($fp)
lw $t0, -184($fp)
lw $t1, -192($fp)
move $t0, $t1
sw $t0, -184($fp)
sw $t1, -192($fp)
addi $sp, $sp, -4
lw $t1, -184($fp)
addi $t0, $t1, -6
sw $t0, -196($fp)
sw $t1, -184($fp)
lw $t0, -184($fp)
lw $t1, -196($fp)
move $t0, $t1
sw $t0, -184($fp)
sw $t1, -196($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
move $t0, $t1
sw $t0, -200($fp)
sw $t1, -12($fp)
lw $t0, -200($fp)
move $a0, $t0 
sw $t0, -200($fp)
addi $sp, $sp, -4
addi $sp, $sp, -8 
sw $ra, 0($sp) 
sw $fp, 4($sp) 
jal process 
move $sp, $fp 
lw $ra, 0($sp) 
lw $fp, 4($sp) 
addi $sp, $sp, 8 
move $t0, $v0 
sw $t0, -204($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
addi $t0, $t1, 3
sw $t0, -208($fp)
sw $t1, -12($fp)
addi $sp, $sp, -4
lw $t1, -208($fp)
addi $t0, $t1, -2
sw $t0, -212($fp)
sw $t1, -208($fp)
addi $sp, $sp, -4
lw $t1, -212($fp)
addi $t0, $t1, -1
sw $t0, -216($fp)
sw $t1, -212($fp)
addi $sp, $sp, -4
lw $t1, -216($fp)
move $t0, $t1
sw $t0, -220($fp)
sw $t1, -216($fp)
lw $t0, -220($fp)
move $a0, $t0 
sw $t0, -220($fp)
addi $sp, $sp, -4
addi $sp, $sp, -8 
sw $ra, 0($sp) 
sw $fp, 4($sp) 
jal process 
move $sp, $fp 
lw $ra, 0($sp) 
lw $fp, 4($sp) 
addi $sp, $sp, 8 
move $t0, $v0 
sw $t0, -224($fp)
lw $t0, -204($fp)
lw $t1, -224($fp)
beq $t0, $t1, label4
sw $t0, -204($fp)
sw $t1, -224($fp)
j label5
label4:
addi $sp, $sp, -4
lw $t1, -76($fp)
addi $t0, $t1, -2
sw $t0, -228($fp)
sw $t1, -76($fp)
addi $sp, $sp, -4
lw $t1, -228($fp)
addi $t0, $t1, 1
sw $t0, -232($fp)
sw $t1, -228($fp)
lw $t0, -76($fp)
lw $t1, -232($fp)
move $t0, $t1
sw $t0, -76($fp)
sw $t1, -232($fp)
label5:
addi $sp, $sp, -4
lw $t1, -12($fp)
addi $t0, $t1, 2
sw $t0, -236($fp)
sw $t1, -12($fp)
addi $sp, $sp, -4
lw $t1, -236($fp)
addi $t0, $t1, 1
sw $t0, -240($fp)
sw $t1, -236($fp)
lw $t0, -12($fp)
lw $t1, -240($fp)
move $t0, $t1
sw $t0, -12($fp)
sw $t1, -240($fp)
j label1
label3:
addi $sp, $sp, -4
lw $t1, -80($fp)
addi $t0, $t1, -12
sw $t0, -244($fp)
sw $t1, -80($fp)
lw $t0, -184($fp)
lw $t1, -244($fp)
move $t0, $t1
sw $t0, -184($fp)
sw $t1, -244($fp)
label6:
lw $t0, -184($fp)
lw $t1, -80($fp)
blt $t0, $t1, label7
sw $t0, -184($fp)
sw $t1, -80($fp)
j label8
label7:
addi $sp, $sp, -4
li $t0, 58
sw $t0, -248($fp)
addi $sp, $sp, -4
lw $t1, -248($fp)
lw $t2, -12($fp)
add $t0, $t1, $t2
sw $t0, -252($fp)
sw $t1, -248($fp)
sw $t2, -12($fp)
lw $t0, -76($fp)
lw $t1, -252($fp)
move $t0, $t1
sw $t0, -76($fp)
sw $t1, -252($fp)
addi $sp, $sp, -4
lw $t1, -80($fp)
addi $t0, $t1, -12
sw $t0, -256($fp)
sw $t1, -80($fp)
lw $t0, -156($fp)
lw $t1, -256($fp)
move $t0, $t1
sw $t0, -156($fp)
sw $t1, -256($fp)
addi $sp, $sp, -4
lw $t1, -184($fp)
addi $t0, $t1, 1
sw $t0, -260($fp)
sw $t1, -184($fp)
lw $t0, -184($fp)
lw $t1, -260($fp)
move $t0, $t1
sw $t0, -184($fp)
sw $t1, -260($fp)
lw $t0, -156($fp)
lw $t1, -80($fp)
move $t0, $t1
sw $t0, -156($fp)
sw $t1, -80($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
lw $t2, -24($fp)
add $t0, $t1, $t2
sw $t0, -264($fp)
sw $t1, -12($fp)
sw $t2, -24($fp)
lw $t0, -84($fp)
lw $t1, -264($fp)
move $t0, $t1
sw $t0, -84($fp)
sw $t1, -264($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
lw $t2, -24($fp)
add $t0, $t1, $t2
sw $t0, -268($fp)
sw $t1, -12($fp)
sw $t2, -24($fp)
lw $t0, -36($fp)
lw $t1, -268($fp)
move $t0, $t1
sw $t0, -36($fp)
sw $t1, -268($fp)
j label6
label8:
lw $t0, -76($fp)
move $a0, $t0 
addi $sp, $sp, -4 
sw $ra, 0($sp) 
jal write  
lw $ra, 0($sp) 
addi $sp, $sp, 4  
sw $t0, -76($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
lw $t2, -24($fp)
add $t0, $t1, $t2
sw $t0, -272($fp)
sw $t1, -12($fp)
sw $t2, -24($fp)
lw $t0, -12($fp)
lw $t1, -272($fp)
move $t0, $t1
sw $t0, -12($fp)
sw $t1, -272($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
lw $t2, -24($fp)
add $t0, $t1, $t2
sw $t0, -276($fp)
sw $t1, -12($fp)
sw $t2, -24($fp)
lw $t0, -24($fp)
lw $t1, -276($fp)
move $t0, $t1
sw $t0, -24($fp)
sw $t1, -276($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
lw $t2, -24($fp)
add $t0, $t1, $t2
sw $t0, -280($fp)
sw $t1, -12($fp)
sw $t2, -24($fp)
lw $t0, -36($fp)
lw $t1, -280($fp)
move $t0, $t1
sw $t0, -36($fp)
sw $t1, -280($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
lw $t2, -24($fp)
add $t0, $t1, $t2
sw $t0, -284($fp)
sw $t1, -12($fp)
sw $t2, -24($fp)
lw $t0, -76($fp)
lw $t1, -284($fp)
move $t0, $t1
sw $t0, -76($fp)
sw $t1, -284($fp)
addi $sp, $sp, -4
lw $t1, -12($fp)
lw $t2, -24($fp)
add $t0, $t1, $t2
sw $t0, -288($fp)
sw $t1, -12($fp)
sw $t2, -24($fp)
lw $t0, -156($fp)
lw $t1, -288($fp)
move $t0, $t1
sw $t0, -156($fp)
sw $t1, -288($fp)
addi $sp, $sp, -4
lw $t1, -36($fp)
lw $t2, -76($fp)
add $t0, $t1, $t2
sw $t0, -292($fp)
sw $t1, -36($fp)
sw $t2, -76($fp)
addi $sp, $sp, -4
lw $t1, -292($fp)
lw $t2, -156($fp)
add $t0, $t1, $t2
sw $t0, -296($fp)
sw $t1, -292($fp)
sw $t2, -156($fp)
lw $t0, -296($fp)
move $a0, $t0 
addi $sp, $sp, -4 
sw $ra, 0($sp) 
jal write  
lw $ra, 0($sp) 
addi $sp, $sp, 4  
sw $t0, -296($fp)
li $t0, 0
move $v0, $t0
jr $ra 
