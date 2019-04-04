..\z80\bin\iccz80 -v0 -ml -uua -b -q -x -K -gA -z9 -t4 -T -Llist\ -Alist\ -I"../z80/inc/" mylib.c 
..\z80\bin\az80 -uu -b -v0 graf.s01
..\z80\bin\az80 -uu -b -v0 myim2.s01
..\z80\bin\xlib mylib.xlib
pause