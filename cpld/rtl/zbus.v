// ZXiznet project
// (c) NedoPC 2012-2018
//
// zx-bus functions: ports mapping/access, ROM mapping

module zbus
(
	input  wire        fclk,

	input  wire [15:0] za,
	inout  wire [ 7:0] zd,
	//
	inout  wire [ 7:0] bd,
	//
	input  wire        ziorq_n,
	input  wire        zrd_n,
	input  wire        zwr_n,
	input  wire        zmreq_n,
	output wire        ziorqge,
	output wire        zblkrom,
	input  wire        zcsrom_n,
	input  wire        zrst_n,

	//
	output wire        ports_wrena,
	output wire        ports_wrstb_n,
	output wire [ 1:0] ports_addr,
	output wire [ 7:0] ports_wrdata,
	input  wire [ 7:0] ports_rddata,
	
	//
	input  wire [ 1:0] rommap_win,
	input  wire        rommap_ena,

	//
	output reg         sl811_cs_n,
	output reg         sl811_a0,

	//
	output reg         w5300_cs_n,
	input  wire        w5300_ports,
	input  wire [ 9:0] async_w5300_addr,
	output reg  [ 9:0] w5300_addr,



	// buffered rd/wr strobes to usb/ether chips
	output reg         bwr_n,
	output reg         brd_n
);
	parameter BASE_ADDR = 8'hAB;




	reg [1:0] rst_n_resync;
	wire rst_n = rst_n_resync[1];


	wire io_addr_ok;

	wire mrd, mwr;

	wire ena_dbuf;
	wire ena_din;
	wire ena_dout;



	// buffering chipselects
	reg async_w5300_cs_n;
	reg async_sl811_cs_n;
	//
	reg [1:0] r_w5300_cs_n;
	reg [1:0] r_sl811_cs_n;

	// buffering sl811 a0
	reg async_sl811_a0;
	//
	reg [1:0] r_sl811_a0;

	// buffering w5300 address
	reg [9:0] r_w5300_addr [0:1];


	// for filtering strobes
	reg [2:0] wr_regs;
	reg [2:0] rd_regs;
	reg       wr_state,
	          rd_state;
	wire      wr_start,
	          rd_start;

	//
	reg [2:0] ctr_5;




	// common read and write latches latch
	reg [7:0] read_latch;
	reg [7:0] write_latch;




	// reset resync
	always @(posedge fclk, negedge zrst_n)
	if( !zrst_n )
		rst_n_resync[1:0] <= 2'b00;
	else
		rst_n_resync[1:0] <= { rst_n_resync[0], 1'b1 };




	// make filtered out vesions of read/write strobes
	always @(posedge fclk)
	begin
		wr_regs[2:0] <= { wr_regs[1:0], ~zwr_n };
		rd_regs[2:0] <= { rd_regs[1:0], ~zrd_n };
	end
	//

	assign wr_start = wr_regs[2:0]==3'b001 && !ctr_5;
	assign rd_start = rd_regs[2:0]==3'b001 && !ctr_5;


	// buffered rd/wrs
	always @(posedge fclk)
	begin
		if( wr_start )
			bwr_n <= 1'b0;
		else if( !ctr_5 )
			bwr_n <= 1'b1;
		//
		if( rd_start )
			brd_n <= 1'b0;
		else if( !ctr_5 )
			brd_n <= 1'b1;
	end
	//
	always @(posedge fclk, negedge rst_n)
	if( !rst_n )
	begin
		ctr_5 <= 3'd0;
	end
	else if( wr_start || rd_start )
		ctr_5 <= 3'd4;
	else if( ctr_5 )
		ctr_5 <= ctr_5 - 3'd1;


	// buffered chipselects
	always @(posedge fclk)
	begin
		r_w5300_cs_n[1:0] <= { r_w5300_cs_n[0], async_w5300_cs_n };
		r_sl811_cs_n[1:0] <= { r_sl811_cs_n[0], async_sl811_cs_n };
	end
	//
	always @(posedge fclk)
	if( wr_start || rd_start )
	begin
		w5300_cs_n <= async_w5300_cs_n;
		sl811_cs_n <= async_sl811_cs_n;
	end
	else if( !ctr_5 )
	begin
		w5300_cs_n <= 1'b1;
		sl811_cs_n <= 1'b1;
	end


	// buffered sl811_a0
	always @(posedge fclk)
		r_sl811_a0[1:0] <= { r_sl811_a0[0], async_sl811_a0 };
	//
	always @(posedge fclk)
	if( wr_start || rd_start )
		sl811_a0 <= async_sl811_a0;



	// buffered w5300_addr
	always @(posedge fclk)
	begin
		r_w5300_addr[0] <= async_w5300_addr;
		r_w5300_addr[1] <= r_w5300_addr[0];
	end
	//
	always @(posedge fclk)
	if( wr_start || rd_start )
		w5300_addr <= async_w5300_addr;






	// addr decode
	assign io_addr_ok = (za[7:0]==BASE_ADDR);


	// IORQGE
	assign ziorqge = io_addr_ok ? 1'b1 : 1'bZ;



	// ports write
	assign ports_addr = za[9:8];
	//
	assign ports_wrdata = zd;
	//
	assign ports_wrena   = io_addr_ok && za[15];
	assign ports_wrstb_n = ziorq_n | zwr_n;



	always @*
		async_sl811_cs_n = !( !w5300_ports && io_addr_ok && ( !za[15] || (za[15] && za[9:8]==2'b00) ) && !ziorq_n );

	//
	always @*
		async_sl811_a0 = ~za[15];


	// w5300 chip select
	assign mwr = !zmreq_n && !zwr_n && (za[15:14]==rommap_win) && rommap_ena;
	assign mrd = !zmreq_n && !zrd_n && !zcsrom_n && (za[15:14]==rommap_win) && rommap_ena;
	//
	always @*
		async_w5300_cs_n = ~(mwr || mrd || ( w5300_ports && io_addr_ok && !za[15] && !ziorq_n ) );

	// block ROM
	assign zblkrom = (rommap_ena && (za[15:14]==rommap_win)) ? 1'b1 : 1'bZ;



	assign ena_dbuf = (~async_sl811_cs_n) | (~async_w5300_cs_n);
	assign ena_dout = ~zrd_n;

	wire ports_rd = io_addr_ok && !ziorq_n && !zrd_n && za[15] && (za[9:8]!=2'b00);

	wire b_ena_dbuf = (~sl811_cs_n) | (~w5300_cs_n);
	wire b_ena_din  = ~bwr_n;

	// ports data read/buffering
/*	assign zd = (io_addr_ok && !ziorq_n && !zrd_n && za[15] && (za[9:8]!=2'b00)) ?
	            ports_rddata : ( (ena_dbuf && ena_dout) ? bd : 8'bZZZZ_ZZZZ );


	assign bd = (ena_dbuf && ena_din) ? zd : 8'bZZZZ_ZZZZ;
*/

	assign zd = ports_rd ? ports_rddata : ((ena_dbuf && ena_dout) ? read_latch : 8'hZZ);

	assign bd = (b_ena_dbuf && b_ena_din) ? write_latch : 8'hZZ;




	// write latch
	always @*
	if( !zwr_n )
		write_latch <= zd;

	// read latch
	always @*
	if( !brd_n )
		read_latch <= bd;




endmodule

