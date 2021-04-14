`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/27/2021 02:06:25 PM
// Design Name: 
// Module Name: test_bench
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module test_bench;
    reg clk;
    reg rst;

    //wire and reges to peek into processor 
    wire [15:0] PC_PEEK;
    wire [15:0] SP_PEEK;
    wire [15:0] BP_PEEK;
    wire [15:0] instruction_PEEK;
    wire [15:0] read0_addr_PEEK;
    wire [15:0] read1_addr_PEEK;
    wire [15:0] w_addr_PEEK;
    wire [15:0] write_data_PEEK;
    wire [15:0] read0_val_PEEK;
    wire [15:0] read1_val_PEEK;
    
    
    processor uP(.clk(clk), .rst(rst), .pc(PC_PEEK), .sp(SP_PEEK), .bp(BP_PEEK), .inst_peek(instruction_PEEK), .read0(read0_addr_PEEK), .read1(read1_addr_PEEK), .writeA(w_addr_PEEK), .writeD(write_data_PEEK), .dread0(read0_val_PEEK), .dread1(read1_val_PEEK) );
initial
begin
        $readmemh("code_generated.mem",uP.rom0.code_memory);
        rst = 1;
        clk = 0;
        #10;
        @(posedge clk);
        rst = 0;
        #10;
        
end
    
//always clk = #1 ~clk;
always begin 
    clk = #1 ~clk;
    if(instruction_PEEK[15:11] == 5'b00111) 
           $stop;
end
endmodule