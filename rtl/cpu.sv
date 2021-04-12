/* cpu
 * The cpu is simply
 * a DMA loopback, but implemented on top of mem_cntrl.sv
 */
module cpu
(
	input wire clk,
	input wire rst_n,
	input wire tx_done,
	input wire rd_valid,
	output logic [1:0] op,
	output logic[63:0] io_address,
	input logic[31:0] common_data_bus_in,
	output logic[31:0] common_data_bus_out
);

	typedef enum reg
	{
		READ = 1'b0,
		WRITE = 1'b1
	} cpu_state;

	
	cpu_state state;

	logic [31:0] line_buffer [15:0];
	logic [3:0] fill_count;
	logic wr_fill;

	always_comb begin
		io_address = 'h0;
		op = 2'b01;
		common_data_bus_out = '0;
		case(state) 
			READ: begin
				
			end
			WRITE: begin
				op = 'b11;
				io_address = 'h400;
				common_data_bus_out = line_buffer[fill_count];
			end
		endcase
	end

	always_ff @(posedge clk, negedge rst_n) begin
		if(!rst_n) begin

			for(integer i = 0; i < 16; ++i)
				line_buffer[i] <= '0;
			state <= READ;
			fill_count <= '0;
			wr_fill <= '0;
		end
		else begin
			for(integer i = 0; i < 16; ++i)
			begin
				if(fill_count == i)
					line_buffer[i] <= common_data_bus_in;

			end
		case(state)
			READ: begin
				if(tx_done) begin
					state <= WRITE;
				end
				if(rd_valid) begin
					fill_count <= fill_count + 1;
				end
			end
			WRITE: begin
				if(!wr_fill) begin
					wr_fill <= 1'b1;
				end
				else begin
					fill_count <= fill_count + 1;

					if(tx_done) begin
						state <= READ;
						wr_fill <= 1'b0;
					end
				end
			end
		endcase
		end
	end

endmodule

