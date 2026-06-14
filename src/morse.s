ldr w0, mem
ldr w1, on
str w1, [x0, #0x08]
ldr w2, ldelay50ms
ldr w3, ldelay1s
ldr w4, ldelay3s
ldr w1, l4
str w1, [x0, #0x1c]
eor w10, w10, w10
delayon00:
add w10, w10, #1
cmp w10, w2
b.ne delayon00
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff00:
add w10, w10, #1
cmp w10, w2
b.ne delayoff00
str w1, [x0, #0x1c]
eor w10, w10, w10
delayon10:
add w10, w10, #1
cmp w10, w2
b.ne delayon10
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff10:
add w10, w10, #1
cmp w10, w2
b.ne delayoff10
str w1, [x0, #0x1c]
eor w10, w10, w10
delayon20:
add w10, w10, #1
cmp w10, w2
b.ne delayon20
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff20:
add w10, w10, #1
cmp w10, w2
b.ne delayoff20
str w1, [x0, #0x1c]
eor w10, w10, w10
delayon30:
add w10, w10, #1
cmp w10, w2
b.ne delayon30
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff30:
add w10, w10, #1
cmp w10, w2
b.ne delayoff30
eor w10, w10, w10
delaybreak0:
add w10, w10, #1
cmp w10, w3
b.ne delaybreak0
str w1, [x0, #0x1c]
eor w10, w10, w10
delayon01:
add w10, w10, #1
cmp w10, w2
b.ne delayon01
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff01:
add w10, w10, #1
cmp w10, w2
b.ne delayoff01
eor w10, w10, w10
delaybreak1:
add w10, w10, #1
cmp w10, w3
b.ne delaybreak1
str w1, [x0, #0x1c]
eor w10, w10, w10
delayon02:
add w10, w10, #1
cmp w10, w2
b.ne delayon02
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff02:
add w10, w10, #1
cmp w10, w2
b.ne delayoff02
str w1, [x0, #0x1c]
eor w10, w10, w10
delay12:
add w10, w10, #1
cmp w10, w3
b.ne delay12
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff12:
add w10, w10, #1
cmp w10, w2
b.ne delayoff12
str w1, [x0, #0x1c]
eor w10, w10, w10
delayon22:
add w10, w10, #1
cmp w10, w2
b.ne delayon22
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff22:
add w10, w10, #1
cmp w10, w2
b.ne delayoff22
str w1, [x0, #0x1c]
eor w10, w10, w10
delayon32:
add w10, w10, #1
cmp w10, w2
b.ne delayon32
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff32:
add w10, w10, #1
cmp w10, w2
b.ne delayoff32
eor w10, w10, w10
delaybreak2:
add w10, w10, #1
cmp w10, w3
b.ne delaybreak2
str w1, [x0, #0x1c]
eor w10, w10, w10
delayon03:
add w10, w10, #1
cmp w10, w2
b.ne delayon03
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff03:
add w10, w10, #1
cmp w10, w2
b.ne delayoff03
str w1, [x0, #0x1c]
eor w10, w10, w10
delay13:
add w10, w10, #1
cmp w10, w3
b.ne delay13
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff13:
add w10, w10, #1
cmp w10, w2
b.ne delayoff13
str w1, [x0, #0x1c]
eor w10, w10, w10
delayon23:
add w10, w10, #1
cmp w10, w2
b.ne delayon23
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff23:
add w10, w10, #1
cmp w10, w2
b.ne delayoff23
str w1, [x0, #0x1c]
eor w10, w10, w10
delayon33:
add w10, w10, #1
cmp w10, w2
b.ne delayon33
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff33:
add w10, w10, #1
cmp w10, w2
b.ne delayoff33
eor w10, w10, w10
delaybreak3:
add w10, w10, #1
cmp w10, w3
b.ne delaybreak3
str w1, [x0, #0x1c]
eor w10, w10, w10
delay04:
add w10, w10, #1
cmp w10, w3
b.ne delay04
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff04:
add w10, w10, #1
cmp w10, w2
b.ne delayoff04
str w1, [x0, #0x1c]
eor w10, w10, w10
delay14:
add w10, w10, #1
cmp w10, w3
b.ne delay14
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff14:
add w10, w10, #1
cmp w10, w2
b.ne delayoff14
str w1, [x0, #0x1c]
eor w10, w10, w10
delay24:
add w10, w10, #1
cmp w10, w3
b.ne delay24
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff24:
add w10, w10, #1
cmp w10, w2
b.ne delayoff24
eor w10, w10, w10
delaybreak4:
add w10, w10, #1
cmp w10, w3
b.ne delaybreak4
eor w10, w10, w10
delayspace5:
add w10, w10, #1
cmp w10, w4
b.ne delayspace5
str w1, [x0, #0x1c]
eor w10, w10, w10
delayon06:
add w10, w10, #1
cmp w10, w2
b.ne delayon06
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff06:
add w10, w10, #1
cmp w10, w2
b.ne delayoff06
str w1, [x0, #0x1c]
eor w10, w10, w10
delay16:
add w10, w10, #1
cmp w10, w3
b.ne delay16
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff16:
add w10, w10, #1
cmp w10, w2
b.ne delayoff16
eor w10, w10, w10
delaybreak6:
add w10, w10, #1
cmp w10, w3
b.ne delaybreak6
str w1, [x0, #0x1c]
eor w10, w10, w10
delay07:
add w10, w10, #1
cmp w10, w3
b.ne delay07
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff07:
add w10, w10, #1
cmp w10, w2
b.ne delayoff07
str w1, [x0, #0x1c]
eor w10, w10, w10
delayon17:
add w10, w10, #1
cmp w10, w2
b.ne delayon17
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff17:
add w10, w10, #1
cmp w10, w2
b.ne delayoff17
str w1, [x0, #0x1c]
eor w10, w10, w10
delayon27:
add w10, w10, #1
cmp w10, w2
b.ne delayon27
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff27:
add w10, w10, #1
cmp w10, w2
b.ne delayoff27
str w1, [x0, #0x1c]
eor w10, w10, w10
delayon37:
add w10, w10, #1
cmp w10, w2
b.ne delayon37
str w1, [x0, #0x28]
eor w10, w10, w10
delayoff37:
add w10, w10, #1
cmp w10, w2
b.ne delayoff37
eor w10, w10, w10
delaybreak7:
add w10, w10, #1
cmp w10, w3
b.ne delaybreak7
eor w10, w10, w10
delayspace8:
add w10, w10, #1
cmp w10, w4
b.ne delayspace8
halt:
and x0, x0, x0
mem:
.int 0x3f200000
on:
.int 0x8
ldelay50ms:
.int 0x200000
ldelay1s:
.int 0x800000
ldelay3s:
.int 0x1800000
off:
.int 0x200000
l4:
.int 0x200000
