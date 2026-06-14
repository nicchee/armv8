ldr w0, l1 // load w0 with 0x3f20 0000 OK
ldr w1, l2 // load w1 with 0x0000 0008 (1000) OK
str w1, [x0, #0x08] // store 0x3f20 0008  0x0000 0008 let pin 21 be output OK
ldr w2, l3 // set w2 for 2 ^23 DELAY OK

ldr w1, l4 // load w1 with bit 21
str w1, [x0, #0x1c] // store 0x3f20001c with bit 21 output

// delay for 0.5 seconds
eor w10, w10, w10 
delay1:
add w10, w10, #1 
cmp w10, w2
b.ne delay1

ldr w1, l4 // load w1 with bit 21 again?? redundant? 
str w1, [x0, #0x28] // store 0x3f20 0028 with bit 21 clear

// delay
eor w10, w10, w10
delay2:
add w10, w10, #1
cmp w10, w2
b.ne delay2

and x0, x0, x0

l1:
.int 0x3f200000
l2:
.int 0x8
l3:
.int 0x800000
l4:
.int 0x200000