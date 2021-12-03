        .globl main
        .text
main:
        lw      $t0,value1              # load 8 into $t0 (register $8)
        lw      $t1,value2              # load 1 into $t1 (register $9)
        sub     $t2,$t0,$t1             # sub 8 - 1, store into $t2 (register $10)
        sw      $t2,summ                # store result into memory 
        j       main                    # jump back to beginning and start over

        .data
value1: .word   8
value2: .word   1
summ:   .word   0               

