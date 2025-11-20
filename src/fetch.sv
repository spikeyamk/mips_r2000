module pc_register (
    input  var logic                        clk   ,
    input  var logic                        rst   ,
    input  var logic [Constants::WIDTH-1:0] pc_in ,
    output var logic [Constants::WIDTH-1:0] pc_out
);
    always_ff @ (posedge clk, negedge rst) begin
        if (!rst) begin
            pc_out <= 0;
        end else begin
            pc_out <= pc_in;
        end
    end
endmodule

module pc_advancer (
    input  var logic [Constants::WIDTH-1:0] pc_in        ,
    input  var logic                        stall        ,
    input  var logic                        branch_taken ,
    input  var logic [Constants::WIDTH-1:0] branch_target,
    output var logic [Constants::WIDTH-1:0] pc_out   
);
    always_comb begin
        if (branch_taken) begin
            pc_out = branch_target + 4;
        end else if (stall) begin
            pc_out = pc_in;
        end else begin
            pc_out = pc_in + 4;
        end
    end
endmodule

module instruction_memory (
    input  var logic [Constants::WIDTH-1:0] pc,
    input  var logic                        branch_taken,
    input  var logic [Constants::WIDTH-1:0] branch_target,
    input  var logic [Constants::BYTE-1:0]  rom [0:Constants::ROM_SIZE-1],
    output var logic [Constants::WIDTH-1:0] out           
);
    always_comb begin
        if (branch_taken) begin
            out = {
                rom[branch_target + 0],
                rom[branch_target + 1],
                rom[branch_target + 2],
                rom[branch_target + 3]
            };
        end else begin
            out = {
                rom[pc + 0],
                rom[pc + 1],
                rom[pc + 2],
                rom[pc + 3]
            };
        end
    end
endmodule

module fetch_buffer (
    input  var logic                        clk            ,
    input  var logic                        rst            ,
    input  var logic                        stall          ,
    input  var logic [Constants::WIDTH-1:0] pc_in          ,
    input  var logic                        branch_taken   ,
    input  var logic [Constants::WIDTH-1:0] branch_target  ,
    input  var logic [Constants::WIDTH-1:0] instruction_in ,
    output var logic [Constants::WIDTH-1:0] pc_out         ,
    output var logic [Constants::WIDTH-1:0] instruction_out
);
    always_ff @ (posedge clk, negedge rst) begin
        if (!rst) begin
            pc_out          <= 32'hffff_fffb;
            instruction_out <= 0;
        end else if (stall) begin
            pc_out          <= pc_in;
            instruction_out <= 0;
        end else begin
            if (branch_taken) begin
                pc_out <= branch_target;
            end else begin
                pc_out <= pc_in;
            end
            instruction_out <= instruction_in;
        end
    end
endmodule

module fetch (
    input  var logic                        clk                ,
    input  var logic                        rst                ,
    input  var logic [Constants::BYTE-1:0]  rom     [0:Constants::ROM_SIZE-1] ,
    input  var logic                        stall              ,
    input  var logic                        branch_taken       ,
    input  var logic [Constants::WIDTH-1:0] branch_target      ,
    output var logic [Constants::WIDTH-1:0] pc_fetched         ,
    output var logic [Constants::WIDTH-1:0] instruction_fetched
);
    logic [Constants::WIDTH-1:0] pc_advanced;
    logic [Constants::WIDTH-1:0] pc         ;

    pc_register pc_register_inst (
        .clk    (clk        ),
        .rst    (rst        ),
        .pc_in  (pc_advanced),
        .pc_out (pc         )
    );

    pc_advancer pc_advancer_inst (
        .pc_in         (pc           ),
        .stall         (stall        ),
        .branch_taken  (branch_taken ),
        .branch_target (branch_target),
        .pc_out        (pc_advanced  )
    );

    logic [Constants::WIDTH-1:0] instruction;
    instruction_memory instruction_memory_inst (
        .pc (pc          ),
        .branch_taken (branch_taken),
        .branch_target (branch_target),
        .rom     (rom),
        .out     (instruction )
    );

    fetch_buffer fetch_buffer_inst (
        .clk             (clk                ),
        .rst             (rst                ),
        .stall           (stall              ),
        .pc_in           (pc                 ),
        .branch_taken    (branch_taken       ),
        .branch_target   (branch_target      ),
        .instruction_in  (instruction        ),
        .pc_out          (pc_fetched         ),
        .instruction_out (instruction_fetched)
    );
endmodule