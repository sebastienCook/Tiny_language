`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/20/2021 01:20:57 PM
// Design Name: 
// Module Name: TL_Interpreter
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

module rom(
    input   [15:0] address,
    output reg [15:0] instruction
);
    reg [15:0] code_memory[65535:0];
    
    always @(*)
    begin 
        instruction <= code_memory[address];
    end
endmodule

module ram(
    input [15:0] r_address_0,
    input [15:0] r_address_1,
    input [15:0] w_address,
    input [15:0] w_data,
    input        we,
    output [15:0] d_read_0,
    output [15:0] d_read_1,
    input clk
);
    reg [15:0] data_memory[65535:0];
    
    assign d_read_0 = data_memory[r_address_0];
    assign d_read_1 = data_memory[r_address_1];
    
    always @(posedge clk)
    begin 
        if(we)
            data_memory[w_address] <= w_data;
    end
endmodule  

module processor(
    input clk,
    input rst,
    
    //peeking outputs
    output [15:0] pc,
    output [15:0] sp,
    output [15:0] bp,
    output [15:0] inst_peek,
    output [15:0] read0,
    output [15:0] read1,
    output [15:0] writeA,
    output [15:0] writeD,
    output [15:0] dread0,
    output [15:0] dread1
);
    reg [15:0] PC;
    reg [15:0] BP;
    reg [15:0] SP;
    reg [15:0] LR;
    //add all the wires needed
    wire [15:0] instruction; 
    
    //ram (stack) wires
    wire [15:0] r_addr0;    //////////
    wire [15:0] r_addr1;    //////////
    wire [15:0] w_addr;     //////////
    wire [15:0] w_data;     //////////
    wire        we;         //////////
    wire [15:0] d_read0;
    wire [15:0] d_read1; 
    
    //stack instrructions 
    wire [1:0] sp_forward;    //////////
    //data processing
    wire [15:0] data_out;       //////////
    
    //branching wires
    wire should_branch;         //////////
    wire branch_taken;          //////////
    wire [15:0] branch_address; //////////
    wire pop_PC_to_LR;          //////////
    
    //peeking outputs
    assign pc = PC;
    assign sp = SP;
    assign bp = BP;
    assign inst_peek = instruction;
    assign read0 = r_addr0;
    assign read1 = r_addr1;
    assign writeA = w_addr;
    assign writeD = w_data;
    assign dread0 = d_read0;
    assign dread1 = d_read1;
    
    //ram 
    assign r_addr0 = 
    //ID
    ((instruction[15:11] == 5'b00010)) ? (BP + instruction[10:0]):
    //SP
    SP;
    
    assign r_addr1 = SP - 16'd1;
    
    
    //stack instruction implementation
    assign sp_forward = 
    //SP = SP
    ((instruction[15:11] == 5'b00011)) ? 2'b00: //NOP
    ((instruction[15:11] == 5'b00111)) ? 2'b00: //pop nothing
    ((instruction[15:12] == 4'b1000)) ? 2'b00: //jump
    ((instruction[15:12] == 4'b1100)) ? 2'b00:  //call 
    ((instruction[15:12] == 4'b0111)) ? 2'b00: //not
    //SP++
    ((instruction[15:12] == 4'b0000)) ? 2'b01: //push reg or constant 
    ((instruction[15:11] == 5'b00010)) ? 2'b01: //push id
    //pop SP
    ((instruction[15:11] == 5'b00100) & (instruction[10:9] == 3'b01)) ? 2'b10:
    //SP--
    2'b11;
    
    assign we = 
    ((instruction[15:11] == 5'b00011)) ? 1'b0: //NOP
    ((instruction[15:11] == 5'b00100)) ? 1'b0: //pop reg
    ((instruction[15:11] == 5'b00111)) ? 1'b0: //pop nothing
    //branching
    ((instruction[15:14] == 2'b10)) ? 1'b0:
    ((instruction[15:14] == 2'b11)) ? 1'b0:
    1'b1;
    
    assign w_addr = 
    // if not sp
    ((instruction[15:12] == 4'b0111)) ? SP:
    //if data processing sp-1
    ((instruction[15:14] == 2'b01)) ? (SP - 16'd1):
    //if push sp++
    ((instruction[15:13] == 3'b000)) ? (SP + 16'd1):
    // if pop ID
    ((instruction[15:12] == 4'b0011)) ? (BP + instruction[10:0]):
    16'b0111111111111111;
    
    assign w_data = 
    //op
    ((instruction[15:14] == 2'b01)) ? data_out:
    //push constant
    ((instruction[15:11] == 5'b00001)) ? instruction[10:0]:
    //push PC
    ((instruction[15:11] == 5'b00000) & (instruction[10:9] == 2'b00)) ? PC:
    //push SP
    ((instruction[15:11] == 5'b00000) & (instruction[10:9] == 2'b01)) ? SP:
    //push BP
    ((instruction[15:11] == 5'b00000) & (instruction[10:9] == 2'b10)) ? BP:
    //push ID
    ((instruction[15:11] == 5'b00010)) ? d_read0:
    //pop ID
    ((instruction[15:11] == 5'b00110)) ? d_read0:
    //shouldn't happen
    16'b0;
    //data processing implementation
    
    assign data_out = 
    //ADD
    ((instruction[15:10] == 6'b010000)) ? (d_read0 + d_read1):
    //SUB
    ((instruction[15:10] == 6'b010001)) ? (d_read0 - d_read1):
    //MUL
    ((instruction[15:10] == 6'b010010)) ? (d_read0 * d_read1):
    //DIV
    ((instruction[15:10] == 6'b010011)) ? (d_read0 / d_read1):
    //NOT
    ((instruction[15:10] == 6'b011100)) ? (~d_read0):
    //EQ
    ((instruction[15:10] == 6'b010100)) ? (d_read0 == d_read1 ? 1 : 0):
    //NEQ
    ((instruction[15:10] == 6'b010101)) ? (d_read0 != d_read1 ? 1 : 0):
    //LT
    ((instruction[15:10] == 6'b010110)) ? (d_read1 < d_read0 ? 1 : 0):
    //LE
    ((instruction[15:10] == 6'b010111)) ? (d_read1 <= d_read0 ? 1 : 0):
    //GT
    ((instruction[15:10] == 6'b011000)) ? (d_read1 > d_read0 ? 1 : 0):
    //GE
    ((instruction[15:10] == 6'b011001)) ? (d_read1 >= d_read0 ? 1 : 0):
    //AND
    ((instruction[15:10] == 6'b011010)) ? (d_read0 & d_read1):
    //OR
    ((instruction[15:10] == 6'b011011)) ? (d_read0 | d_read1):
    //shouldn't happen
    d_read0;
    
    //branch instruction implementation
    assign should_branch = 
        //jump, brct, brcf
        ((instruction[15:14] == 2'b10) & (branch_taken)) ? 1'b1:
        //call and ret
        ((instruction[15:14] == 2'b11)) ? 1'b1:
        //pop register (PC)
        ((instruction[15:12] == 5'b1101)) ? 1'b1:
        1'b0;
        
    assign branch_taken = 
    //jump
    ((instruction[13:12] == 2'b00)) ? 1'b1:
    //brct
    ((instruction[15:12] == 4'b1001) & (d_read0 != 16'd0)) ? 1'b1:
    //brcf
    ((instruction[15:12] == 4'b1010) & (d_read0 == 16'd0)) ? 1'b1:
    1'b0;
    
    assign branch_address = 
    //jump, brct, brcf
    ((instruction[15:14] == 2'b10) & instruction[11] == 1'b1) ? $signed(PC[11:0] + instruction[11:0]):
    ((instruction[15:14] == 2'b10)) ? (PC + instruction[11:0]):
    //call
    ((instruction[15:12] == 4'b1100) & instruction[11] == 1'b1) ? $signed(PC[11:0] + instruction[11:0]):
    ((instruction[15:12] == 4'b1100)) ? (PC + instruction[11:0]):
    //pop register (PC) --> only happens on return
    ((instruction[15:12] == 4'b1101)) ? (LR):
    16'b0;
    
    assign pop_PC_to_LR = 
    //pop register (PC) --> only happens on return so increment by 1
    ((instruction[15:11] == 5'b00100) & (instruction[10:9] == 2'b00)) ? 1'b1:
    1'b0;
    
    
    rom rom0(.address(PC), .instruction(instruction));
    ram ram0(.r_address_0(r_addr0), .r_address_1(r_addr1), .w_address(w_addr), .w_data(w_data), .we(we), .d_read_0(d_read0), .d_read_1(d_read1), .clk(clk));
        
    
    
    //rest of the logic
    always @(posedge clk)
    begin 
        if(rst)
            PC <= 16'sd1;
        else
        begin 
            if(should_branch)
            begin
                PC <= branch_address;
            end
            else 
                PC <= PC + 16'sd1;
        end
    end

    always @(posedge clk)
    begin 
        if(rst)
        begin
            BP <= 16'sd0;
        end
        else
            if((instruction[15:11] == 5'b00100) & (instruction[10:9] == 2'b10))
                BP <= d_read0;
        begin 
        end
    end

    always @(posedge clk)
    begin 
        if(rst)
        begin
            SP <= 16'sd0;
        end
        else
        begin 
            if(sp_forward == 2'b01)
                SP <= SP + 16'b1; 
            else if(sp_forward == 2'b10)
                SP <= d_read0; 
            else if(sp_forward == 2'b11)
                SP <= SP - 16'b1; //everything that takes from the stack
            else
                SP <= SP; //do nothing NOP
        end
    end
 
    always @(posedge clk)
    begin 
        if(rst)
        begin
            LR <= 16'sd0;
        end
        else
            if(pop_PC_to_LR)
                LR <= d_read0 + 16'd1;
        begin 
        end
    end   
endmodule 
    

    
    
    
    
    
    