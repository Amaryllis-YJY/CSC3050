`include "alu.v"
`include "RegisterFile.v"
`include "ControlUnit.v"
`include "InstructionRAM.v"
`include "MainMemory.v"

module CPU
(
  input wire clk,
  input wire rst,

  output wire is_display
);

wire stall;
wire pc_jr; 
wire pc_jump;
wire pc_branch;

wire d_dr_A;
wire e_alu_out_A;
wire m_alu_out_A;
wire m_read_data_A;

wire d_dr_B;
wire e_alu_out_B;
wire m_alu_out_B;
wire m_read_data_B;

wire is_equal;
wire[31:0] instruction;
reg[31:0] pc_reg;

//IF signal
wire f_stall;
wire[31:0] f_pc;

//ID signal
reg[31:0] fwd_rd_A;
reg[31:0] fwd_rd_B;

reg[31:0] d_pc;
reg[31:0] d_pc_plus4;

reg[31:0] d_extend_shamt;
reg[31:0] d_extend_imm;

wire[31:0] d_drA;
wire[31:0] d_drB;

wire[3:0] d_alu_control;
wire[1:0] d_alu_src_A;//0 for rs, 1 for sa, 2 for link
wire[1:0] d_alu_src_B;//0 for rt, 1 for imm, 2 for link
wire[1:0] d_reg_dst;//0 for rd, 1 for rt, 2 for link

wire d_reg_write;
wire d_extend_sign;
wire d_is_load;
wire d_is_display;
wire d_mem_to_reg;
wire d_mem_write;
wire d_mem_used;
wire d_m_forward;
wire d_rst;

reg[31:0] d_inst;
reg[5:0] opcode;
reg[4:0] rs;
reg[4:0] rt;
reg[5:0] funct;

//EXE signal
reg[31:0] e_extend_shamt;
reg[31:0] e_extend_imm;

reg[3:0] e_alu_control;
reg[1:0] e_alu_src_A;//0 for rs, 1 for sa, 2 for link
reg[1:0] e_alu_src_B;//0 for rt, 1 for imm, 2 for link, 3 for branch
reg[1:0] e_reg_dst;//0 for rd, 1 for rt, 2 for link

reg[31:0] e_rdA;
reg[31:0] e_rdB;
reg[31:0] e_pc_plus4;

reg[31:0] e_src_A;
reg[31:0] e_src_B;
reg[4:0] e_write_reg;

wire[2:0] flags;
wire[31:0] e_alu_out;

reg[4:0] e_rt;
reg[4:0] e_rd;

reg e_m_forward;
reg e_is_load;
reg e_is_display;
reg e_reg_write;

reg e_mem_to_reg;
reg e_mem_write;
reg e_mem_used;

wire[31:0] e_write_data;

//MEM signal
reg m_is_load;
reg m_is_display;

reg m_m_forward;
reg m_mem_to_reg;
reg m_mem_write;
reg m_mem_used;
reg m_reg_write;

reg[4:0] m_write_reg;
reg[31:0] m_alu_out;
reg[31:0] m_write_data;

wire[31:0] m_read_data;

//WB signal
reg w_reg_write;
reg[4:0] w_write_reg;
reg[31:0] w_alu_out;
reg[31:0] w_read_data;
reg w_mem_to_reg;

reg[31:0] w_result;


//IF
assign f_pc=pc_reg;

InstructionRAM instruction_ram(
  .CLOCK(clk),
  .ENABLE(1'b1),
  .RESET(rst),
  .FETCH_ADDRESS(f_pc>>2),
  .DATA(instruction)
);



always @(posedge clk)
begin
  case(rst)
  1:
    pc_reg<=32'b0;

  0:
  begin
    if(stall==0)
    begin
      if(pc_jump)
        pc_reg<={d_pc[31:28],d_inst[25:0],2'b0};
      else if(pc_jr)
        pc_reg<=fwd_rd_A;
      else if(pc_branch)
        pc_reg<=d_pc_plus4+(d_extend_imm<<2);
      else//normal case
        pc_reg<=pc_reg+4;
    end
  end
  endcase
end

//ID
always @(posedge clk)
begin
  case(d_rst)
  1:
  begin
    d_inst<=0;
    opcode<=0;
    rs<=0;
    rt<=0;

    d_extend_imm<=0;
    d_extend_shamt<=0;
    funct<=0;

    d_pc<=0;
    d_pc_plus4<=0;
  end
  0:
  begin
    if(stall==0)
    begin
      d_pc<=f_pc;
      d_pc_plus4<=f_pc+4;

      d_inst<=instruction;
      opcode<=instruction[31:26];
      rs<=instruction[25:21];
      rt<=instruction[20:16];
      funct<=instruction[5:0];
    end
  end
  endcase
end

ControlUnit control_unit(
  .opcode(opcode),
  .rs_address(rs),
  .rt_address(rt),
  .funct(funct),

  .e_is_load(e_is_load),
  .e_reg_write(e_reg_write),
  .e_write_reg(e_write_reg),

  .m_is_load(m_is_load),
  .m_reg_write(m_reg_write),
  .m_write_reg(m_write_reg),

  .is_equal(is_equal),

  .d_dr_A(d_dr_A),
  .e_alu_out_A(e_alu_out_A),
  .m_alu_out_A(m_alu_out_A),
  .m_read_data_A(m_read_data_A),

  .d_dr_B(d_dr_B),
  .e_alu_out_B(e_alu_out_B),
  .m_alu_out_B(m_alu_out_B),
  .m_read_data_B(m_read_data_B),

  .m_forward(d_m_forward),
  .pc_jump(pc_jump),
  .pc_jr(pc_jr),
  .pc_branch(pc_branch),

  .alu_control(d_alu_control),
  .alu_src_A(d_alu_src_A),
  .alu_src_B(d_alu_src_B),
  .reg_dst(d_reg_dst),

  .extend_sign(d_extend_sign),
  .is_load(d_is_load),
  .is_display(d_is_display),
  .mem_to_reg(d_mem_to_reg),
  .mem_write(d_mem_write),
  .mem_used(d_mem_used),
  .reg_write(d_reg_write),
  .stall(stall),
  .d_rst(d_rst)
);

always @(*)
begin
  if(d_extend_sign)
    d_extend_imm<={{16{d_inst[15]}},d_inst[15:0]};
  else
    d_extend_imm<={16'b0,d_inst[15:0]};
  d_extend_shamt<={27'b0,d_inst[10:6]};
end

RegisterFile reg_file(
  .clk(clk),

  .a1(rs),
  .a2(rt),
  .a3(w_write_reg),
  .reg_write(w_reg_write),
  .wd3(w_result),

  .rd1(d_drA),
  .rd2(d_drB)
);

always @(*)
begin
  if(e_alu_out_A==1)
    fwd_rd_A=e_alu_out;
  else if(m_alu_out_A==1)
    fwd_rd_A=m_alu_out;
  else if(m_read_data_A==1)
    fwd_rd_A=m_read_data;
  else
    fwd_rd_A=d_drA;

  if(e_alu_out_B==1)
    fwd_rd_B=e_alu_out;
  else if(m_alu_out_B==1)
    fwd_rd_B=m_alu_out;
  else if(m_read_data_B==1)
    fwd_rd_B=m_read_data;
  else
    fwd_rd_B=d_drB;
end

assign is_equal=(fwd_rd_A==fwd_rd_B)?1:0;

//EXE
always @(posedge clk)
begin
  case(stall)
  1:
  begin
    e_is_load<=0;
    e_is_display<=0;
    e_reg_write<=0;

    e_extend_imm<=0;
    e_extend_shamt<=0;

    e_alu_control<=0;
    e_alu_src_A<=0;
    e_alu_src_B<=0;
    e_reg_dst<=0;
    e_m_forward<=0;

    e_rdA<=0;
    e_rdB<=0;
    e_pc_plus4<=0;

    e_mem_to_reg<=0;
    e_mem_write<=0;
    e_mem_used<=0;

    e_rt<=0;
    e_rd<=0;
  end

  default:
  begin
    e_is_load<=d_is_load;
    e_is_display<=d_is_display;
    e_reg_write<=d_reg_write;

    e_extend_imm<=d_extend_imm;
    e_extend_shamt<=d_extend_shamt;

    e_alu_control<=d_alu_control;
    e_alu_src_A<=d_alu_src_A;
    e_alu_src_B<=d_alu_src_B;
    e_reg_dst<=d_reg_dst;
    e_m_forward<=d_m_forward;

    e_rdA<=fwd_rd_A;
    e_rdB<=fwd_rd_B;
    e_pc_plus4<=d_pc_plus4;

    e_mem_to_reg<=d_mem_to_reg;
    e_mem_write<=d_mem_write;
    e_mem_used<=d_mem_used;

    e_rt<=d_inst[20:16];
    e_rd<=d_inst[15:11];
  end
  endcase
end

always @(*)
begin
  if(e_alu_src_A==0)
    e_src_A=e_rdA;
  else if(e_alu_src_A==1)
    e_src_A=e_extend_shamt;
  else if(e_alu_src_A==2)
    e_src_A=e_pc_plus4;

  if(e_alu_src_B==0)
    e_src_B=e_rdB;
  else if(e_alu_src_B==1)
    e_src_B=e_extend_imm;
  else if(e_alu_src_B==2)
    e_src_B=32'b0;

  if(e_reg_dst==0)
    e_write_reg=e_rd;
  else if(e_reg_dst==1)
    e_write_reg=e_rt;
  else if(e_reg_dst==2)
    e_write_reg=31;//register ra
end

assign e_write_data=e_rdB;

alu ALU(
  .alu_control(e_alu_control),
  .a(e_src_A),
  .b(e_src_B),

  .result(e_alu_out),
  .flags(flags)
);

//MEM
always @(posedge clk)
begin
  m_m_forward<=e_m_forward;
  if(m_m_forward)
    m_write_data<=w_result;
  else
    m_write_data<=e_write_data;
  
  m_mem_to_reg<=e_mem_to_reg;
  m_mem_write<=e_mem_write;
  m_mem_used<=e_mem_used;
  m_is_load<=e_is_load;
  m_alu_out<=e_alu_out;
  m_write_reg<=e_write_reg;
  m_reg_write<=e_reg_write;
  m_is_display<=e_is_display;
end

MainMemory main_mem(
  .CLOCK(clk),
  .ENABLE(m_mem_used),
  .FETCH_ADDRESS(m_alu_out>>2),
  .EDIT_SERIAL({m_mem_write,m_alu_out>>2,m_write_data}),
  .IS_DISPLAY(m_is_display),
  .DATA(m_read_data)
);

//WB
always @(posedge clk)
begin
  w_reg_write<=m_reg_write;
  w_mem_to_reg<=m_mem_to_reg;
  w_alu_out<=m_alu_out;
  w_read_data<=m_read_data;
  w_write_reg<=m_write_reg;
end

always @(*)
begin
  if(w_mem_to_reg==0) 
    w_result=w_alu_out;
  else
    w_result=w_read_data;
end

assign is_display=m_is_display;

endmodule