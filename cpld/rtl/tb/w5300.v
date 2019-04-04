// ZXiznet project
// (c) NedoPC 2012
//
// test module for w5300
// tests whether transactions going for it are correct

module w5300
(
	input  wire       rst_n,
	input  wire [9:0] addr,
	input  wire       cs_n,
	input  wire       rd_n,
	input  wire       wr_n,
	output reg        int_n,
	inout  wire [7:0] d
);

	
	reg [9:0] access_addr;
	reg       access_rnw;
	reg [7:0] rd_data;
	reg [7:0] wr_data;

	
	wire rd = ~(cs_n|rd_n);
	wire wr = ~(cs_n|wr_n);


	initial
	begin
		int_n = 1'b1;
	end



	always @(negedge rd)
	begin
		access_addr <= addr;
		access_rnw  <= 1'b1;
	end

	assign d = rd ? rd_data : 8'bZZZZ_ZZZZ;
	
	
	
	always @(negedge wr)
	begin
		access_addr <= addr;
		access_rnw  <= 1'b0;
		wr_data     <= d;
	end





	task set_int_n
	(
		input new_int_n
	);
		int_n = new_int_n;

	endtask



	function get_rst_n;
	
		get_rst_n = rst_n;

	endfunction


	task init_access;

		access_addr = 'bX;
		access_rnw  = 'bX;
		wr_data     = 'bX;

	endtask
	
	
	function [9:0] get_addr;
		
		get_addr = access_addr;
	
	endfunction
	
	
	function get_rnw;
	
		get_rnw = access_rnw;
	
	endfunction
	
	
	function [7:0] get_wr_data;
		
		get_wr_data = wr_data;
	
	endfunction
	
	
	task set_rd_data( input [7:0] data );
	
		rd_data = data;
	
	endtask




endmodule

