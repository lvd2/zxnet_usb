	DEVICE ZXSPECTRUM128

	ORG 0x8000
START  
;регистры чипа
WIZ_BASE_ADDR	= 0xc000
W_MR			= WIZ_BASE_ADDR+0x0000
W_SHAR			= WIZ_BASE_ADDR+0x0008

S0_MR		= WIZ_BASE_ADDR+0x0201 ;мл.байт
S0_CR		= WIZ_BASE_ADDR+0x0203 ;мл.байт
S0_SSR		= WIZ_BASE_ADDR+0x0209 ;мл.байт
S0_PORTR	= WIZ_BASE_ADDR+0x020a
S0_DPORTR	= WIZ_BASE_ADDR+0x0212
S0_DIPR		= WIZ_BASE_ADDR+0x0214
S0_TX_WRSR	= WIZ_BASE_ADDR+0x0222 ;мл.слово
S0_RX_RSR	= WIZ_BASE_ADDR+0x022a 
S0_TX_FAKE	= WIZ_BASE_ADDR+0x2000
S0_RX_FAKE	= WIZ_BASE_ADDR+0x3000
;/***************************************/ 
;/* The bit of Sn_MR regsiter defintion */ 
;/***************************************/ 
Sn_MR_CLOSE        = 0x00                 ;/**< Protocol bits of Sn_MR. */
Sn_MR_TCP          = 0x01                 ;/**< Protocol bits of Sn_MR. */
Sn_MR_UDP          = 0x02                 ;/**< Protocol bits of Sn_MR. */
Sn_MR_IPRAW        = 0x03                 ;/**< Protocol bits of Sn_MR. */
Sn_MR_MACRAW       = 0x04                 ;/**< Protocol bits of Sn_MR. */
Sn_MR_PPPoE        = 0x05                 ;/**< Protocol bits of Sn_MR. */

/******************************/ 
/* The values of CR defintion */ 
/******************************/
Sn_CR_OPEN         = 0x01                 ;/**< OPEN command value of Sn_CR. */
Sn_CR_LISTEN       = 0x02                 ;/**< LISTEN command value of Sn_CR. */
Sn_CR_CONNECT      = 0x04                 ;/**< CONNECT command value of Sn_CR. */
Sn_CR_DISCON       = 0x08                 ;/**< DISCONNECT command value of Sn_CR. */
Sn_CR_CLOSE        = 0x10                 ;/**< CLOSE command value of Sn_CR. */
Sn_CR_SEND         = 0x20                 ;/**< SEND command value of Sn_CR. */
Sn_CR_SEND_MAC     = 0x21                 ;/**< SEND_MAC command value of Sn_CR. */ 
Sn_CR_SEND_KEEP    = 0x22                 ;/**< SEND_KEEP command value of Sn_CR */
Sn_CR_RECV         = 0x40                 ;/**< RECV command value of Sn_CR */
Sn_CR_PCON         = 0x23                 ;/**< PCON command value of Sn_CR */
Sn_CR_PDISCON      = 0x24                 ;/**< PDISCON command value of Sn_CR */ 
Sn_CR_PCR          = 0x25                 ;/**< PCR command value of Sn_CR */
Sn_CR_PCN          = 0x26                 ;/**< PCN command value of Sn_CR */
Sn_CR_PCJ          = 0x27                 ;/**< PCJ command value of Sn_CR */

;/**********************************/ 
;/* The values of Sn_SSR defintion */ 
;/**********************************/
SOCK_CLOSED        = 0x00                 ;< SOCKETn is released */
SOCK_ARP           = 0x01                 ;< ARP-request is transmitted in order to acquire destination hardware address. */
SOCK_INIT          = 0x13                 ;< SOCKETn is open as TCP mode. */
SOCK_LISTEN        = 0x14                 ;< SOCKETn operates as "TCP SERVER" and waits for connection-request (SYN packet) from "TCP CLIENT". */
SOCK_SYNSENT       = 0x15                 ;< Connect-request(SYN packet) is transmitted to "TCP SERVER". */
SOCK_SYNRECV       = 0x16                 ;< Connect-request(SYN packet) is received from "TCP CLIENT". */
SOCK_ESTABLISHED   = 0x17                 ;< TCP connection is established. */
SOCK_FIN_WAIT      = 0x18                 ;< SOCKETn is closing. */
SOCK_CLOSING       = 0x1A                 ;< SOCKETn is closing. */
SOCK_TIME_WAIT     = 0x1B                 ;< SOCKETn is closing. */
SOCK_CLOSE_WAIT    = 0x1C                 ;< Disconnect-request(FIN packet) is received from the peer. */
SOCK_LAST_ACK      = 0x1D                 ;< SOCKETn is closing. */
SOCK_UDP           = 0x22                 ;< SOCKETn is open as UDP mode. */
SOCK_IPRAW         = 0x32                 ;< SOCKETn is open as IPRAW mode. */
SOCK_MACRAW        = 0x42                 ;< SOCKET0 is open as MACRAW mode. */
SOCK_PPPoE         = 0x5F                 ;< SOCKET0 is open as PPPoE mode. */

	di
	ld sp,0xc000
	ld hl,0x4000
	ld (hl),0
	ld de,0x4001
	ld bc,192*256/8-1
	ldir
;делаем хардресет чипа
;Все тайминги округлены до 50мс
	ei
	halt
	ld bc,0x83ab
	in a,(c)
	and 0xee
	out (c),a	; ресет
	halt
	or 0x10
	out (c),a	; снятие ресета
	halt
	
;Проецирование регистров в адресное пространство z80.
;Для примера проецируем в адреса 0xc000-0xffff (в условиях ZX-Evo baseconf).
;Проецирование будет происходить только при подключенном ПЗУ в заданном адресном окне.
;Страница ПЗУ не имеет значения
	ld a,1
	out (0xbf),a
	ld bc,0xfff7
	ld a,0x3f
	out (c),a
;Теперь включаем проецирование
	ld bc,0x82ab
	ld a,7	
	out (c),a
	
;далее софтресет чипа(насчёт нужности неуверен, но непомешает)
	ld a,0x80
	ld (W_MR+1),a
	halt
	ld a,0x3c
	ld (W_MR),a
	di
;закидываем MAC, шлюз, маску и ИП. Поменять под себя!!!
	ld hl,mac
	ld de,W_SHAR
	ld bc,20
	ldir
	;С этого момента чип будет пинговаться и настройки можно считать завершёнными.
	;Эстетам можно почитать пдфку и настроить чёнить ещё, но для примера и этого достаточно.
	
;----------------------------------------------------------
;теперь подключимся к TCP(НТТР) серверу и чегонить получим
;----------------------------------------------------------

;настроим сокет
soc_noinit
	ld a,Sn_CR_CLOSE	;закроем соединение
	ld (S0_CR),a		
	ld a,Sn_MR_TCP		;режим сокета TCP
	ld (S0_MR),a
	ld hl,(source_port)	;приращивем внутренний порт, чтоп с предыдущим не пересечся
	inc hl
	ld (source_port),hl
	ld e,h				;бигендиан, меняем байты местами
	ld d,l
	ld (S0_PORTR),de
	ld a,Sn_CR_OPEN		;даем команду открытия сокета
	ld (S0_CR),a
wait_cr0
	ld a,(S0_CR)		;ожидаем исполнения команды
	or a
	jr nz,wait_cr0
	ld a,(S0_SSR)		;если не инициализировался, то заново
	cp SOCK_INIT
	jr nz,soc_noinit
;--------------------	
;конектимся к серверу
;--------------------
	ld hl,S0_DPORTR ;порт сервера !бигендиан!
	ld (hl),0
	inc hl
	ld (hl),80
	ld hl,S0_DIPR	;ip сервера мароза1999 217.146.69.13
	ld (hl),217
	inc hl
	ld (hl),146
	inc hl
	ld (hl),69
	inc hl
	ld (hl),13
	ld a,Sn_CR_CONNECT		;даем команду конекта
	ld (S0_CR),a
wait_cr1
	ld a,(S0_CR)		;ожидаем исполнения команды
	or a
	jr nz,wait_cr1
;ожидаем коннекта с сервером
wait_con
	ld a,(S0_SSR)
	or a
	jp z,soc_noinit		;Сервер чёйта закрыл(неоткрыл) коннект
	cp SOCK_ESTABLISHED	;вообще то тут надо проверять и насчёт тайм аута, и другой фигни
	jr nz,wait_con		;но мы забьём на это дело
wait_con_end	

;приконектились, отошлём GET запрос
;--------------------
;отправка пакета
;--------------------
	ld hl,get_str		;закидываем дату в буфер чипа
	ld de,S0_TX_FAKE
	ld bc,get_str_end-get_str
	inc bc				;закидывать надо чётное количество байт
	res 0,c				;поэтому округлим до чётного
	ldir				;ахтунг говнокод!!! ldir'ить можно не более 512 байт, смотри описание платки,
						;но мы опустим этот момент, т.к. строка у нас явно короче
	ld a,high (get_str_end-get_str)
	ld (S0_TX_WRSR),a	;закидываем длинну даты, не забывая про бигэндиан
	ld a,low (get_str_end-get_str)
	ld (S0_TX_WRSR+1),a	;а вот сдесь нужно указывать точное кол-во байт, невзирая на нечётность
	
	ld a,Sn_CR_SEND	;даем команду отправки
	ld (S0_CR),a
wait_cr2
	ld a,(S0_CR)		;ожидаем исполнения команды
	or a
	jr nz,wait_cr2
	
;--------------------
;ожидаем пакет от сервера
;--------------------
	ld de,0
wait_pack0
	ld de,(S0_RX_RSR)	;читаем длинну пакета
	ld a,d
	or e				;если нулевая, то ждём дальше
	jr z,wait_pack0
wait_pack1
	ld hl,(S0_RX_RSR)	;ога, пришло
	ex de,hl			;теперь ждём устаканивания даты
	xor a				;повторно читаем и если не сходится, то читаем ещё раз длинну
	sbc hl,de
	ld a,h
	or l
	jr nz,wait_pack1
;пакет получен чипом полностью, надо его вычитать
;т.к. в S0_RX_RSR длинна кратна двум, то...

;--------------------
;сейчас прочитаем реальную длинну, она лежит в первых двух байтах буфера приема
;--------------------
	ld hl,(S0_RX_FAKE)
	ld b,l				;долбанный бигендиан
	ld c,h
;--------------------
;читаем пакет, в de у нас лежит длинна кратная двум(бигендиан)
;--------------------
;выёживатся с лдиром небудем
	ld b,e
	ld c,d
	dec bc		;в регистре длинна пакета была с учётом двух первых байт,
	dec bc		;которые добавил сам визнет(точная длинна пакета)
	ld hl,S0_RX_FAKE 
	ld de,buf_rx
loop_read
	ld l,0
	ld a,c
	or b
	jr z,end_read
	ldi
	ldi
	jr loop_read
end_read
	
	ld a,Sn_CR_RECV		;даем знать что пакет забрали
	ld (S0_CR),a
wait_cr3
	ld a,(S0_CR)		;ожидаем исполнения команды
	or a
	jr nz,wait_cr3
	
;типа всё, далее разбираем пакет, получаем следующий, отправляем. Но это уже сами, я закрываю соединение.
	
;--------------------
;закроем соединение
;--------------------
	
	ld a,Sn_CR_CLOSE	;закроем соединение
	ld (S0_CR),a	
	
;--------------------
;Всё, пример окончен.
;--------------------


	ld bc,768			;Не много по говнокодим.
	ld hl,buf_rx+478	;Эта цифра получена ваяшарком(очень рекомендую, чтоп наглядно было и познавательно)
	ld de,0x5800		;А вообщебы тут надо распарсить HTTP реквест
	ldir
	di
	halt

;конфиг сети(!!!Эти значения измените под конфигвашей сети!!!)
mac	
	db 0x00,0x08,0xDC,0x01,0x02,0x03	;цифры от балды взял, из визнетовой доки
reserved
	db 0x00,0x00	;это для того, чтоб одним лдиром закинуть
gateway
	db 192,168,0,1
mask
	db 255,255,255,0
ip
	db 192,168,0,77
	
;внутренний порт
source_port dw 0

;http://zx.maros.pri.ee/file/id/15733/filename/Yerzmyey_-_Cat_42_%282009%29_%28_International_Vodka_Party_5%2C_4%29.atr	
get_str
	db "GET /file/id/15733/filename/Yerzmyey_-_Cat_42_%282009%29_%28_International_Vodka_Party_5%2C_4%29.atr HTTP/1.1",13,10
	db "Host: zx.maros.pri.ee",13,10
	db 13,10,0
get_str_end
buf_rx
	
ENDPROG
	SAVEHOB  "s1.$C","s1.C",START,ENDPROG-START
	SAVEBIN  "s1",START,ENDPROG-START
