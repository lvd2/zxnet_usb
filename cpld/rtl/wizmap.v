// ZXiznet project
// (c) NedoPC 2012-2018
//
// mapping of wiznet address space

module wizmap
(
	//
	input  wire [15:0] za,

	//
	input  wire        w5300_a0inv,
	input  wire        w5300_ports,
	input  wire [ 3:0] w5300_hi,

	//
	output wire [ 9:0] w5300_addr
);

	reg [9:0] mem_w5300; 



	always @* mem_w5300[0] = w5300_a0inv ^ za[0];



	always @*
	if( za[13]==1'b0 )
	begin
		mem_w5300[9:1] = za[9:1];
	end
	else // if( za[13]==1'b1 )
	begin
		mem_w5300[9]   = 1'b1;
		mem_w5300[8:6] = za[11:9];

		if( za[12]==1'b0 )
			mem_w5300[5:1] = 5'b10111;
		else // if( za[12]==1'b1 )
			mem_w5300[5:1] = 5'b11000;
	end



	assign w5300_addr = w5300_ports ? {w5300_hi,za[13:9],za[8]^w5300_a0inv} : mem_w5300;
	



endmodule

