// DO NOT INSERT INTO ZXBUS!!!
//
// ZXiznet project TEST (see README)
// (c) NedoPC 2018
//
// DO NOT INSERT INTO ZXBUS!!!
//
// top-level module
//
// DO NOT INSERT INTO ZXBUS!!!

module top
(
	input  wire        fclk, // 48MHz clock
	
	output reg         usb_clk, // 12MHz clock for USB chip (sl811)
	
	output wire [14:0] za,
	input wire zb,
	output wire [ 7:0] zd,
	
	output wire [ 7:0] bd,

	output wire        ziorq_n,
	output wire        zrd_n,
	input  wire        zwr_n,
	output wire        zmreq_n,
	output wire        ziorqge,
	output wire        zblkrom,
	output wire        zcsrom_n,
	input  wire        zrst_n,
	output wire        zint_n,

	// buffered RD_N and WR_N for chips
	output wire        brd_n,
	output wire        bwr_n,

	output wire         w5300_rst_n,
	output wire  [ 9:0] w5300_addr,
	output wire         w5300_cs_n,
	input  wire        w5300_int_n,


	// sl811 Usb chip
	output wire         sl811_rst_n,
	input  wire        sl811_intrq,
	output wire         sl811_ms_n,
	output wire         sl811_cs_n,
	output wire         sl811_a0,

	// usb power presence
	input  wire        usb_power
);


	reg usb_ckreg;


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



	// resets
	assign w5300_rst_n = 1'b0;
	assign sl811_rst_n = 1'b0;

	// usb selector
	assign sl811_ms_n = ~usb_power;



	assign w5300_cs_n = 1'b1;
	assign sl811_cs_n = 1'b1;



	reg [53:0] shreg;

	always @(posedge fclk)
	begin
		shreg[53:1] <= shreg[52:0];

		shreg[0] <= ~(shreg[53] ^ shreg[52] ^ shreg[17] ^ shreg[16]);
	end


	assign za = shreg[14:0];
	assign zd         = shreg[23:16];
	assign bd         = shreg[31:24];
	assign w5300_addr = shreg[41:32];


	assign ziorq_n  = shreg[42];
	assign zrd_n    = shreg[43];
	assign zmreq_n  = shreg[45];
	assign ziorqge  = shreg[46];
	assign zblkrom  = shreg[47];
	assign zcsrom_n = shreg[48];
	assign zint_n   = shreg[50];
	assign brd_n    = shreg[51];
	assign bwr_n    = shreg[52];
	assign sl811_a0 = shreg[53];



endmodule

