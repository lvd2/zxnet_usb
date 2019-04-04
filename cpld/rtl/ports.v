// ZXiznet project
// (c) NedoPC 2012-2018
//
// ports of card: #83AB, #82AB, #81AB (read/write),
// correspond to addr=2'b11, 2'b10, 2'b01

module ports
(
	input  wire       rst_n,

	input  wire       wrstb_n, // write strobe for ports, latched at positive edge
	input  wire       wrena,   // write enable for ports
	//
	input  wire [1:0] addr, // addressing: no port at 2'b00
	//
	input  wire [7:0] wrdata,
	output reg  [7:0] rddata,


	// inputs and outputs
	output reg        ena_w5300_int,
	output reg        ena_sl811_int,
	output reg        ena_zxbus_int,
	input  wire       w5300_int_n,
	input  wire       sl811_intrq,
	input  wire       internal_int,
	//
	output reg  [1:0] rommap_win,
	output reg        rommap_ena,
	output reg        w5300_a0inv,
	output reg        w5300_rst_n,
	output reg        w5300_ports,
	output reg  [3:0] w5300_hi,

	//
	output reg        sl811_ms_n,
	output reg        sl811_rst_n,
	//
	input  wire       usb_power
);




	// wr #83AB
	always @(posedge wrstb_n, negedge rst_n)
	if( !rst_n )
	begin
		ena_w5300_int <= 1'b0;
		ena_sl811_int <= 1'b0;
		ena_zxbus_int <= 1'b0;
		w5300_rst_n <= 1'b0;
		sl811_rst_n <= 1'b0;
	end
	else if( wrena && addr==2'b11 )
	begin
		ena_w5300_int <= wrdata[2];
		ena_sl811_int <= wrdata[3];
		ena_zxbus_int <= wrdata[6];
		w5300_rst_n <= wrdata[4];
		sl811_rst_n <= wrdata[5];
	end
	

	// wr #82AB
	always @(posedge wrstb_n, negedge rst_n)
	if( !rst_n )
	begin
		rommap_win <= 2'b00;
		rommap_ena <= 1'b0;
		
		w5300_a0inv <= 1'b0;
		w5300_ports <= 1'b0;
		sl811_ms_n <= 1'b0;
	end
	else if( wrena && addr==2'b10 )
	begin
		rommap_win <= wrdata[1:0];
		rommap_ena <= wrdata[2] & (~wrdata[4]);
		
		w5300_a0inv <= wrdata[3];
		w5300_ports <= wrdata[4] & (~wrdata[2]);
		sl811_ms_n <= ~wrdata[6];
	end


	// wr #81AB
	always @(posedge wrstb_n, negedge rst_n)
	if( !rst_n )
	begin
		w5300_hi    <= 4'd0;
	end
	else if( wrena && addr==2'b01 )
	begin
		w5300_hi    <= wrdata[3:0];
	end



	// read ports
	always @*
	case(addr)
		// #83AB
		2'b11: rddata = { internal_int, ena_zxbus_int, sl811_rst_n, w5300_rst_n, ena_sl811_int, ena_w5300_int, sl811_intrq, ~w5300_int_n };
		
		// #82AB
		2'b10: rddata = { usb_power, ~sl811_ms_n, 1'bX, w5300_ports, w5300_a0inv, rommap_ena, rommap_win[1:0] };

		// #81AB
		2'b01: rddata = { 4'bXXXX, w5300_hi };

		default: rddata = 8'bXXXX_XXXX;

	endcase




endmodule

