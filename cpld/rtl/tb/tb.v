// ZXiznet project
// (c) NedoPC 2018
//
// testbench


`timescale 1ns/100ps

//// CPU at 14MHz
`define HALF_CPU_PERIOD (35.7)

// CPU at 7MHz
//`define HALF_CPU_PERIOD (70)

// filter clock
`define HALF_FCLK_PERIOD (10.4)

module tb;

	reg rst_n;
	reg clk;

	reg fclk;


	tri1 iorq_n,
	     mreq_n,
	     rd_n,
	     wr_n;

	tri1 int_n;

	wire csrom_n;

	wire iorqge,
	     blkrom;
	

	wire [15:0] a;
	wire [ 7:0] d;

	tri  [ 7:0] #1 bd;

	wire brd_n, bwr_n;


	reg [7:0] tmp;




	wire [9:0] w5300_addr;
	wire       w5300_rst_n;
	wire       w5300_cs_n;
	wire       w5300_int_n;

	wire       sl811_rst_n;
	wire       sl811_a0;
	wire       sl811_cs_n;
	wire       sl811_ms_n;
	wire       sl811_intrq;

	
	
	reg usb_power;


	reg [1:0] where_rom;




	// filter clock
	initial
	begin
		fclk = 1'b1;

		forever #`HALF_FCLK_PERIOD fclk = ~fclk;
	end


	initial
	begin
		clk = 1'b1;

		forever #`HALF_CPU_PERIOD clk = ~clk;
	end


	initial
	begin
		rst_n = 1'b0;

		repeat(3) @(posedge clk);

		rst_n <= 1'b1;
	end



	initial
	begin
		where_rom = 2'b00;

		usb_power = 1'b0;
	end








	top DUT
	(
		.fclk(fclk),

		.za(a),
		.zd(d),

		.ziorq_n(iorq_n),
		.zmreq_n(mreq_n),
		.zrd_n(rd_n),
		.zwr_n(wr_n),

		.zcsrom_n(csrom_n),
		
		.ziorqge(iorqge),
		.zblkrom(blkrom),

		.zrst_n(rst_n),
		.zint_n(int_n),

		.bd(bd),

		.bwr_n(bwr_n),
		.brd_n(brd_n),


		.w5300_rst_n(w5300_rst_n),
		.w5300_addr (w5300_addr ),
		.w5300_cs_n (w5300_cs_n ),
		.w5300_int_n(w5300_int_n),
		
		.sl811_rst_n(sl811_rst_n),
		.sl811_intrq(sl811_intrq),
		.sl811_ms_n (sl811_ms_n ),
		.sl811_cs_n (sl811_cs_n ),
		.sl811_a0   (sl811_a0   ),

		.usb_power(usb_power)

	);












	ssz80 z
	(
		.clk  (clk  ),
		.rst_n(rst_n),

		.iorq_n(iorq_n),
		.mreq_n(mreq_n),
		.rd_n  (rd_n  ),
		.wr_n  (wr_n  ),

		.a(a),
		.d(d)
	);





	w5300 w
	(
		.rst_n(w5300_rst_n),
		.addr (w5300_addr ),
		.cs_n (w5300_cs_n ),
		.rd_n (brd_n      ),
		.wr_n (bwr_n      ),
		.int_n(w5300_int_n),
		.d(bd)
	);


	sl811 s
	(
		.rst_n(sl811_rst_n),
		.a0   (sl811_a0   ),
		.cs_n (sl811_cs_n ),
		.wr_n (bwr_n      ),
		.rd_n (brd_n      ),
		.ms   (~sl811_ms_n),
		.intrq(sl811_intrq),
		.d(bd)
	);


	// csrom gen
	assign csrom_n = !(a[15:14]==where_rom[1:0]);

	
	
	
	
	
////////////////////////////////////////////////////////////////////////////////	
////////////////////////////////////////////////////////////////////////////////	
////////////////////////////////////////////////////////////////////////////////	
/// here start tests

	reg [15:0] rstint_port = 16'h83_AB;
//	reg [15:0] w5300_port  = 16'h82_AB;
//	reg [15:0] sl811_port  = 16'h81_AB;
	reg [15:0] cfg_port    = 16'h82_AB;
	reg [15:0] wiomap_port = 16'h81_AB;
	reg [15:0] sl_addr     = 16'h80_AB;
	reg [15:0] sl_data     = 16'h7F_AB;
	
	initial
	begin : tests
	
	
		reg [7:0] tmp;
		reg [7:0] tmp2;

		
		wait(rst_n===1'b1);

		repeat(10) @(posedge clk);
	
	

		test_pwon_resets();
		test_pwon_ints();



		forever // repeat(10000)
		begin
			case( $random%6 )
			0: check_sl811_access();
			1: check_w5300_mem_access();
			2: check_w5300_port_access();
			3: test_resets();
			4: check_sl811_port();
			5: test_ints(); 
			endcase
		end

		








		




		
		
		$display("all tests passed!");
		$stop;
	end

	
	

	
	
	task wait_end_access();

		reg a;

		a = 1'b0;
		wait( brd_n==1'b1 && bwr_n==1'b1 );

		a <= 1'b1;
		wait(a==1'b1);

	endtask
	







	// translates memory address to w5300 address in accordance with specs
	function [9:0] mk_w5300_addr( input [13:0] mem_addr );

		if( mem_addr<14'h2000 )
		begin
			mk_w5300_addr = mem_addr[9:0];
		end
		else if( mem_addr<14'h3000 )
		begin
			mk_w5300_addr[9] = 1'b1;
			mk_w5300_addr[8:6] = mem_addr[11:9];
			mk_w5300_addr[5:1] = 5'b10111;
			mk_w5300_addr[0] = mem_addr[0];
		end
		else // if( mem_addr<14'h4000 )
		begin
			mk_w5300_addr[9] = 1'b1;
			mk_w5300_addr[8:6] = mem_addr[11:9];
			mk_w5300_addr[5:1] = 5'b11000;
			mk_w5300_addr[0] = mem_addr[0];
		end

	endfunction






	task check_w5300_port_access();

		reg [7:0] tmp,rddata,wrdata;
		reg [9:0] waddr;
		reg       a0_inv;

		reg [15:0] port;


		a0_inv = $random>>31;

		waddr = $random>>22;



		z.iowr(cfg_port,{3'd0,1'b1,a0_inv,3'd0});

		z.iord(cfg_port,tmp);
		if( tmp[4:0]!=={1'b1,a0_inv,3'd0} )
		begin
			$display("can't set w5300 port!");
			$stop;
		end
		

		z.iowr(wiomap_port,waddr[9:6]);
		z.iord(wiomap_port,tmp);
		if( tmp[3:0]!==waddr[9:6] )
		begin
			$display("can't set wiomap port!");
			$stop;
		end



		// make access port address
		port = sl_data;
		port[13:8] = waddr[5:0];


		w.init_access(); // clear access_* to X


		if( $random>>31 )
		begin
			wrdata = $random>>24;
			z.iowr(port,wrdata);
			wait_end_access();
			if( w.get_addr()!==(waddr^a0_inv) ||
			    w.get_rnw()!==1'b0            ||
			    w.get_wr_data()!==wrdata      )
			begin
				$display("w5300 port write failed!");
				$stop;
			end
		end
		else
		begin
			rddata = $random>>24;
			w.set_rd_data(rddata);
			z.iord(port,tmp);
			@(negedge clk);
			if( w.get_addr()!==(waddr^a0_inv) ||
			    w.get_rnw()!==1'b1            ||
			    tmp!==rddata                  )
			begin
				$display("w5300 port read failed!");
				$stop;
			end
		end

	endtask




	task check_w5300_mem_access;

		reg [7:0] tmp,rddata,wrdata;
		reg [15:0] memaddr;
		reg [ 9:0] waddr;

		reg [1:0] rom;
		reg       a0_inv;
		reg       sub_ena;


		rom    = $random>>30;
		a0_inv = $random>>31;
		sub_ena= $random>>31;

		z.iowr(cfg_port,{4'd0,a0_inv,sub_ena,rom});

		z.iord(cfg_port,tmp);
		if( tmp[4:0]!=={1'd0,a0_inv,sub_ena,rom} )
		begin
			$display("can't set w5300 port!");
			$stop;
		end


		if( $random>>31 )
			where_rom = rom;
		else
		begin
			where_rom = $random>>30;
			while( where_rom==rom )
				where_rom = $random>>30;
		end



		w.init_access(); // clear access_* to X


		memaddr = $random>>18;
		memaddr[15:14] = where_rom;



		if( $random>>31 )
		begin
			repeat(10)
			begin
				wrdata = $random>>24;
				z.memwr(memaddr,wrdata);
				wait_end_access();
				if( where_rom==rom && sub_ena )
				begin
					if( w.get_addr()!==(mk_w5300_addr(memaddr[13:0])^a0_inv) ||
					    w.get_rnw()!==1'b0                                   ||
					    w.get_wr_data()!==wrdata                             )
					begin
						$display("w5300 write failed!");
						$stop;
					end
				end
				else
				begin
					if( w.get_addr()!=={10{1'bX}} || w.get_rnw()!==1'bX || w.get_wr_data()!=={8{1'bX}} )
					begin
						$display("write succeeded with wrong ROM mapping!");
						$stop;
					end
				end
			end
		end
		else
		begin
			rddata = $random>>24;
			w.set_rd_data(rddata);
			z.memrd(memaddr,tmp);
			@(negedge clk);
			if( where_rom==rom && sub_ena )
			begin
				if( w.get_addr()!==(mk_w5300_addr(memaddr[13:0])^a0_inv) ||
				    w.get_rnw()!==1'b1                                   ||
				    tmp!==rddata                                         )
				begin
					$display("w5300 read failed!");
					$stop;
				end
			end
			else
			begin
				if( w.get_addr()!=={10{1'bX}} || w.get_rnw()!==1'bX )
				begin
					$display("read succeeded with wrong ROM mapping!");
					$stop;
				end
			end
		end













	endtask







	task check_sl811_access;

		reg [7:0] tmp, rddata, wrdata;

		reg [15:0] rdport;


		// turn off w5300 port access
		z.iord(cfg_port,tmp);
		if( tmp[4]!==1'b0 )
		begin
			tmp[4]=1'b0;
			z.iowr(cfg_port,tmp);
		end


		// check address reg
		wrdata=$random>>24;
		z.iowr(sl_addr,wrdata);
		wait_end_access();
		if( s.get_rnw()!==1'b0 || s.get_addr()!==1'b0 || s.get_wr_data()!==wrdata )
		begin
			$display("sl811 address write failed!");
			$stop;
		end
		//
		rddata=$random>>24;
		s.set_rd_data(rddata);
		z.iord(sl_addr,tmp);
		@(negedge clk);
		if( s.get_rnw()!==1'b1 || s.get_addr()!==1'b0 || tmp!==rddata )
		begin
			$display("sl811 address read failed!");
			$stop;
		end



		// check data reg
		rdport=sl_data;
		rdport[14:8]=$random>>25;
		wrdata=$random>>24;
		z.iowr(rdport,wrdata);
		wait_end_access();
		if( s.get_rnw()!==1'b0 || s.get_addr()!==1'b1 || s.get_wr_data()!==wrdata )
		begin
			$display("sl811 data write failed!");
			$stop;
		end
		//
		rddata=$random>>24;
		s.set_rd_data(rddata);
		z.iord(rdport,tmp);
		@(negedge clk);
		if( s.get_rnw()!==1'b1 || s.get_addr()!==1'b1 || tmp!==rddata )
		begin
			$display("sl811 data read failed!");
			$stop;
		end

	endtask




	task check_sl811_port;

		reg [7:0] tmp;

		reg ms;

		ms=$random>>31;

		z.iowr(cfg_port,{1'b0,ms,6'd0});
		wait_end_access();

		@(posedge clk);
		if( (rst_n===1'b1 && s.get_ms()!==ms   ) ||
		    (rst_n===1'b0 && s.get_ms()!==1'b1 ) )
		begin
			$display("sl811_ms_n behaves wrong!");
			$stop;
		end

		
		usb_power <= $random>>31;
		@(posedge clk);
		z.iord(cfg_port,tmp);
		if( tmp[7]!==usb_power )
		begin
			$display("can't sense usb_power!");
			$stop;
		end

	endtask





	task test_pwon_ints;

		reg [7:0] tmp;

		// check ints
		z.iord(rstint_port,tmp);
		if( tmp[1:0]!==2'b00 )
		begin
			$display("int requests after reset!");
			$stop;
		end
		if( tmp[3:2]!==2'b00 )
		begin
			$display("ints enabled after reset!");
			$stop;
		end
		if( tmp[7]!==1'b0 )
		begin
			$display("internal int is on after reset!");
			$stop;
		end
		if( tmp[6]!==1'b0 )
		begin
			$display("ext.int assertion enabled after reset!");
			$stop;
		end
		if( int_n!==1'b1 )
		begin
			$display("ext.int asserted after reset!");
			$stop;
		end
	endtask
	




	task test_ints;

		reg [7:0] tmp;

		reg [1:0] ints,intena;

		reg eintena;

		
		ints = $random>>30;

		intena = $random>>30;

		eintena = $random>>31;

		
		s.set_intrq(ints[1]);
		w.set_int_n(~ints[0]);

		@(posedge clk);


		z.iord(rstint_port,tmp);
		tmp[3:2]=intena;
		tmp[6]=eintena;
		z.iowr(rstint_port,tmp);

		
		z.iord(rstint_port,tmp);

		if( tmp[1]!==ints[1] || tmp[0]!==ints[0] )
		begin
			$display("wrong int signals states!");
			$stop;
		end

		if( (  (ints&intena) && tmp[7]!==1'b1) ||
		    ( !(ints&intena) && tmp[7]!==1'b0) )
		begin
			$display("wrong internal int state!");
			$stop;
		end

		if( tmp[6]!==eintena )
		begin
			$display("wrong eintena state!");
			$stop;
		end

		if( (eintena && (ints&intena)) ? (int_n!==1'b0) : (int_n!==1'b1) )
		begin
			$display("wrong int_n forming!");
			$stop;
		end




	endtask

	
	



	task test_resets;

		reg [7:0] tmp;

		reg [1:0] resets;


		
		resets = $random>>30;


		// read-modify-write reset register
		z.iord(rstint_port,tmp);

		tmp[5:4] = resets[1:0];

		z.iowr(rstint_port,tmp);

		if( s.get_rst_n() !== resets[1] )
		begin
			$display("no control of sl811 reset!");
			$stop;
		end

		if( w.get_rst_n() !== resets[0] )
		begin
			$display("no control of w5300 reset!");
			$stop;
		end

	endtask
	



	task test_pwon_resets;

		reg [7:0] tmp;

		// test resets state after reset
		if( w.get_rst_n() !== 1'b0 )
		begin
			$display("w5300 hasn't rst_n=0 after reset!");
			$stop;
		end
		//
		if( s.get_rst_n() !== 1'b0 )
		begin
			$display("sl811 hasn't rst_n=0 after reset!");
			$stop;
		end

		// read reset register and check it
		z.iord(rstint_port,tmp);
		if( tmp[5:4]!==2'b00 )
		begin
			$display("reset bits in #83AB not 0 after reset!");
			$stop;
		end

	endtask











	// time marks
	always
	begin : timemarks

		int ms;

		ms = ($time/1000000);

		#1000000.0;

		$display("time mark: %d ms",ms);
	end





endmodule

