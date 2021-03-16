module mem_ctrl_tb();

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


	mem_ctrl DUT(.*);

	initial begin

		errors = '0;
		clk = 0;
		rst_n = 0;
		host_init = 0;
		host_rd_ready = '0;
		host_wr_ready = '0;
		op = '0;
		raw_address = '0;
		address_offset = '0;
		common_data_bus_read_in = '0;
		host_data_bus_read_in = '0;

		repeat (5) @(posedge clk);

		if(DUT.state != DUT.STARTUP) begin
			$display("Error: DUT failed to hold STARTUP state...");
			++errors;
		end

		// Test moving out of startup.
		host_init = 1'b1;
		rst_n = 1'b1;
		@(posedge clk);
		@(negedge clk);
		if(DUT.state != DUT.READY) begin
			$display("Error: DUT could not goto READY on init.");
			++errors;
		end
		if(DUT.ready != 1'b1) begin
			$display("Error: DUT does not raise ready properly");
			++errors;
		end

		op = 2'b01; // read
		cl_tester = $urandom();
		host_rd_ready = 1'b1;
		host_data_bus_read_in = cl_tester;
		raw_address = $urandom();
		address_offset = $urandom();

		@(posedge clk);
		@(negedge clk);
		if(host_re != 1'b1) begin
			$display("Host read enable was not asserted on time.");
			++errors;
		end
		if(corrected_address != raw_address + address_offset) begin
			$display("Address calculation incorrect.");
		end

		@(posedge clk);
		@(negedge clk);
		if(DUT.line_buffer != cl_tester) begin
			$display("Error: DUT can't read line properly!");
			++errors;
		end

		// Now we are in FILL. Test the fill values.
		if(DUT.common_data_bus_write_out != cl_tester[WORD_SIZE-1:0])
		begin
			$display("Error: data read from common data bus was wrong");
			++errors;
		end

		// 512 / 32 = 16. Wait that many cycles
		repeat (15) @(posedge clk);
		@(negedge clk);
		if(!tx_done) begin
			$display ("Error: tx_done not broadcasted at the end of the read.");
			errors++;
		end

		@(posedge clk);

		op = 2'b00;
		@(negedge clk);
		// Check that we've since returned to READY and are waiting for a new command.
		if(DUT.state != DUT.READY) begin
			$display ("Error: read op did not return to READY.");
			errors++;
		end

		// Now test write operation.
		op = 2'b11;
		@(posedge clk);
		@(negedge clk);
		
		if(DUT.state != DUT.FILL) begin
			$display ("Error: DUT did not start write.");
			errors++;
		end

		common_data_bus_read_in = $urandom();
		for(itr = 0; itr < 16; ++itr) begin
			@(posedge clk);
			common_data_bus_read_in = $urandom();
		end

		repeat (2) @(posedge clk);
		@(negedge clk);
		host_wr_ready = 1'b1;
		@(posedge clk);
		@(negedge clk);

		if(errors == 0)
			$display("YAHOO! All tests passed!");
		else
			$display("ARRR!!! Ye codes be blast! Get debugging!");
		// Stop simulation
		$stop;
	end

endmodule
