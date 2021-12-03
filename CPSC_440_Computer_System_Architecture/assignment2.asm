# Samuel Sandoval


        .globl main
        .text
main:
        lw    $v1, celsius           # load celsius value into $t0


        addi  $t0, $zero, 9
        mult  $t0, $v1             # multiplies cels times 9, get whole result
        mflo  $t0

        addi  $t1, $zero, 5
        div   $t0, $t1                # divide result above by 5
        mflo  $t0

        
        addi  $t2, $t0, 32              # result of above + 32
	
        sw    $t2, faren              # store result into faren


        j       main                    # jump back to beginning and start over

.data
celsius: .word  32
faren: .word 0


