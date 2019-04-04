// ZXiznet project
// (c) NedoPC 2012
//
// test module for sl811
// tests whether transactions going for it are correct

module sl811
(
	input  wire       rst_n,
	input  wire       a0,
	input  wire       cs_n,
	input  wire       rd_n,
	input  wire       wr_n,
	input  wire       ms,
	output reg        intrq,
	inout  wire [7:0] d
);

	
	reg       access_addr;
	reg       access_rnw;
	reg [7:0] wr_data;
	reg [7:0] rd_data;
	
	wire rd = ~(cs_n|rd_n);
	wire wr = ~(cs_n|wr_n);


	initial
	begin
		intrq = 1'b0;
	end



	always @(negedge rd)
	begin
		access_addr <= a0;
		access_rnw  <= 1'b1;
	end

	assign d = rd ? rd_data : 8'bZZZZ_ZZZZ;
	
	
	
	always @(negedge wr)
	begin
		access_addr <= a0;
		access_rnw  <= 1'b0;
		wr_data     <= d;
	end





	task set_intrq
	(
		input new_intrq
	);
		intrq = new_intrq;

	endtask
	

	function get_rst_n;
	
		get_rst_n = rst_n;

	endfunction


	function get_ms;

		get_ms = ms;
	
	endfunction

	
	function get_addr;
		
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

