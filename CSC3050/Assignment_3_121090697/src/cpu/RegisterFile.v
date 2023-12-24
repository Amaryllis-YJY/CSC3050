module RegisterFile(
    input wire clk,

    input wire[4:0] a1,
    input wire[4:0] a2,
    input wire[4:0] a3,
    input wire reg_write,
    input wire[31:0] wd3,

    output reg[31:0] rd1,
    output reg[31:0] rd2
);

reg[31:0] reg_file[1:31];

always @(negedge clk)
begin
    if(reg_write&&a3!=0)
        reg_file[a3]<=wd3;
end

always @(*)
begin
    rd1=32'b0;
    rd2=32'b0;
    if(a1!=0)
        rd1=reg_file[a1];
    if(a2!=0)
        rd2=reg_file[a2];
end

endmodule