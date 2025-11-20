package Decode;
    typedef enum logic [2-1:0] {
        LoadStoreDataSizeMode_BYTE = $bits(logic [2-1:0])'(2'b00),
        LoadStoreDataSizeMode_HALF_WORD = $bits(logic [2-1:0])'(2'b01),
        LoadStoreDataSizeMode_WORD = $bits(logic [2-1:0])'(2'b11)
    } LoadStoreDataSizeMode;

    typedef enum logic [3-1:0] {
        BranchMode_BLTZ = $bits(logic [3-1:0])'(3'b000),
        BranchMode_BGEZ = $bits(logic [3-1:0])'(3'b001),
        BranchMode_BEQ = $bits(logic [3-1:0])'(3'b100),
        BranchMode_BNE = $bits(logic [3-1:0])'(3'b101),
        BranchMode_BLEZ = $bits(logic [3-1:0])'(3'b110),
        BranchMode_BGTZ = $bits(logic [3-1:0])'(3'b111)
    } BranchMode;

    typedef enum logic [5-1:0] {
        ALUMode_ADD = $bits(logic [5-1:0])'(5'b1_0000),
        ALUMode_ADDU = $bits(logic [5-1:0])'(5'b1_0001),
        ALUMode_SUB = $bits(logic [5-1:0])'(5'b1_0010),
        ALUMode_SUBU = $bits(logic [5-1:0])'(5'b1_0011),
        ALUMode_AND = $bits(logic [5-1:0])'(5'b1_0100),
        ALUMode_OR = $bits(logic [5-1:0])'(5'b1_0101),
        ALUMode_XOR = $bits(logic [5-1:0])'(5'b1_0110),
        ALUMode_NOR = $bits(logic [5-1:0])'(5'b1_0111),
        ALUMode_SLL = $bits(logic [5-1:0])'(5'b0_0100),
        ALUMode_SRL = $bits(logic [5-1:0])'(5'b0_0110),
        ALUMode_SRA = $bits(logic [5-1:0])'(5'b0_0111),
        ALUMode_SLT = $bits(logic [5-1:0])'(5'b1_1010),
        ALUMode_SLTU = $bits(logic [5-1:0])'(5'b1_1011)
    } ALUMode;
endpackage

module parser (
    input var logic [Constants::WIDTH-1:0] instruction,

    output var logic                                 rs        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rs_address,
    output var logic                                 rt        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rt_address,
    output var logic                                 rd        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address,

    output var logic                               shamt       ,
    output var logic [Constants::SHAMT_WIDTH-1:0]  shamt_value ,
    output var logic                               imm         ,
    output var logic [Constants::IMM_WIDTH-1:0]    imm_value   ,
    output var logic                               target      ,
    output var logic [Constants::TARGET_WIDTH-1:0] target_value,

    output var logic         alu_mode      ,
    output var logic [5-1:0] alu_mode_value,

    output var logic         link       ,
    output var logic         branch     ,
    output var logic [3-1:0] branch_mode,
    output var logic         jump       ,

    output var logic         lui                      ,
    output var logic         load                     ,
    output var logic         load_sign_extend         ,
    output var logic [2-1:0] load_store_data_size_mode,
    output var logic         store                
);
    always_comb begin

        rs         = 0;
        rs_address = 0;
        rt         = 0;
        rt_address = 0;
        rd         = 0;
        rd_address = 0;

        shamt        = 0;
        shamt_value  = 0;
        imm          = 0;
        imm_value    = 0;
        target       = 0;
        target_value = 0;

        alu_mode       = 0;
        alu_mode_value = 0;

        link        = 0;
        branch      = 0;
        branch_mode = 0;
        jump        = 0;

        lui                       = 0;
        load                      = 0;
        load_sign_extend          = 0;
        load_store_data_size_mode = 0;
        store                     = 0;

        if (instruction[31:27] == 5'b00001) begin
            // j target, jal target
            jump         = 1;
            target       = 1;
            target_value = instruction[25:0];
            if (instruction[26]) begin
                link           = 1;
                alu_mode       = 1;
                alu_mode_value = Decode::ALUMode_ADDU;
                rd             = 1;
                rd_address     = 31;
            end
        end
        if (instruction[31:26] == 6'b000000) begin
            if (instruction[25:21] == 5'b00000) begin
                if ((instruction[5:2] == 4'b0000) && (instruction[1:0] != 2'b01)) begin
                    // Immediate Register Shift Operations
                    rs             = 1;
                    rs_address     = instruction[20:16];
                    rd             = 1;
                    rd_address     = instruction[15:11];
                    shamt          = 1;
                    shamt_value    = instruction[10:6];
                    alu_mode       = 1;
                    alu_mode_value = {1'b0, 1'b0, 1'b1, instruction[1:0]};
                end
            end
            if (instruction[20:16] == 5'b00000) begin
                // Register Jump Operations
                if (instruction[15:0] == 16'b00000_00000001000) begin
                    // jr rs
                    jump       = 1;
                    rs         = 1;
                    rs_address = instruction[25:21];
                end
                if (instruction[15:0] == 16'b11111_00000001001) begin
                    // jalr rs
                    jump           = 1;
                    link           = 1;
                    alu_mode       = 1;
                    alu_mode_value = Decode::ALUMode_ADDU;
                    rd             = 1;
                    rd_address     = 31;
                    rs             = 1;
                    rs_address     = instruction[25:21];
                end
            end
            if (instruction[10:6] == 5'b00000) begin
                if (instruction[5:4] == 2'b10) begin
                    if (instruction[3] == 0) begin
                        // Register Arithmetic, Logic Operations
                        rs             = 1;
                        rs_address     = instruction[25:21];
                        rt             = 1;
                        rt_address     = instruction[20:16];
                        rd             = 1;
                        rd_address     = instruction[15:11];
                        alu_mode       = 1;
                        alu_mode_value = {instruction[5], instruction[3:0]};
                    end
                    if (instruction[3:1] == 3'b101) begin
                        // Register Comparison Operations
                        alu_mode       = 1;
                        alu_mode_value = {instruction[5], instruction[3:0]};
                        rs             = 1;
                        rs_address     = instruction[25:21];
                        rt             = 1;
                        rt_address     = instruction[20:16];
                        rd             = 1;
                        rd_address     = instruction[15:11];
                    end
                end
                if ((instruction[5:2] == 4'b0001) && (instruction[1:0] != 2'b01)) begin
                    // Register Shift Operations
                    rs             = 1;
                    rs_address     = instruction[20:16];
                    rt             = 1;
                    rt_address     = instruction[25:21];
                    rd             = 1;
                    rd_address     = instruction[15:11];
                    alu_mode       = 1;
                    alu_mode_value = {instruction[5], instruction[3:0]};
                end
            end
        end
        if (instruction[31:29] == 3'b001) begin
            if (instruction[28:26] != 3'b111) begin
                if (instruction[31:27] == 5'b00101) begin
                    // Immediate Comparison Operations
                    rs             = 1;
                    rs_address     = instruction[25:21];
                    rd             = 1;
                    rd_address     = instruction[20:16];
                    imm            = 1;
                    imm_value      = instruction[15:0];
                    alu_mode       = 1;
                    alu_mode_value = {1'b1, instruction[29:26]};
                end else begin
                    // Immediate Arithmetic, Logic Operations
                    rs             = 1;
                    rs_address     = instruction[25:21];
                    rd             = 1;
                    rd_address     = instruction[20:16];
                    imm            = 1;
                    imm_value      = instruction[15:0];
                    alu_mode       = 1;
                    alu_mode_value = {1'b1, 1'b0, instruction[28:26]};
                end
            end
            if (instruction[28:26] == 3'b111) begin
                if (instruction[25:21] == 5'b00000) begin
                    // lui rt, imm
                    alu_mode       = 1;
                    alu_mode_value = Decode::ALUMode_SLL;
                    lui            = 1;
                    rd             = 1;
                    rd_address     = instruction[20:16];
                    imm            = 1;
                    imm_value      = instruction[15:0];
                end
            end
        end
        if (instruction[31:26] == 6'b000001) begin
            if (instruction[19:17] == 3'b000) begin
                // Branch with zero operations
                branch      = 1;
                branch_mode = instruction[18:16];
                if (instruction[20] == 1) begin
                    link           = 1;
                    alu_mode       = 1;
                    alu_mode_value = Decode::ALUMode_ADDU;
                    rd             = 1;
                    rd_address     = 31;
                end
                imm        = 1;
                imm_value  = instruction[15:0];
                rs         = 1;
                rs_address = instruction[25:21];
                rt         = 1;
                rt_address = 0;
            end
        end
        if (instruction[31:28] == 4'b0001) begin
            if (instruction[27] == 0) begin
                // Branch Operations
                rs          = 1;
                rs_address  = instruction[25:21];
                rt          = 1;
                rt_address  = instruction[20:16];
                imm         = 1;
                imm_value   = instruction[15:0];
                branch      = 1;
                branch_mode = instruction[28:26];
            end
            if (instruction[20:16] == 5'b00000) begin
                // Branch Operations
                rs          = 1;
                rs_address  = instruction[25:21];
                rt          = 1;
                rt_address  = 0;
                imm         = 1;
                imm_value   = instruction[15:0];
                branch      = 1;
                branch_mode = instruction[28:26];
            end
        end
        if (instruction[31] == 1) begin
            if (instruction[30] == 0) begin
                if (instruction[29] == 0) begin
                    if (instruction[27:26] != 2'b10) begin
                        // Load operations
                        rs                        = 1;
                        rs_address                = instruction[25:21];
                        rd                        = 1;
                        rd_address                = instruction[20:16];
                        imm                       = 1;
                        imm_value                 = instruction[15:0];
                        load                      = 1;
                        load_store_data_size_mode = instruction[27:26];
                        load_sign_extend          = ~(instruction[28]);
                    end
                end
                if (instruction[29:28] == 2'b10) begin
                    if (instruction[27:26] != 2'b10) begin
                        // Store operations
                        store                     = 1;
                        load_store_data_size_mode = instruction[27:26];
                        rs                        = 1;
                        rs_address                = instruction[25:21];
                        rt                        = 1;
                        rt_address                = instruction[20:16];
                        imm                       = 1;
                        imm_value                 = instruction[15:0];
                    end
                end
            end
        end
    end
endmodule

module registers (
    input var logic clk,
    input var logic rst,

    input var logic [Constants::REG_ADDR_WIDTH-1:0] rs_address,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rt_address,

    input var logic                                 rd        ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address,
    input var logic [Constants::WIDTH-1:0]          rd_data   ,

    output var logic [Constants::WIDTH-1:0] rs_data,
    output var logic [Constants::WIDTH-1:0] rt_data,
    output var logic [Constants::WIDTH-1:0] reg_file [0:Constants::REG_COUNT - 1-1]
);
    always_comb begin
        rs_data = (
            (rs_address ==? 0) ? 0 : (
                rd && (rd_address == rs_address)
                ? rd_data
                : reg_file[rs_address - 1]
            )
        );
        rt_data = (
            (rt_address ==? 0) ? 0 : (
                rd && (rd_address == rt_address)
                ? rd_data
                : reg_file[rt_address - 1]
            )
        );
    end

    always_ff @ (posedge clk, negedge rst) begin
        if (!rst) begin
            for (int unsigned i = 0; i < (Constants::REG_COUNT - 1); i++) begin
                reg_file[i] <= 0;
            end
        end else if (rd && (rd_address != 0)) begin
            reg_file[rd_address - 1] <= rd_data;
        end
    end
endmodule

module decode_buffer (
    input var logic clk,
    input var logic rst,

    input var logic [Constants::WIDTH-1:0] pc_in,

    input var logic                                 rs_in        ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rs_address_in,
    input var logic [Constants::WIDTH-1:0]          rs_data_in   ,
    input var logic                                 rt_in        ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rt_address_in,
    input var logic [Constants::WIDTH-1:0]          rt_data_in   ,
    input var logic                                 rd_in        ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_in,

    input var logic                               shamt_in       ,
    input var logic [Constants::SHAMT_WIDTH-1:0]  shamt_value_in ,
    input var logic                               imm_in         ,
    input var logic [Constants::IMM_WIDTH-1:0]    imm_value_in   ,
    input var logic                               target_in      ,
    input var logic [Constants::TARGET_WIDTH-1:0] target_value_in,

    input var logic         alu_mode_in      ,
    input var logic [5-1:0] alu_mode_value_in,

    input var logic         link_in       ,
    input var logic         branch_in     ,
    input var logic [3-1:0] branch_mode_in,
    input var logic         jump_in       ,

    input var logic         lui_in                      ,
    input var logic         load_in                     ,
    input var logic         load_sign_extend_in         ,
    input var logic [2-1:0] load_store_data_size_mode_in,
    input var logic         store_in                    ,

    output var logic [Constants::WIDTH-1:0] pc_out,

    output var logic                                 rs_out        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rs_address_out,
    output var logic [Constants::WIDTH-1:0]          rs_data_out   ,
    output var logic                                 rt_out        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rt_address_out,
    output var logic [Constants::WIDTH-1:0]          rt_data_out   ,
    output var logic                                 rd_out        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_out,

    output var logic                               shamt_out       ,
    output var logic [Constants::SHAMT_WIDTH-1:0]  shamt_value_out ,
    output var logic                               imm_out         ,
    output var logic [Constants::IMM_WIDTH-1:0]    imm_value_out   ,
    output var logic                               target_out      ,
    output var logic [Constants::TARGET_WIDTH-1:0] target_value_out,

    output var logic         alu_mode_out      ,
    output var logic [5-1:0] alu_mode_value_out,

    output var logic         link_out       ,
    output var logic         branch_out     ,
    output var logic [3-1:0] branch_mode_out,
    output var logic         jump_out       ,

    output var logic         lui_out                      ,
    output var logic         load_out                     ,
    output var logic         load_sign_extend_out         ,
    output var logic [2-1:0] load_store_data_size_mode_out,
    output var logic         store_out                
);
    always_ff @ (posedge clk, negedge rst) begin
        if (!rst) begin
            pc_out <= 0;

            rs_out         <= 0;
            rs_address_out <= 0;
            rs_data_out    <= 0;
            rt_out         <= 0;
            rt_address_out <= 0;
            rt_data_out    <= 0;
            rd_out         <= 0;
            rd_address_out <= 0;

            shamt_out        <= 0;
            shamt_value_out  <= 0;
            imm_out          <= 0;
            imm_value_out    <= 0;
            target_out       <= 0;
            target_value_out <= 0;

            alu_mode_out       <= 0;
            alu_mode_value_out <= 0;

            link_out        <= 0;
            branch_out      <= 0;
            branch_mode_out <= 0;
            jump_out        <= 0;

            lui_out                       <= 0;
            load_out                      <= 0;
            load_sign_extend_out          <= 0;
            load_store_data_size_mode_out <= 0;
            store_out                     <= 0;
        end else begin
            pc_out <= pc_in;

            rs_out         <= rs_in;
            rs_address_out <= rs_address_in;
            rs_data_out    <= rs_data_in;
            rt_out         <= rt_in;
            rt_address_out <= rt_address_in;
            rt_data_out    <= rt_data_in;
            rd_out         <= rd_in;
            rd_address_out <= rd_address_in;

            shamt_out        <= shamt_in;
            shamt_value_out  <= shamt_value_in;
            imm_out          <= imm_in;
            imm_value_out    <= imm_value_in;
            target_out       <= target_in;
            target_value_out <= target_value_in;

            alu_mode_out       <= alu_mode_in;
            alu_mode_value_out <= alu_mode_value_in;

            link_out        <= link_in;
            branch_out      <= branch_in;
            branch_mode_out <= branch_mode_in;
            jump_out        <= jump_in;

            lui_out                       <= lui_in;
            load_out                      <= load_in;
            load_sign_extend_out          <= load_sign_extend_in;
            load_store_data_size_mode_out <= load_store_data_size_mode_in;
            store_out                     <= store_in;
        end
    end
endmodule

module decode (
    input  var logic                        clk                ,
    input  var logic                        rst                ,
    input  var logic [Constants::BYTE-1:0]  rom     [0:Constants::ROM_SIZE-1] ,
    input  var logic                        stall              ,
    input  var logic                        branch_taken       ,
    input  var logic [Constants::WIDTH-1:0] branch_target      ,

    input var logic                                 rd_wb        ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_wb,
    input var logic [Constants::WIDTH-1:0]          rd_data_wb   ,

    output var logic [Constants::WIDTH-1:0] pc_decoded,

    output var logic                                 rs_decoded        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rs_address_decoded,
    output var logic [Constants::WIDTH-1:0]          rs_data_decoded   ,
    output var logic                                 rt_decoded        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rt_address_decoded,
    output var logic [Constants::WIDTH-1:0]          rt_data_decoded   ,
    output var logic                                 rd_decoded        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_decoded,

    output var logic                               shamt_decoded       ,
    output var logic [Constants::SHAMT_WIDTH-1:0]  shamt_value_decoded ,
    output var logic                               imm_decoded         ,
    output var logic [Constants::IMM_WIDTH-1:0]    imm_value_decoded   ,
    output var logic                               target_decoded      ,
    output var logic [Constants::TARGET_WIDTH-1:0] target_value_decoded,

    output var logic         alu_mode_decoded      ,
    output var logic [5-1:0] alu_mode_value_decoded,

    output var logic         link_decoded       ,
    output var logic         branch_decoded     ,
    output var logic [3-1:0] branch_mode_decoded,
    output var logic         jump_decoded       ,

    output var logic         lui_decoded                      ,
    output var logic         load_decoded                     ,
    output var logic         load_sign_extend_decoded         ,
    output var logic [2-1:0] load_store_data_size_mode_decoded,
    output var logic         store_decoded,

    output var logic [Constants::WIDTH-1:0] reg_file [0:Constants::REG_COUNT - 1-1]
);
    var logic [Constants::WIDTH-1:0] pc_fetched;
    var logic [Constants::WIDTH-1:0] instruction_fetched;

    fetch fetch_inst (
        .clk(clk),
        .rst(rst),
        .rom(rom),
        .stall(stall),
        .branch_taken(branch_taken),
        .branch_target(branch_target),
        .pc_fetched(pc_fetched),
        .instruction_fetched(instruction_fetched)
    );

    logic                                 rs        ;
    logic [Constants::REG_ADDR_WIDTH-1:0] rs_address;
    logic                                 rt        ;
    logic [Constants::REG_ADDR_WIDTH-1:0] rt_address;
    logic                                 rd        ;
    logic [Constants::REG_ADDR_WIDTH-1:0] rd_address;

    logic                               shamt       ;
    logic [Constants::SHAMT_WIDTH-1:0]  shamt_value ;
    logic                               imm         ;
    logic [Constants::IMM_WIDTH-1:0]    imm_value   ;
    logic                               target      ;
    logic [Constants::TARGET_WIDTH-1:0] target_value;

    logic         alu_mode      ;
    logic [5-1:0] alu_mode_value;

    logic         link       ;
    logic         branch     ;
    logic [3-1:0] branch_mode;
    logic         jump       ;

    logic         lui                      ;
    logic         load                     ;
    logic         load_sign_extend         ;
    logic [2-1:0] load_store_data_size_mode;
    logic         store                    ;

    parser parser_inst (
        .instruction (instruction_fetched),
        .
        rs         (rs        ),
        .rs_address (rs_address),
        .rt         (rt        ),
        .rt_address (rt_address),
        .rd         (rd        ),
        .rd_address (rd_address),
        .
        shamt        (shamt       ),
        .shamt_value  (shamt_value ),
        .imm          (imm         ),
        .imm_value    (imm_value   ),
        .target       (target      ),
        .target_value (target_value),
        .
        alu_mode       (alu_mode      ),
        .alu_mode_value (alu_mode_value),
        .
        link        (link       ),
        .branch      (branch     ),
        .branch_mode (branch_mode),
        .jump        (jump       ),
        .
        lui                       (lui                      ),
        .load                      (load                     ),
        .load_sign_extend          (load_sign_extend         ),
        .load_store_data_size_mode (load_store_data_size_mode),
        .store                     (store                    )
    );

    logic [Constants::WIDTH-1:0] rs_data;
    logic [Constants::WIDTH-1:0] rt_data;

    registers registers_inst (
        .clk (clk),
        .rst (rst),
        .
        rs_address (rs_address),
        .rt_address (rt_address),
        .
        rd         (rd_wb        ),
        .rd_address (rd_address_wb),
        .rd_data    (rd_data_wb   ),
        .
        rs_data (rs_data),
        .rt_data (rt_data),
        .reg_file (reg_file)
    );

    decode_buffer decode_buffer_inst (
        .clk (clk),
        .rst (rst),
        .
        pc_in (pc_fetched),
        .
        rs_in         (rs        ),
        .rs_address_in (rs_address),
        .rt_in         (rt        ),
        .rt_address_in (rt_address),
        .rd_in         (rd        ),
        .rd_address_in (rd_address),
        .
        shamt_in        (shamt       ),
        .shamt_value_in  (shamt_value ),
        .imm_in          (imm         ),
        .imm_value_in    (imm_value   ),
        .target_in       (target      ),
        .target_value_in (target_value),
        .
        alu_mode_in       (alu_mode      ),
        .alu_mode_value_in (alu_mode_value),
        .
        link_in        (link       ),
        .branch_in      (branch     ),
        .branch_mode_in (branch_mode),
        .jump_in        (jump       ),
        .
        lui_in                       (lui                      ),
        .load_in                      (load                     ),
        .load_sign_extend_in          (load_sign_extend         ),
        .load_store_data_size_mode_in (load_store_data_size_mode),
        .store_in                     (store                    ),
        .
        rs_data_in (rs_data),
        .rt_data_in (rt_data),
        .
        pc_out (pc_decoded),
        .
        rs_out         (rs_decoded        ),
        .rs_address_out (rs_address_decoded),
        .rt_out         (rt_decoded        ),
        .rt_address_out (rt_address_decoded),
        .rd_out         (rd_decoded        ),
        .rd_address_out (rd_address_decoded),
        .
        shamt_out        (shamt_decoded       ),
        .shamt_value_out  (shamt_value_decoded ),
        .imm_out          (imm_decoded         ),
        .imm_value_out    (imm_value_decoded   ),
        .target_out       (target_decoded      ),
        .target_value_out (target_value_decoded),
        .
        alu_mode_out       (alu_mode_decoded      ),
        .alu_mode_value_out (alu_mode_value_decoded),
        .
        link_out        (link_decoded       ),
        .branch_out      (branch_decoded     ),
        .branch_mode_out (branch_mode_decoded),
        .jump_out        (jump_decoded       ),
        .
        lui_out                       (lui_decoded                      ),
        .load_out                      (load_decoded                     ),
        .load_sign_extend_out          (load_sign_extend_decoded         ),
        .load_store_data_size_mode_out (load_store_data_size_mode_decoded),
        .store_out                     (store_decoded                    ),
        .
        rs_data_out (rs_data_decoded),
        .rt_data_out (rt_data_decoded)
    );
endmodule