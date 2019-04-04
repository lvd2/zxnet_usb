// initially part of ZXiznet project (c) NedoPC 2012
//
// Simple Simulator of Z80
//  performs only some Z80 cycles as tasks, no /WAIT etc.

module ssz80
(
	input  wire       clk,
	input  wire       rst_n,

	output reg        mreq_n,
	output reg        iorq_n,
	output reg        wr_n,
	output reg        rd_n,

	output reg  [15:0] a,

	inout  wire [ 7:0] d
);


	reg [7:0] dout;
	reg       oena;


	assign d = oena ? dout : 8'bZZZZ_ZZZZ;




	initial
	begin
		a='d0;

		dout = 'd0;
		oena = 1'b0;

		mreq_n = 1'bZ;
		iorq_n = 1'bZ;
		rd_n   = 1'bZ;
		wr_n   = 1'bZ;
	end




	task memrd;

		input  [15:0] addr;
		output [ 7:0] data;

		begin
			@(posedge clk);
			mreq_n <= 1'b1;
			iorq_n <= 1'b1;
			rd_n   <= 1'b1;
			wr_n   <= 1'b1;
			oena <= 1'b0;
			a <= addr;

			@(negedge clk);

			mreq_n <= 1'b0;
			rd_n   <= 1'b0;

			@(negedge clk);
			@(negedge clk);

			data = d;
			mreq_n <= 1'b1;
			rd_n   <= 1'b1;

			@(posedge clk);
		end
	endtask


	task memwr;

		input  [15:0] addr;
		input  [ 7:0] data;

		begin
			@(posedge clk);

			mreq_n <= 1'b1;
			iorq_n <= 1'b1;
			rd_n   <= 1'b1;
			wr_n   <= 1'b1;
			a <= addr;
			dout <= data;
			oena <= 1'b1;

			@(negedge clk);

			mreq_n <= 1'b0;
			@(negedge clk);
			wr_n   <= 1'b0;
			@(negedge clk);

			mreq_n <= 1'b1;
			wr_n   <= 1'b1;
			//wait(wr_n==1'b1); // delta-cycle delay!!!

			@(posedge clk);
			oena <= 1'b0;
		end
	endtask


	task iord;

		input [15:0] addr;

		output [7:0] data;

		begin

			@(posedge clk);

			mreq_n <= 1'b1;
			iorq_n <= 1'b1;
			rd_n   <= 1'b1;
			wr_n   <= 1'b1;

			oena <= 1'b0;
			a <= addr;

			@(negedge clk);

			iorq_n <= 1'b0;
			rd_n   <= 1'b0;

			@(negedge clk);
			@(negedge clk);

			data = d;

			iorq_n <= 1'b1;
			rd_n   <= 1'b1;

			@(posedge clk);
		end

	endtask


	task iowr;

		input [15:0] addr;
		input [ 7:0] data;

		begin

			@(posedge clk);

			mreq_n <= 1'b1;
			iorq_n <= 1'b1;
			rd_n   <= 1'b1;
			wr_n   <= 1'b1;

			a <= addr;
			dout <= data;
			oena <= 1'b1;

			@(negedge clk);

			iorq_n <= 1'b0;
			wr_n   <= 1'b0;

			@(negedge clk);
			@(negedge clk);

			iorq_n <= 1'b1;
			wr_n   <= 1'b1;

			//wait(wr_n==1'b1); // delta-cycle delay!!!

			
			@(posedge clk);
			oena <= 1'b0;
		end

	endtask




endmodule

