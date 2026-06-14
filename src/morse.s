ldr w0, mem
ldr w1, on
str w1, [x0, #0x08]
ldr w2, ldelay50ms
ldr w3, ldelay1s
ldr w4, ldelay3s
ldr w1, l4
str w1, [x0, #0x1c]
eor w10, w10, w10
delay50ms:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w2
b.ne delay50ms
str w1, [x0, #0x1c]
str w1, [x0, #0x1c]
eor w10, w10, w10
delay1s:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w3
b.ne delay1s
str w1, [x0, #0x1c]
eor w10, w10, w10
delay1s:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w3
b.ne delay1s
str w1, [x0, #0x1c]
eor w10, w10, w10
delay1s:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w3
b.ne delay1s
str w1, [x0, #0x1c]
str w1, [x0, #0x1c]
eor w10, w10, w10
delay50ms:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w2
b.ne delay50ms
str w1, [x0, #0x1c]
str w1, [x0, #0x1c]
eor w10, w10, w10
delay50ms:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w2
b.ne delay50ms
str w1, [x0, #0x1c]
str w1, [x0, #0x1c]
eor w10, w10, w10
delay50ms:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w2
b.ne delay50ms
str w1, [x0, #0x1c]
eor w10, w10, w10
delay1s:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w3
b.ne delay1s
eor w10, w10, w10
delay3s:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w4
b.ne delay3s
str w1, [x0, #0x1c]
eor w10, w10, w10
delay1s:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w3
b.ne delay1s
str w1, [x0, #0x1c]
str w1, [x0, #0x1c]
eor w10, w10, w10
delay50ms:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w2
b.ne delay50ms
str w1, [x0, #0x1c]
str w1, [x0, #0x1c]
eor w10, w10, w10
delay1s:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w3
b.ne delay1s
str w1, [x0, #0x1c]
str w1, [x0, #0x1c]
eor w10, w10, w10
delay50ms:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w2
b.ne delay50ms
str w1, [x0, #0x1c]
eor w10, w10, w10
delay1s:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w3
b.ne delay1s
str w1, [x0, #0x1c]
eor w10, w10, w10
delay1s:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w3
b.ne delay1s
str w1, [x0, #0x1c]
str w1, [x0, #0x1c]
eor w10, w10, w10
delay50ms:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w2
b.ne delay50ms
str w1, [x0, #0x1c]
str w1, [x0, #0x1c]
eor w10, w10, w10
delay50ms:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w2
b.ne delay50ms
str w1, [x0, #0x1c]
eor w10, w10, w10
delay1s:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w3
b.ne delay1s
str w1, [x0, #0x1c]
eor w10, w10, w10
delay50ms:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w2
b.ne delay50ms
str w1, [x0, #0x1c]
eor w10, w10, w10
delay1s:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w3
b.ne delay1s
eor w10, w10, w10
delay3s:
eor w10, w10, w10
add w10, w10, #1
cmp w10, w4
b.ne delay3s
halt:
and x0, x0, x0
mem:
.int 0x3f200000
on:
.int 0x8
ldelay50ms:
.int 0x400000
ldelay1s:
.int 0x800000
ldelay3s:
.int 0x1800000
off:
.int 0x200000
