module cpu_tb();


	parameter ADDR_BITCOUNT = 64;
	parameter WORD_SIZE = 32;
	parameter CL_SIZE_WIDTH = 512;

	integer itr;
	integer errors;
	logic clk;
	logic rst_n;
	logic host_init;
	logic host_rd_ready;
	logic host_wr_ready;
	logic [1:0] op;
	logic [ADDR_BITCOUNT-1:0] raw_address;
	logic [ADDR_BITCOUNT-1:0] address_offset;

	logic [WORD_SIZE-1:0] common_data_bus_read_in;
	logic [WORD_SIZE-1:0] common_data_bus_write_out;

	logic [CL_SIZE_WIDTH-1:0] host_data_bus_read_in;
	logic [CL_SIZE_WIDTH-1:0] host_data_bus_write_out;

	logic [CL_SIZE_WIDTH-1:0] cl_tester;

	logic [ADDR_BITCOUNT-1:0] corrected_address;

	logic ready;
	logic tx_done;
	logic rd_valid;
	logic host_re; // might need since controls fifo shifting...
	logic host_we;

	// Create clock signal
	always #20 assign clk = ~clk;

	logic[63:0] io_address;
	logic[31:0] common_data_bus_in;
	logic[31:0] common_data_bus_out;

	mem_ctrl DUT(.*);
	cpu DUT_2(.*);

	// Assign aliases
	assign common_data_bus_read_in = common_data_bus_out;
	assign common_data_bus_in = common_data_bus_write_out;
	assign raw_address = io_address;

	initial begin

		errors = '0;
		clk = 0;
		rst_n = 0;
		host_init = 0;
		host_rd_ready = '1;
		host_wr_ready = '1;
		host_data_bus_read_in = -512'd1;
		op = '0;
		address_offset = '0;

		repeat (10) @(posedge clk);
		rst_n = 1;
		host_init = 1;

		repeat (40) @(posedge clk);

		if(errors == 0)
			$display("YAHOO! All tests passed!");
		else
			$display("ARRR!!! Ye codes be blast! Get debugging!");
		// Stop simulation
		$stop;
	end

endmodule
