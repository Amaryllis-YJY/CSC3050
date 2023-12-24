module ControlUnit(
    input wire[5:0] opcode,
    input wire[4:0] rs_address,
    input wire[4:0] rt_address,
    input wire[5:0] funct,

    input wire e_is_load,
    input wire e_reg_write,
    input wire[4:0] e_write_reg,
    input wire m_is_load,
    input wire m_reg_write,
    input wire[4:0] m_write_reg,
    
    input wire is_equal,//state that whether rs and rt are equal
    
    output reg d_dr_A,
    output reg e_alu_out_A,
    output reg m_alu_out_A,
    output reg m_read_data_A,

    output reg d_dr_B,
    output reg e_alu_out_B,
    output reg m_alu_out_B,
    output reg m_read_data_B,
    
    output reg m_forward,

    //output reg pc_plus4,
    output reg pc_jump,
    output reg pc_jr,
    output reg pc_branch,

    output reg[3:0] alu_control,
    output reg[1:0] alu_src_A,//0 for rs, 1 for sa, 2 for link
    output reg[1:0] alu_src_B,//0 for rt, 1 for imm, 2 for link
    output reg[1:0] reg_dst,//0 for rd, 1 for rt, 2 for link

    output reg extend_sign,
    output reg is_load,
    output reg is_display,
    output reg mem_to_reg,
    output reg mem_write,
    output reg mem_used,
    output reg reg_write,
    output reg stall,
    output reg d_rst
);

/* alu_control list:
add=0,
addu=1,
sub=2,
subu=3,
and=4,
nor=5,
or=6,
xor=7,
slt=8
sltu=9,
sll=10,
sllv=11,
srl=12,
srlv=13,
sra=14,
srav=15.*/

reg used_rs,used_rt;
reg is_link,is_store;

always @(*)
begin
    reg_write=0;
    mem_to_reg=0;
    reg_dst=0;
    extend_sign=0;
    is_load=0;
    mem_write=0;
    mem_used=0;

    alu_src_A=0;
    alu_src_B=0;

    used_rs=0;
    used_rt=0;
    is_link=0;
    is_store=0;
    is_display=0;

    //pc_plus4=1;
    pc_jr=0;
    pc_branch=0;
    pc_jump=0;

    case(opcode)
    6'b000000:
    begin
        case(funct)        
        6'b100000://add
        begin
            reg_write=1;
            alu_control=0;
            used_rs=1;
            used_rt=1;
        end

        6'b100001://addu
        begin
            reg_write=1;
            alu_control=1;
            used_rs=1;
            used_rt=1;
        end

        6'b100010://sub
        begin
            reg_write=1;
            alu_control=2;
            used_rs=1;
            used_rt=1;
        end

        6'b100011://subu
        begin
            reg_write=1;
            alu_control=3;
            used_rs=1;
            used_rt=1;
        end

        6'b100100://and
        begin
            reg_write=1;
            alu_control=4;
            used_rs=1;
            used_rt=1;
        end

        6'b100111://nor
        begin
            reg_write=1;
            alu_control=5;
            used_rs=1;
            used_rt=1;
        end

        6'b100101://or
        begin
            reg_write=1;
            alu_control=6;
            used_rs=1;
            used_rt=1;
        end

        6'b100110://xor
        begin
            reg_write=1;
            alu_control=7;
            used_rs=1;
            used_rt=1;
        end

        6'b101010://slt
        begin
            reg_write=1;
            alu_control=8;
            used_rs=1;
            used_rt=1;
        end
        6'b101011://sltu
        begin
            reg_write=1;
            alu_control=9;
            used_rs=1;
            used_rt=1;
        end

        6'b000000://sll
        begin
            reg_write=1;
            alu_control=10;
            alu_src_A=1;
            used_rs=1;
            used_rt=1;
        end

        6'b000100://sllv
        begin
            reg_write=1;
            alu_control=11;
            used_rs=1;
            used_rt=1;
        end

        6'b000010://srl
        begin
            reg_write=1;
            alu_control=12;
            alu_src_A=1;
            used_rs=1;
            used_rt=1;
        end

        6'b000110://srlv
        begin
            reg_write=1;
            alu_control=13;
            used_rs=1;
            used_rt=1;
        end

        6'b000011://sra
        begin
            reg_write=1;
            alu_control=14;
            alu_src_A=1;
            used_rs=1;
            used_rt=1;
        end

        6'b000111://srav
        begin
            reg_write=1;
            alu_control=15;
            used_rs=1;
            used_rt=1;
        end

        6'b001000://jr
        begin
            reg_write=0;
            pc_jr=1;
            used_rs=1;
            is_link=1;
        end
        endcase
    end

    6'b001000://addi
    begin
        reg_write=1;
        alu_control=0;
        alu_src_B=1;
        used_rs=1;
        reg_dst=1;
        extend_sign=1;
    end

    6'b001001://addiu
    begin
        reg_write=1;
        alu_control=1;
        alu_src_B=1;
        used_rs=1;
        reg_dst=1;
    end

    6'b001100://andi
    begin
        reg_write=1;
        alu_control=4;
        alu_src_B=1;
        used_rs=1;
        reg_dst=1;
    end

    6'b000100://beq
    begin
        if(is_equal)
        begin
            pc_branch=1;
            is_link=1;
        end
        extend_sign=1;
        used_rs=1;
        used_rt=1;
    end

    6'b000101://bne
    begin
        if(~is_equal)
        begin
            pc_branch=1;
            is_link=1;
        end
        extend_sign=1;
        used_rs=1;
        used_rt=1;
    end

    6'b001101://ori
    begin
        reg_write=1;
        alu_control=6;
        alu_src_B=1;
        used_rs=1;
        reg_dst=1;
    end

    6'b001110://xori
    begin
        reg_write=1;
        alu_control=7;
        alu_src_B=1;
        used_rs=1;
        reg_dst=1;      
    end

    6'b001010://slti
    begin
        reg_write=1;
        alu_control=8;
        alu_src_B=1;
        used_rs=1;
        reg_dst=1;
    end

    6'b001011://sltiu
    begin
        reg_write=1;
        alu_control=9;
        alu_src_B=1;
        used_rs=1;
        reg_dst=1;
    end

    6'b100011://lw
    begin
        reg_write=1;
        alu_control=0;
        alu_src_B=1;
        mem_to_reg=1;
        mem_used=1;
        reg_dst=1;
        is_load=1;
        used_rs=1;        
        extend_sign=1;
    end

    6'b101011://sw
    begin
        alu_control=0;
        alu_src_B=1;
        mem_used=1;
        mem_write=1;
        is_store=1;
        used_rs=1;
        used_rt=1;
    end

    6'b000010://j
    begin
        pc_jump=1;
        is_link=1;
    end

    6'b000011://jal
    begin
        reg_write=1;
        alu_control=0;
        pc_jump=1;
        alu_src_A=2;
        alu_src_B=2;
        mem_to_reg=0;
        reg_dst=2;
        is_link=1;
    end

    6'b111111://stop
    begin
        is_display=1;
    end
    endcase
end

always @(*)
begin
    stall=0;
    m_forward=0;
    d_dr_A=1;
    e_alu_out_A=0;
    m_alu_out_A=0;
    m_read_data_A=0;

    d_rst=0;
    d_dr_B=1;
    e_alu_out_B=0;
    m_alu_out_B=0;
    m_read_data_B=0;

    if(used_rs&&rs_address!=0)
    begin       
        if(rs_address==e_write_reg&&e_reg_write)
        begin
            if(e_is_load)
                stall=1;
            else
            begin
                e_alu_out_A=1;
                d_dr_A=0;
            end
        end
        else if(rs_address==m_write_reg&&m_reg_write)
        begin
            if(m_is_load)
            begin
                m_read_data_A=1;
                d_dr_A=0;
            end
            else
            begin
                m_alu_out_A=1;
                d_dr_A=0;
            end
        end
    end

    if(used_rt&&rt_address!=0)
    begin
        if(rt_address==e_write_reg&&e_reg_write)
        begin
            if(e_is_load)
            begin
                if(is_store==0)
                    stall=1;
                else
                    m_forward=1;
            end
            else
            begin
                e_alu_out_B=1;
                d_dr_B=0;
            end
        end
        else if(rt_address==m_write_reg&&m_reg_write)
        begin
            if(m_is_load)
            begin
                m_read_data_B=1;
                d_dr_B=0;
            end
            else
            begin
                m_alu_out_B=1;
                d_dr_B=0;
            end
        end
    end
    if(is_link)
        d_rst=1;
end


endmodule