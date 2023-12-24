module alu(instruction,regA,regB,result,flags);

input[31:0] instruction,regA,regB;
output[31:0] result;
output[2:0] flags;

reg[2:0] flags;
reg[5:0] opcode,funct;
reg[31:0] rd,rs,rt;
reg[31:0] imm;
reg[5:0] shamt;


always @(*)
begin
    opcode=instruction[31:26];
    funct=instruction[5:0];
    flags=3'b000;
    if(instruction[25:21]=={5'b0})
    begin
        rs=regA;
        rt=regB;
    end
    else
    begin
        rs=regB;
        rt=regA;
    end
    shamt=instruction[10:6];

    case(opcode)

    6'b000000:
    begin
        case(funct)
        
        6'b100000://add
        begin
            rd=rs+rt;
            if(!rs[31]&&!rt[31]&&rd[31]||rs[31]&&rt[31]&&!rd[31])
                flags[0]=1;
            else
                flags[0]=0;
        end

        6'b100001://addu
        begin
            rd=rs+rt;
        end

        6'b100010://sub
        begin
            rd=rs-rt;
            if(!rs[31]&&rt[31]&&rd[31]||rs[31]&&!rt[31]&&!rd[31])
                flags[0]=1;
            else
                flags[0]=0;
        end

        6'b100011://subu
        begin
            rd=rs-rt;
        end

        6'b100100://and
        begin
            rd=rs&rt;
        end

        6'b100111://nor
        begin
            rd=~(rs|rt);
        end

        6'b100101://or
        begin
            rd=rs|rt;
        end

        6'b100110://xor
        begin
            rd=rs^rt;
        end

        6'b101010://slt
        begin
            rd=rs-rt;
            flags[1]=rd[31];
        end

        6'b101011://sltu
        begin
            rd=rs-rt;
            flags[1]=(rs<rt);
        end

        6'b000000://sll
        begin
            rd=rt<<shamt;
        end

        6'b000100://sllv
        begin
            rd=rt<<rs;
        end

        6'b000010://srl
        begin
            rd=rt>>shamt;
        end

        6'b000110://srlv
        begin
            rd=rt>>rs;
        end

        6'b000011://sra
        begin
            rd=rt>>>shamt;
        end

        6'b000111://srav
        begin
            rd=rt>>>rs;
        end

        endcase
    end

    6'b001000://addi
    begin
        imm=$signed(instruction[15:0]);
        rd=rs+imm;
        if(!rs[31]&&rd[31]||rs[31]&&!rd[31])
            flags[0]=1;
        else
            flags[0]=0;
    end

    6'b001001://addiu
    begin
        imm=instruction[15:0];
        rd=rs+imm;
    end

    6'b001100://andi
    begin
        imm=instruction[15:0];
        rd=rs&imm;
    end

    6'b000100://beq
    begin
        rd=rs-rt;
        if(rd=={32'b0})
            flags[2]=1;
        else
            flags[2]=0;
    end

    6'b000101://bne
    begin
        rd=rs-rt;
        if(rd=={32'b0})
            flags[2]=1;
        else
            flags[2]=0;
    end

    6'b001101://ori
    begin
        imm=instruction[15:0];
        rd=rs|imm;
    end

    6'b001110://xori
    begin
        imm=instruction[15:0];
        rd=rs^imm;
    end

    6'b001010://slti
    begin
        imm=$signed(instruction[15:0]);
        rd=rs-imm;
        flags[1]=rd[31];
    end

    6'b001011://sltiu
    begin
        imm=instruction[15:0];
        rd=rs-imm;
        flags[1]=(rs<imm);
    end

    6'b100011://lw
    begin
        imm=$signed(instruction[15:0]);
        rd=rs+imm;
    end

    6'b101011://sw
    begin
        imm=$signed(instruction[15:0]);
        rd=rs+imm;
    end

    endcase

end

assign result=rd[31:0];

endmodule;