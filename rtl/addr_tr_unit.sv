/* addr_tr_unit.sv
 * Address Translation Unit
 *
 * Given four base addresses for the four different 1GB segments,
 * give the correct address to the host corresponding to a 64-bit "virtual" address.
 * 
 * wchen329
 */
module addr_tr_unit
#(
	parameter ADDR_BITCOUNT = 64
)
(
	input logic [ADDR_BITCOUNT-1:0] virtual_addr,

	input logic [ADDR_BITCOUNT-1:0] base_address_s0,
	input logic [ADDR_BITCOUNT-1:0] base_address_s1,
	input logic [ADDR_BITCOUNT-1:0] base_address_s2,
	input logic [ADDR_BITCOUNT-1:0] base_address_s3,

	output logic [ADDR_BITCOUNT-1:0] corrected_address
);

	assign corrected_address =
		~virtual_addr[31] & ~virtual_addr[30] ? base_address_s0 + {2'b0, virtual_addr[29:0]} :
		~virtual_addr[31] &  virtual_addr[30] ? base_address_s1 + {2'b0, virtual_addr[29:0]} :
		 virtual_addr[31] & ~virtual_addr[30] ? base_address_s2 + {2'b0, virtual_addr[29:0]} :
		 virtual_addr[31] &  virtual_addr[30] ? base_address_s3 + {2'b0, virtual_addr[29:0]} : '0;

endmodule

