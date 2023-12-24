module alu(
    input wire [3:0] alu_control,
    input wire [31:0] a,
    input wire [31:0] b,
    
    output reg [31:0] result,
    output reg [2:0] flags
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

reg[31:0] temp;

always @(*)
begin
    temp=0;
    result=0;
    flags=0;
    case(alu_control)
    0://add
    begin
        result=a+b;
        if(!a[31]&&!b[31]&&result[31]||a[31]&&b[31]&&!result[31])
            flags[0]=1;
        else
            flags[0]=0;
    end

    1://addu
    begin
        result=a+b;
    end

    2://sub
    begin
        result=a-b;
        if(!a[31]&&b[31]&&result[31]||a[31]&&!b[31]&&!result[31])
            flags[0]=1;
        else
            flags[0]=0;
    end

    3://subu
    begin
        result=a-b;
    end

    4://and
    begin
        result=a&b;
    end

    5://nor
    begin
        result=~(a|b);
    end

    6://or
    begin
        result=a|b;
    end

    7://xor
    begin
        result=a^b;
    end

    8://slt
    begin
        temp=a-b;
        flags[1]=temp[31];
        result=temp[31];
    end

    9://sltu
    begin
        flags[1]=(a<b);
        result={31'b0,flags[1]};
    end

    10://sll
    begin
        result=b<<a;
    end

    11://sllv
    begin
        result=b<<a[3:0];
    end

    12://srl
    begin
        result=b>>a;
    end
    13://srlv
    begin
        result=b>>a[3:0];
    end

    14://sra
    begin
        result=$signed(b)>>>a;
    end

    15://srav
    begin
        result=$signed(b)>>>a[3:0];
    end
    endcase
end

endmodule