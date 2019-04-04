set iardir=..\utils\iar\
set sjasm=..\utils\sjasm
set iccpar=-v0 -ml -uua -q -e -K -gA -s9 -t4 -T -Llist\ -Alist\ -I../utils/iar/

%iardir%iccz80 %iccpar% zxevo_io
%iardir%iccz80 %iccpar% main
%iardir%iccz80 %iccpar% usb\hal
%iardir%iccz80 %iccpar% usb\sl811
%iardir%iccz80 %iccpar% usb\tpbulk
%iardir%az80 -uu -b -v0 sample
%iardir%az80 -uu -b -v0 font6x8
%iardir%az80 Cstartup
%iardir%xlink sample font6x8 main zxevo_io hal sl811 tpbulk -f Lnk.xcl
copy code.raw C:\Users\cash\Desktop\txt\us_w5300\aa
%sjasm% hobeta.asm
del *.r01
del list\*.lst
rem pause