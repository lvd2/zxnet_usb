// ZXiznet project
// (c) NedoPC 2012-2018
//
// top-level module

module top
(
	input  wire        fclk, // 48MHz clock
	
	output reg         usb_clk, // 12MHz clock for USB chip (sl811)
	
	// zxbus
	input  wire [15:0] za,
	inout  wire [ 7:0] zd,
	
	// buffered data bus for chips
	inout  wire [ 7:0] bd,

	// zxbus control signals
	input  wire        ziorq_n,
	input  wire        zrd_n,
	input  wire        zwr_n,
	input  wire        zmreq_n,
	output wire        ziorqge,
	output wire        zblkrom,
	input  wire        zcsrom_n,
	input  wire        zrst_n,
	output wire        zint_n,


	// buffered RD_N and WR_N for chips
	output wire        brd_n,
	output wire        bwr_n,


	// w5300 Ethernet chip
	output wire        w5300_rst_n,
	output wire [ 9:0] w5300_addr,
	output wire        w5300_cs_n,
	input  wire        w5300_int_n,


	// sl811 Usb chip
	output wire        sl811_rst_n,
	input  wire        sl811_intrq,
	output wire        sl811_ms_n,
	output wire        sl811_cs_n,
	output wire        sl811_a0,

	// usb power presence
	input  wire        usb_power

	// = total 56 pins (maximum is 66)
	//   among them 27 outputs (maximum is 64)
);


	reg usb_ckreg; // for simulation


	// CLOCKED_FILTER for brd/bwr:
	// just resync for brd, resync + 6ck max for bwr
	reg pre_brd_n;

	reg bwr_n_r;
	reg bwr_n_rr;

	reg [4:0] shreg;



	wire ena_w5300_int;
	wire ena_sl811_int;
	wire ena_zxbus_int;
	wire internal_int;

	wire [7:0] ports_wrdata;
	wire [7:0] ports_rddata;
	wire [1:0] ports_addr;
	wire       ports_wrena;
	wire       ports_wrstb_n;

	wire [1:0] rommap_win;
	wire       rommap_ena;

	wire       w5300_a0inv;
	wire       w5300_ports;
	wire [3:0] w5300_hi;

	wire [9:0] pre_w5300_addr;



	// USB chip clock
	initial // for simulation
	begin
		usb_ckreg = 1'b0;
		usb_clk   = 1'b0;
	end
	//
	always @(posedge fclk)
	begin
		usb_ckreg <=  usb_clk;
		usb_clk   <= ~usb_ckreg;
	end




	// zx-bus
	zbus zbus
	(
		.fclk(fclk),

		.za(za),
		.zd(zd),
		//
		.bd(bd),
		//
		.ziorq_n (ziorq_n ),
		.zrd_n   (zrd_n   ),
		.zwr_n   (zwr_n   ),
		.zmreq_n (zmreq_n ),
		.ziorqge (ziorqge ),
		.zblkrom (zblkrom ),
		.zcsrom_n(zcsrom_n),
		.zrst_n  (zrst_n  ),
		//
		.ports_wrena  (ports_wrena  ),
		.ports_wrstb_n(ports_wrstb_n),
		.ports_addr   (ports_addr   ),
		.ports_wrdata (ports_wrdata ),
		.ports_rddata (ports_rddata ),
		//
		.rommap_win(rommap_win),
		.rommap_ena(rommap_ena),
		//
		.sl811_cs_n(sl811_cs_n),
		.sl811_a0  (sl811_a0  ),
		//
		.w5300_cs_n (w5300_cs_n ),
		.w5300_ports(w5300_ports),
		//
		.async_w5300_addr(pre_w5300_addr),
		.w5300_addr      (w5300_addr    ),
		//
		.brd_n(brd_n),
		.bwr_n(bwr_n)
	);


	// map Z80 space to wiznet space
	wizmap wizmap
	(
		.za(za),

		.w5300_a0inv(w5300_a0inv   ),
		.w5300_addr (pre_w5300_addr),
		.w5300_ports(w5300_ports   ),
		.w5300_hi   (w5300_hi      )
	);


	// ports
	ports ports
	(
		.rst_n(zrst_n),
		//
		.wrstb_n(ports_wrstb_n),
		.wrena  (ports_wrena  ),
		.addr   (ports_addr   ),
		.wrdata (ports_wrdata ),
		.rddata (ports_rddata ),
		//
		.ena_w5300_int(ena_w5300_int),
		.ena_sl811_int(ena_sl811_int),
		.ena_zxbus_int(ena_zxbus_int),
		//
		.w5300_int_n(w5300_int_n),
		.sl811_intrq(sl811_intrq),
		//
		.internal_int(internal_int),
		//
		.rommap_win(rommap_win),
		.rommap_ena(rommap_ena),
		//
		.w5300_a0inv(w5300_a0inv),
		.w5300_rst_n(w5300_rst_n),
		.w5300_ports(w5300_ports),
		.w5300_hi   (w5300_hi   ),
		//
		.sl811_ms_n (sl811_ms_n ),
		.sl811_rst_n(sl811_rst_n),
		//
		.usb_power(usb_power)
	);


/*	// brd
	always @(posedge fclk)
	begin
		pre_brd_n <= zrd_n;
		brd_n     <= pre_brd_n;
	end
	// bwr
	always @(posedge fclk)
	begin
		bwr_n_r  <= zwr_n;
		bwr_n_rr <= bwr_n_r;
		//
		if( bwr_n_rr )
			shreg <= 5'b1_1111;
		else
			shreg <= {shreg[3:0],1'b0};
		//
		bwr_n <= bwr_n_r || !shreg;
	end
	`else
	// buffered RD_N and WR_N
	always @* brd_n = zrd_n;
	always @* bwr_n = zwr_n;
*/

	// interrupt generation
	assign internal_int = (ena_w5300_int & (~w5300_int_n)) |
	                      (ena_sl811_int &   sl811_intrq ) ;
 	//
	assign zint_n = (internal_int & ena_zxbus_int) ? 1'b0 : 1'bZ;


endmodule

