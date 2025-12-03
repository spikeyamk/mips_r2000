package Execute;
    typedef enum logic [2-1:0] {
        ForwarderSource_id = $bits(logic [2-1:0])'(2'b00),
        ForwarderSource_ex = $bits(logic [2-1:0])'(2'b01),
        ForwarderSource_WB = $bits(logic [2-1:0])'(2'b10)
    } ForwarderSource;
endpackage

module imm_extender (
    input  var logic                              imm               ,
    input  var logic [Constants::IMM_WIDTH-1:0]   imm_value         ,
    input  var logic                              shamt             ,
    input  var logic [Constants::SHAMT_WIDTH-1:0] shamt_value       ,
    input  var logic                              alu_mode          ,
    input  var logic [5-1:0]                      alu_mode_value    ,
    input  var logic                              load              ,
    input  var logic                              store             ,
    input  var logic                              branch            ,
    output var logic [Constants::WIDTH-1:0]       imm_value_extended
);
    always_comb begin
        if (imm) begin
            if (
                (load | store | branch)
                || (
                    alu_mode
                    && (
                        (alu_mode_value == Decode::ALUMode_ADD)
                        || (alu_mode_value == Decode::ALUMode_ADDU)
                        || (alu_mode_value == Decode::ALUMode_SUB)
                        || (alu_mode_value == Decode::ALUMode_SUBU)
                        || (alu_mode_value == Decode::ALUMode_SLT)
                    )
                )
            ) begin
                imm_value_extended = {
                    (((imm_value[15] == 1) ==? (1)) ? (
                        16'hffff
                    ) : (
                        16'h0000
                    )),
                    imm_value
                };
            end else begin
                imm_value_extended = {16'h0000, imm_value};
            end
        end else if (shamt) begin
            imm_value_extended = {27'b0, shamt_value};
        end else begin
            imm_value_extended = 0;
        end
    end
endmodule

module register_forwarder (
    input  var logic [Constants::WIDTH-1:0] r_data_id     ,
    input  var logic [Constants::WIDTH-1:0] alu_result_ex,
    input  var logic [Constants::WIDTH-1:0] rd_data_wb         ,
    input  var logic [2-1:0]                selector           ,
    output var logic [Constants::WIDTH-1:0] r_data_forwarded   
);
    always_comb begin
        r_data_forwarded = (((selector) ==? (Execute::ForwarderSource_id)) ? (
            r_data_id
        ) : ((selector) ==? (Execute::ForwarderSource_ex)) ? (
            alu_result_ex
        ) : ((selector) ==? (Execute::ForwarderSource_WB)) ? (
            rd_data_wb
        ) : (
            0
        ));
    end
endmodule

module forwarding_unit (
    input var logic                                 r                  ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] r_address          ,
    input var logic                                 rd_ex        ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_ex,
    input var logic                                 rd_wb              ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_wb      ,

    output var logic [2-1:0] selector
);
    always_comb begin
        selector = Execute::ForwarderSource_id;
        if (r) begin
            if ((rd_ex == 1) && (r_address == rd_address_ex)) begin
                selector = Execute::ForwarderSource_ex;
            end else if ((rd_wb == 1) && (r_address == rd_address_wb)) begin
                selector = Execute::ForwarderSource_WB;
            end
        end
    end
endmodule

module alu_register_imm_mux (
    input  var logic                        imm               ,
    input  var logic                        shamt             ,
    input  var logic                        branch            ,
    input  var logic [Constants::WIDTH-1:0] rt_data_forwarded ,
    input  var logic [Constants::WIDTH-1:0] imm_value_extended,
    output var logic [Constants::WIDTH-1:0] alu_b         
);
    always_comb begin
        alu_b = (branch ==? 1) ? rt_data_forwarded :
            ((imm ==? 1) || (shamt ==? 1)) ? imm_value_extended :
            rt_data_forwarded;
    end
endmodule

module alu (
    input var logic [Constants::WIDTH-1:0] a             ,
    input var logic [Constants::WIDTH-1:0] b             ,
    input var logic [Constants::WIDTH-1:0] pc            ,
    input var logic                        link          ,
    input var logic                        alu_mode      ,
    input var logic [5-1:0]                alu_mode_value,
    input var logic                        branch        ,
    input var logic [3-1:0]                branch_mode   ,
    input var logic                        lui           ,
    input var logic                        load          ,
    input var logic                        store         ,

    output var logic [Constants::WIDTH-1:0] result,
    output var logic                        branch_result
);
    always_comb begin
        result = 0;
        branch_result = 0;
        if (branch) begin
            branch_result = (
                ((branch_mode) ==? (Decode::BranchMode_BLTZ)) ? (
                    ($signed(a) < $signed(b))
                ) : ((branch_mode) ==? (Decode::BranchMode_BGEZ)) ? (
                    ($signed(a) >= $signed(b))
                ) : ((branch_mode) ==? (Decode::BranchMode_BEQ)) ? (
                    (a == b)
                ) : ((branch_mode) ==? (Decode::BranchMode_BNE)) ? (
                    (a != b)
                ) : ((branch_mode) ==? (Decode::BranchMode_BLEZ)) ? (
                    ($signed(a) <= $signed(b))
                ) : ((branch_mode) ==? (Decode::BranchMode_BGTZ)) ? (
                    ($signed(a) > $signed(b))
                ) : (
                    1'b0
                )
            );
        end
        if (load || store) begin
            result = a + b;
        end else if (alu_mode) begin
            result = (((alu_mode_value) ==? (Decode::ALUMode_ADD)) ? (
                (a + b)
            ) : ((alu_mode_value) ==? (Decode::ALUMode_ADDU)) ? (
                ((link ==? 1) ? (pc + 4 + 4) : (a + b))
            ) : ((alu_mode_value) ==? (Decode::ALUMode_SUB)) ? (
                (a - b)
            ) : ((alu_mode_value) ==? (Decode::ALUMode_SUBU)) ? (
                (a - b)
            ) : ((alu_mode_value) ==? (Decode::ALUMode_AND)) ? (
                (a & b)
            ) : ((alu_mode_value) ==? (Decode::ALUMode_OR)) ? (
                (a | b)
            ) : ((alu_mode_value) ==? (Decode::ALUMode_XOR)) ? (
                (a ^ b)
            ) : ((alu_mode_value) ==? (Decode::ALUMode_NOR)) ? (
                ~(a | b)
            ) : ((alu_mode_value) ==? (Decode::ALUMode_SLL)) ? (
                ((lui ==? 0) ? (a << b[4:0]) : (b << 16))
            ) : ((alu_mode_value) ==? (Decode::ALUMode_SRL)) ? (
                (a >> b[4:0])
            ) : ((alu_mode_value) ==? (Decode::ALUMode_SRA)) ? (
                (a >>> b[4:0])
            ) : ((alu_mode_value) ==? (Decode::ALUMode_SLT)) ? (
                {31'b0, ($signed(a) < $signed(b))}
            ) : ((alu_mode_value) ==? (Decode::ALUMode_SLTU)) ? (
                {31'b0, (a < b)}
            ) : (
                0
            ));
        end
    end
endmodule

module brancher (
    input var logic [Constants::WIDTH-1:0] pc                      ,
    input var logic                        branch                  ,
    input var logic                        branch_comparison_result,
    input var logic                        link                    ,
    input var logic                        jump                    ,
    input var logic                        target                  ,
    input var logic [26-1:0]               target_value            ,
    input var logic                        rs                      ,
    input var logic [Constants::WIDTH-1:0] rs_data                 ,
    input var logic                        imm                     ,
    input var logic [Constants::WIDTH-1:0] imm_value               ,
    input var logic                        rd                      ,

    output var logic                        branch_taken ,
    output var logic [Constants::WIDTH-1:0] branch_target,
    output var logic                        rd_branched
);
    always_comb begin
        branch_target = 0;
        branch_taken  = (jump | (branch & branch_comparison_result));
        rd_branched = rd;
        if (branch | jump) begin
            if (target) begin
                branch_target = {4'b00, target_value, 2'b00};
            end else if (imm) begin
                branch_target = (pc + 4) + (imm_value <<< 2);
            end else if (rs) begin
                branch_target = rs_data;
            end
            rd_branched = (jump & link) | (branch_taken & link);
        end
    end
endmodule

module execute_buffer (
    input var logic clk,
    input var logic nrst,

    input var logic [Constants::WIDTH-1:0] pc_in        ,
    input var logic                        alu_mode_in  ,
    input var logic [Constants::WIDTH-1:0] alu_result_in,

    input var logic [Constants::WIDTH-1:0] rt_data_in,

    input var logic                                 rd_in        ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_in,

    input var logic         load_in                     ,
    input var logic [2-1:0] load_store_data_size_mode_in,
    input var logic         load_sign_extend_in         ,
    input var logic         store_in                    ,

    output var logic [Constants::WIDTH-1:0] pc_out        ,
    output var logic                        alu_mode_out  ,
    output var logic [Constants::WIDTH-1:0] alu_result_out,

    output var logic [Constants::WIDTH-1:0] rt_data_out,

    output var logic                                 rd_out        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_out,

    output var logic         load_out                     ,
    output var logic [2-1:0] load_store_data_size_mode_out,
    output var logic         load_sign_extend_out         ,
    output var logic         store_out                    
);
    always_ff @ (posedge clk, negedge nrst) begin
        if (!nrst) begin
            pc_out         <= 0;
            alu_mode_out   <= 0;
            alu_result_out <= 0;

            rt_data_out <= 0;

            rd_out         <= 0;
            rd_address_out <= 0;

            load_out                      <= 0;
            load_store_data_size_mode_out <= 0;
            load_sign_extend_out          <= 0;
            store_out                     <= 0;
        end else begin
            pc_out         <= pc_in;
            alu_mode_out   <= alu_mode_in;
            alu_result_out <= alu_result_in;

            rt_data_out <= rt_data_in;

            rd_out         <= rd_in;
            rd_address_out <= rd_address_in;

            load_out                      <= load_in;
            load_store_data_size_mode_out <= load_store_data_size_mode_in;
            load_sign_extend_out          <= load_sign_extend_in;
            store_out                     <= store_in;
        end
    end
endmodule

module execute (
    input  var logic                        clk                ,
    input  var logic                        nrst                ,
    input  var logic [Constants::BYTE-1:0]  rom     [0:Constants::ROM_SIZE-1] ,
    input  var logic                        stall              ,

    input var logic                                 rd_wb        ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_wb,
    input var logic [Constants::WIDTH-1:0]          rd_data_wb   ,

    output var logic [Constants::WIDTH-1:0] pc_ex           ,

    output var logic                                 rd_ex        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_ex,

    output var logic                        alu_mode_ex  ,
    output var logic [Constants::WIDTH-1:0] alu_result_ex,

    output var logic [Constants::WIDTH-1:0] rt_data_ex,

    output var logic         load_ex                     ,
    output var logic [2-1:0] load_store_data_size_mode_ex,
    output var logic         load_sign_extend_ex         ,
    output var logic         store_ex,
    output var logic [Constants::WIDTH-1:0] reg_file [0:Constants::REG_COUNT - 1-1]
);
    var logic [Constants::WIDTH-1:0] pc_id;

    var logic                                 rs_id;
    var logic [Constants::REG_ADDR_WIDTH-1:0] rs_address_id;
    var logic [Constants::WIDTH-1:0]          rs_data_id;
    var logic                                 rt_id;
    var logic [Constants::REG_ADDR_WIDTH-1:0] rt_address_id;
    var logic [Constants::WIDTH-1:0]          rt_data_id;
    var logic                                 rd_id;
    var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_id;

    var logic                               shamt_id;
    var logic [Constants::SHAMT_WIDTH-1:0]  shamt_value_id ;
    var logic                               imm_id;
    var logic [Constants::IMM_WIDTH-1:0]    imm_value_id;
    var logic                               target_id;
    var logic [Constants::TARGET_WIDTH-1:0] target_value_id;

    var logic         alu_mode_id;
    var logic [5-1:0] alu_mode_value_id;

    var logic         link_id;
    var logic         branch_id;
    var logic [3-1:0] branch_mode_id;
    var logic         jump_id;

    var logic         lui_id;
    var logic         load_id;
    var logic         load_sign_extend_id;
    var logic [2-1:0] load_store_data_size_mode_id;
    var logic         store_id;

    var logic                        branch_taken_branched;
    var logic [Constants::WIDTH-1:0] branch_target_branched;

    decode decode_inst (
        .clk(clk),
        .nrst(nrst),
        .rom(rom),
        .stall(stall),
        .branch_taken_ex(branch_taken_branched),
        .branch_target_ex(branch_target_branched),

        .rd_wb(rd_wb),
        .rd_address_wb(rd_address_wb),
        .rd_data_wb(rd_data_wb),

        .pc_id(pc_id),

        .rs_id(rs_id),
        .rs_address_id(rs_address_id),
        .rs_data_id(rs_data_id),
        .rt_id(rt_id),
        .rt_address_id(rt_address_id),
        .rt_data_id(rt_data_id),
        .rd_id(rd_id),
        .rd_address_id(rd_address_id),

        .shamt_id(shamt_id),
        .shamt_value_id(shamt_value_id),
        .imm_id(imm_id),
        .imm_value_id(imm_value_id),
        .target_id(target_id),
        .target_value_id(target_value_id),

        .alu_mode_id(alu_mode_id),
        .alu_mode_value_id(alu_mode_value_id),

        .link_id(link_id),
        .branch_id(branch_id),
        .branch_mode_id(branch_mode_id),
        .jump_id(jump_id),

        .lui_id(lui_id),
        .load_id(load_id),
        .load_sign_extend_id(load_sign_extend_id),
        .load_store_data_size_mode_id(load_store_data_size_mode_id),
        .store_id(store_id),
        .reg_file(reg_file)
    );

    logic [Constants::WIDTH-1:0] imm_value_extended;
    imm_extender imm_extender_inst (
        .imm                (imm_id       ),
        .imm_value          (imm_value_id ),
        .shamt              (shamt_id     ),
        .shamt_value        (shamt_value_id),
        .alu_mode           (alu_mode_id  ),
        .alu_mode_value     (alu_mode_value_id),
        .load               (load_id),
        .store              (store_id),
        .branch             (branch_id),
        .imm_value_extended (imm_value_extended)
    );

    logic [2-1:0] forwarder_a_selector;
    forwarding_unit forwarding_unit_a (
        .r                   (rs_id          ),
        .r_address           (rs_address_id  ),
        .rd_ex         (rd_ex         ),
        .rd_address_ex (rd_address_ex ),
        .rd_wb               (rd_wb               ),
        .rd_address_wb       (rd_address_wb       ),
        .selector            (forwarder_a_selector)
    );
    logic [Constants::WIDTH-1:0] rs_data_forwarded;
    register_forwarder register_forwarder_a (
        .r_data_id      (rs_data_id     ),
        .alu_result_ex (alu_result_ex ),
        .rd_data_wb          (rd_data_wb          ),
        .selector            (forwarder_a_selector),
        .r_data_forwarded    (rs_data_forwarded   )
    );

    logic [2-1:0] forwarder_b_selector;
    forwarding_unit forwarding_unit_b (
        .r                   (rt_id          ),
        .r_address           (rt_address_id  ),
        .rd_ex               (rd_ex          ),
        .rd_address_ex       (rd_address_ex  ),
        .rd_wb               (rd_wb               ),
        .rd_address_wb       (rd_address_wb       ),
        .selector            (forwarder_b_selector)
    );
    logic [Constants::WIDTH-1:0] rt_data_forwarded;
    register_forwarder register_forwarder_b (
        .r_data_id      (rt_data_id     ),
        .alu_result_ex  (alu_result_ex  ),
        .rd_data_wb          (rd_data_wb          ),
        .selector            (forwarder_b_selector),
        .r_data_forwarded    (rt_data_forwarded   )
    );
    logic [Constants::WIDTH-1:0] alu_b;
    alu_register_imm_mux alu_register_imm_mux_inst (
        .imm                (imm_id       ),
        .shamt              (shamt_id     ),
        .branch             (branch_id    ),
        .rt_data_forwarded  (rt_data_forwarded ),
        .imm_value_extended (imm_value_extended),
        .alu_b              (alu_b             )
    );

    logic [Constants::WIDTH-1:0] alu_result;
    logic                        alu_branch_result;
    alu alu_inst (
        .a              (rs_data_forwarded),
        .b              (alu_b            ),
        .pc             (pc_id       ),
        .link           (link_id     ),
        .alu_mode       (alu_mode_id ),
        .alu_mode_value (alu_mode_value_id),
        .branch         (branch_id   ),
        .branch_mode    (branch_mode_id),
        .lui            (lui_id      ),
        .load           (load_id     ),
        .store          (store_id    ),
        .
        result (alu_result),
        .branch_result (alu_branch_result)
    );

    logic rd_branched;
    brancher brancher_inst (
        .pc                       (pc_id        ),
        .branch                   (branch_id    ),
        .branch_comparison_result (alu_branch_result ),
        .link                     (link_id      ),
        .jump                     (jump_id      ),
        .target                   (target_id    ),
        .target_value             (target_value_id),
        .rs                       (rs_id        ),
        .rs_data                  (rs_data_forwarded ),
        .imm                      (imm_id       ),
        .imm_value                (imm_value_extended),
        .rd                       (rd_id        ),
        .
        branch_taken   (branch_taken_branched ),
        .branch_target (branch_target_branched),
        .rd_branched   (rd_branched           )
    );

    execute_buffer execute_buffer_inst (
        .clk  (clk ),
        .nrst (nrst),
        .
        pc_in          (pc_id      ),
        .alu_mode_in   (alu_mode_id),
        .alu_result_in (alu_result ),
        .
        rt_data_in (rt_data_forwarded),
        .
        rd_in          (rd_branched),
        .rd_address_in (rd_address_id),
        .
        load_in                       (load_id),
        .load_store_data_size_mode_in (load_store_data_size_mode_id),
        .load_sign_extend_in          (load_sign_extend_id),
        .store_in                     (store_id),

        .
        pc_out          (pc_ex        ),
        .alu_mode_out   (alu_mode_ex  ),
        .alu_result_out (alu_result_ex),
        .
        rt_data_out (rt_data_ex),
        .
        rd_out          (rd_ex        ),
        .rd_address_out (rd_address_ex),
        .
        load_out                       (load_ex                     ),
        .load_store_data_size_mode_out (load_store_data_size_mode_ex),
        .load_sign_extend_out          (load_sign_extend_ex         ),
        .store_out                     (store_ex                    )
    );
endmodule