ldr w0, l1
ldr w1, l2
str w1, [x0, #0x08]
ldr w2, l3

loop:
ldr w1, l4
str w1, [x0, #0x1c]

eor w10, w10, w10
delay1:
add w10, w10, #1
cmp w10, w2
b.ne delay1

ldr w1, l4
str w1, [x0, #0x28]

eor w10, w10, w10
delay2:
add w10, w10, #1
cmp w10, w2
b.ne delay2

b loop

halt:
and x0, x0, x0

l1:
.int 0x3f200000
l2:
.int 0x8
l3:
.int 0x400000
l4:
.int 0x200000
