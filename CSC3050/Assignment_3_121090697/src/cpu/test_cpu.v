`timescale 1ns/1ps
//`include "cpu.v"
module test(
);
reg clk;
reg rst;

initial
begin
    $dumpfile("test.vcd");
    $dumpvars;
end

initial
begin
    rst=1'b1;
    #10
    rst=1'b0;
end

initial
begin
    clk=1'b1;
    forever #10 clk=~clk;
end

initial
begin
    #20000
    $stop;
end

CPU cpu(
    .clk(clk),
    .rst(rst),
    .is_display(is_display)
);

endmodule


