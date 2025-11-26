module divider #(
    parameter int unsigned STOPPER = 25_000_000
) (
    input logic clk,
    input logic rst,
    output logic out
);
	logic[31:0] count = 0;
	
	always @(posedge clk, negedge rst) begin
        if (!rst) begin
            count <= 32'b0;
            out <= 1'b0;
        end else if (count < STOPPER) begin
            count <= count + 1'b1;
        end else begin
            count <= 32'b0;
            out <= ~out;
        end
    end
endmodule

module counter #(
    parameter int unsigned WIDTH = 3
) (
    input logic clk,
    input logic rst,
    output logic [WIDTH-1:0] out
);
    always_ff @(posedge clk, negedge rst) begin
        if (!rst) begin
            out <= {WIDTH{1'b0}};
        end else begin
            out <= out + 1'b1;
        end
    end
endmodule

module sevseg_decoder (
	input logic [3:0] digit_number,
	output logic [6:0] seg
);
    always_comb begin
        case (digit_number)
            4'h0: seg = 7'b1000000;
            4'h1: seg = 7'b1111001;
            4'h2: seg = 7'b0100100;
            4'h3: seg = 7'b0110000;
            4'h4: seg = 7'b0011001;
            4'h5: seg = 7'b0010010;
            4'h6: seg = 7'b0000010;
            4'h7: seg = 7'b1111000;
            4'h8: seg = 7'b0000000;
            4'h9: seg = 7'b0011000;
            4'hA: seg = 7'b0001000;
            4'hB: seg = 7'b0000011;
            4'hC: seg = 7'b1000110;
            4'hD: seg = 7'b0100001;
            4'hE: seg = 7'b0000110;
            4'hF: seg = 7'b0001110;
            default: seg = 7'b1001001;
        endcase
    end
endmodule

module sevseg (
    input logic clk,
    input logic rst,
    input logic [31:0] number,
    output logic [7:0] an,
    output logic [6:0] seg,
    output logic dp
);
    logic clk_divided_10_000;
    divider #(
        .STOPPER(10_000)
    ) divider_10_000 (
        .clk(clk),
        .rst(rst),
        .out(clk_divided_10_000)
    );

    logic [3-1:0] digit_selector;
    counter #(
        .WIDTH(3)
    ) counter_digit_selector (
        .clk(clk_divided_10_000),
        .rst(rst),
        .out(digit_selector)
    );
    
    always_ff @(posedge clk_divided_10_000, negedge rst) begin
        if (!rst || (an == 8'b1111_1111)) begin
            an <= 8'b1111_1110;
        end else begin
            an <= (an == 8'b0111_1111) ? 8'b1111_1110 : ((an << 1'b1) | 8'b0000_0001);
        end
    end

    logic [3:0] digit_number;
    sevseg_decoder sevseg_decoder_inst(
        .digit_number(digit_number),
        .seg(seg)
    );

    logic [3:0] number_nibbles [0:7];
    always_comb begin
        dp = 1'b1;
        number_nibbles[7] = number[31:28];
        number_nibbles[6] = number[27:24];
        number_nibbles[5] = number[23:20];
        number_nibbles[4] = number[19:16];
        number_nibbles[3] = number[15:12];
        number_nibbles[2] = number[11:8];
        number_nibbles[1] = number[7:4];
        number_nibbles[0] = number[3:0];
        digit_number = number_nibbles[digit_selector];
    end
endmodule

module debounce_sync (
    input logic clk,
    input logic in,
    output logic out
);
    logic sync_0;
    logic sync_1;

    always_ff @(posedge clk, negedge in) begin
        if (!in) begin
            sync_0 <= 0;
            sync_1 <= 0;
        end else begin
            sync_0 <= 1;
            sync_1 <= sync_0;
        end
    end
    
    localparam int unsigned CYCLES = 10_000_000;
    logic [$clog2(CYCLES)-1:0] count = 0;
    logic stable_state = 1;

    always_ff @(posedge clk) begin
        if (sync_1 != stable_state) begin
            count <= count + 1;

            if (count == CYCLES-1) begin
                stable_state <= sync_1;
                count <= 0;
            end
        end else begin
            count <= 0;
        end
    end
    
    always_comb begin
        out = stable_state;
    end
endmodule

module stall_stepper (
    input logic clk,
    input logic rst,
    input logic stall,
    input logic step,
    output logic out
);
    logic step_posedge_flag;
    logic step_posedge_flag_nrst;
    always_ff @(posedge step, negedge step_posedge_flag_nrst) begin
        if (!step_posedge_flag_nrst) begin
            step_posedge_flag <= 0;
        end else begin
            step_posedge_flag <= 1;
        end
    end

    logic stall_stepped;
    always_ff @(negedge clk, negedge rst) begin
        if (!rst) begin
            stall_stepped <= step;
        end else if (step_posedge_flag & stall) begin
            stall_stepped <= 0;
            step_posedge_flag_nrst <= 0;
        end else begin
            step_posedge_flag_nrst <= 1;
            stall_stepped <= stall;
        end
    end

    always_comb begin
        if (stall) begin
            out = stall_stepped;
        end else begin
            out = stall;
        end
    end
endmodule

module bubble_sort_demo (
    input logic clk,
    input logic rst,
    input logic stall,
    input logic turbo,
    input logic step,
    input logic show_pc,
    input logic show_stack_array,
    input logic show_ra,
    input logic show_s8,
    input logic show_sp,
    output logic [7:0] an,
    output logic [6:0] seg,
    output logic dp
);
    logic rst_debounced_synced;
    debounce_sync debounce_sync_rst (
        .clk(clk),
        .in(rst),
        .out(rst_debounced_synced)
    );
    
    logic stall_debounced_synced;
    debounce_sync debounce_sync_stall (
        .clk(clk),
        .in(stall),
        .out(stall_debounced_synced)
    );
    
    logic step_debounced_synced;
    debounce_sync debounce_sync_step (
        .clk(clk),
        .in(step),
        .out(step_debounced_synced)
    );
    
    logic [Constants::BYTE-1:0] rom [0:Constants::ROM_SIZE-1];
    initial begin
        rom[0] = 8'h3c;
        rom[1] = 8'h1d;
        rom[2] = 8'h80;
        rom[3] = 8'h00;
        rom[4] = 8'h27;
        rom[5] = 8'hbd;
        rom[6] = 8'h00;
        rom[7] = 8'h80;
        rom[8] = 8'h3c;
        rom[9] = 8'h08;
        rom[10] = 8'h80;
        rom[11] = 8'h00;
        rom[12] = 8'h25;
        rom[13] = 8'h08;
        rom[14] = 8'h00;
        rom[15] = 8'h00;
        rom[16] = 8'h3c;
        rom[17] = 8'h09;
        rom[18] = 8'h80;
        rom[19] = 8'h00;
        rom[20] = 8'h25;
        rom[21] = 8'h29;
        rom[22] = 8'h00;
        rom[23] = 8'h00;
        rom[24] = 8'h01;
        rom[25] = 8'h09;
        rom[26] = 8'h08;
        rom[27] = 8'h2a;
        rom[28] = 8'h10;
        rom[29] = 8'h20;
        rom[30] = 8'h00;
        rom[31] = 8'h05;
        rom[32] = 8'h00;
        rom[33] = 8'h00;
        rom[34] = 8'h00;
        rom[35] = 8'h00;
        rom[36] = 8'had;
        rom[37] = 8'h00;
        rom[38] = 8'h00;
        rom[39] = 8'h00;
        rom[40] = 8'h25;
        rom[41] = 8'h08;
        rom[42] = 8'h00;
        rom[43] = 8'h04;
        rom[44] = 8'h08;
        rom[45] = 8'h00;
        rom[46] = 8'h00;
        rom[47] = 8'h06;
        rom[48] = 8'h00;
        rom[49] = 8'h00;
        rom[50] = 8'h00;
        rom[51] = 8'h00;
        rom[52] = 8'h3c;
        rom[53] = 8'h08;
        rom[54] = 8'h80;
        rom[55] = 8'h00;
        rom[56] = 8'h25;
        rom[57] = 8'h08;
        rom[58] = 8'h04;
        rom[59] = 8'h00;
        rom[60] = 8'h3c;
        rom[61] = 8'h09;
        rom[62] = 8'h80;
        rom[63] = 8'h00;
        rom[64] = 8'h25;
        rom[65] = 8'h29;
        rom[66] = 8'h00;
        rom[67] = 8'h00;
        rom[68] = 8'h3c;
        rom[69] = 8'h0a;
        rom[70] = 8'h80;
        rom[71] = 8'h00;
        rom[72] = 8'h25;
        rom[73] = 8'h4a;
        rom[74] = 8'h00;
        rom[75] = 8'h00;
        rom[76] = 8'h01;
        rom[77] = 8'h2a;
        rom[78] = 8'h08;
        rom[79] = 8'h2a;
        rom[80] = 8'h10;
        rom[81] = 8'h20;
        rom[82] = 8'h00;
        rom[83] = 8'h0a;
        rom[84] = 8'h00;
        rom[85] = 8'h00;
        rom[86] = 8'h00;
        rom[87] = 8'h00;
        rom[88] = 8'h00;
        rom[89] = 8'h00;
        rom[90] = 8'h00;
        rom[91] = 8'h00;
        rom[92] = 8'h8d;
        rom[93] = 8'h0b;
        rom[94] = 8'h00;
        rom[95] = 8'h00;
        rom[96] = 8'h00;
        rom[97] = 8'h00;
        rom[98] = 8'h00;
        rom[99] = 8'h00;
        rom[100] = 8'had;
        rom[101] = 8'h2b;
        rom[102] = 8'h00;
        rom[103] = 8'h00;
        rom[104] = 8'h25;
        rom[105] = 8'h08;
        rom[106] = 8'h00;
        rom[107] = 8'h04;
        rom[108] = 8'h25;
        rom[109] = 8'h29;
        rom[110] = 8'h00;
        rom[111] = 8'h04;
        rom[112] = 8'h08;
        rom[113] = 8'h00;
        rom[114] = 8'h00;
        rom[115] = 8'h13;
        rom[116] = 8'h00;
        rom[117] = 8'h00;
        rom[118] = 8'h00;
        rom[119] = 8'h00;
        rom[120] = 8'h00;
        rom[121] = 8'h00;
        rom[122] = 8'h00;
        rom[123] = 8'h00;
        rom[124] = 8'h0c;
        rom[125] = 8'h00;
        rom[126] = 8'h00;
        rom[127] = 8'hb2;
        rom[128] = 8'h00;
        rom[129] = 8'h00;
        rom[130] = 8'h00;
        rom[131] = 8'h00;
        rom[132] = 8'h00;
        rom[133] = 8'h00;
        rom[134] = 8'h00;
        rom[135] = 8'h00;
        rom[136] = 8'h10;
        rom[137] = 8'h00;
        rom[138] = 8'hff;
        rom[139] = 8'hff;
        rom[140] = 8'h00;
        rom[141] = 8'h00;
        rom[142] = 8'h00;
        rom[143] = 8'h00;
        rom[144] = 8'h00;
        rom[145] = 8'h00;
        rom[146] = 8'h00;
        rom[147] = 8'h00;
        rom[148] = 8'h27;
        rom[149] = 8'hbd;
        rom[150] = 8'hff;
        rom[151] = 8'hf0;
        rom[152] = 8'haf;
        rom[153] = 8'hbe;
        rom[154] = 8'h00;
        rom[155] = 8'h0c;
        rom[156] = 8'h03;
        rom[157] = 8'ha0;
        rom[158] = 8'hf0;
        rom[159] = 8'h25;
        rom[160] = 8'haf;
        rom[161] = 8'hc4;
        rom[162] = 8'h00;
        rom[163] = 8'h10;
        rom[164] = 8'haf;
        rom[165] = 8'hc5;
        rom[166] = 8'h00;
        rom[167] = 8'h14;
        rom[168] = 8'haf;
        rom[169] = 8'hc0;
        rom[170] = 8'h00;
        rom[171] = 8'h00;
        rom[172] = 8'h10;
        rom[173] = 8'h00;
        rom[174] = 8'h00;
        rom[175] = 8'h1b;
        rom[176] = 8'h00;
        rom[177] = 8'h00;
        rom[178] = 8'h00;
        rom[179] = 8'h00;
        rom[180] = 8'h8f;
        rom[181] = 8'hc2;
        rom[182] = 8'h00;
        rom[183] = 8'h00;
        rom[184] = 8'h00;
        rom[185] = 8'h00;
        rom[186] = 8'h00;
        rom[187] = 8'h00;
        rom[188] = 8'h00;
        rom[189] = 8'h02;
        rom[190] = 8'h10;
        rom[191] = 8'h80;
        rom[192] = 8'h8f;
        rom[193] = 8'hc3;
        rom[194] = 8'h00;
        rom[195] = 8'h10;
        rom[196] = 8'h00;
        rom[197] = 8'h00;
        rom[198] = 8'h00;
        rom[199] = 8'h00;
        rom[200] = 8'h00;
        rom[201] = 8'h62;
        rom[202] = 8'h10;
        rom[203] = 8'h21;
        rom[204] = 8'h8c;
        rom[205] = 8'h43;
        rom[206] = 8'h00;
        rom[207] = 8'h00;
        rom[208] = 8'h8f;
        rom[209] = 8'hc2;
        rom[210] = 8'h00;
        rom[211] = 8'h00;
        rom[212] = 8'h00;
        rom[213] = 8'h00;
        rom[214] = 8'h00;
        rom[215] = 8'h00;
        rom[216] = 8'h24;
        rom[217] = 8'h42;
        rom[218] = 8'h00;
        rom[219] = 8'h01;
        rom[220] = 8'h00;
        rom[221] = 8'h02;
        rom[222] = 8'h10;
        rom[223] = 8'h80;
        rom[224] = 8'h8f;
        rom[225] = 8'hc4;
        rom[226] = 8'h00;
        rom[227] = 8'h10;
        rom[228] = 8'h00;
        rom[229] = 8'h00;
        rom[230] = 8'h00;
        rom[231] = 8'h00;
        rom[232] = 8'h00;
        rom[233] = 8'h82;
        rom[234] = 8'h10;
        rom[235] = 8'h21;
        rom[236] = 8'h8c;
        rom[237] = 8'h42;
        rom[238] = 8'h00;
        rom[239] = 8'h00;
        rom[240] = 8'h00;
        rom[241] = 8'h00;
        rom[242] = 8'h00;
        rom[243] = 8'h00;
        rom[244] = 8'h00;
        rom[245] = 8'h43;
        rom[246] = 8'h10;
        rom[247] = 8'h2b;
        rom[248] = 8'h10;
        rom[249] = 8'h40;
        rom[250] = 8'h00;
        rom[251] = 8'h04;
        rom[252] = 8'h00;
        rom[253] = 8'h00;
        rom[254] = 8'h00;
        rom[255] = 8'h00;
        rom[256] = 8'h00;
        rom[257] = 8'h00;
        rom[258] = 8'h10;
        rom[259] = 8'h25;
        rom[260] = 8'h10;
        rom[261] = 8'h00;
        rom[262] = 8'h00;
        rom[263] = 8'h0e;
        rom[264] = 8'h00;
        rom[265] = 8'h00;
        rom[266] = 8'h00;
        rom[267] = 8'h00;
        rom[268] = 8'h8f;
        rom[269] = 8'hc2;
        rom[270] = 8'h00;
        rom[271] = 8'h00;
        rom[272] = 8'h00;
        rom[273] = 8'h00;
        rom[274] = 8'h00;
        rom[275] = 8'h00;
        rom[276] = 8'h24;
        rom[277] = 8'h42;
        rom[278] = 8'h00;
        rom[279] = 8'h01;
        rom[280] = 8'haf;
        rom[281] = 8'hc2;
        rom[282] = 8'h00;
        rom[283] = 8'h00;
        rom[284] = 8'h8f;
        rom[285] = 8'hc2;
        rom[286] = 8'h00;
        rom[287] = 8'h14;
        rom[288] = 8'h00;
        rom[289] = 8'h00;
        rom[290] = 8'h00;
        rom[291] = 8'h00;
        rom[292] = 8'h24;
        rom[293] = 8'h42;
        rom[294] = 8'hff;
        rom[295] = 8'hff;
        rom[296] = 8'h8f;
        rom[297] = 8'hc3;
        rom[298] = 8'h00;
        rom[299] = 8'h00;
        rom[300] = 8'h00;
        rom[301] = 8'h00;
        rom[302] = 8'h00;
        rom[303] = 8'h00;
        rom[304] = 8'h00;
        rom[305] = 8'h62;
        rom[306] = 8'h10;
        rom[307] = 8'h2b;
        rom[308] = 8'h14;
        rom[309] = 8'h40;
        rom[310] = 8'hff;
        rom[311] = 8'hdf;
        rom[312] = 8'h00;
        rom[313] = 8'h00;
        rom[314] = 8'h00;
        rom[315] = 8'h00;
        rom[316] = 8'h24;
        rom[317] = 8'h02;
        rom[318] = 8'h00;
        rom[319] = 8'h01;
        rom[320] = 8'h03;
        rom[321] = 8'hc0;
        rom[322] = 8'he8;
        rom[323] = 8'h25;
        rom[324] = 8'h8f;
        rom[325] = 8'hbe;
        rom[326] = 8'h00;
        rom[327] = 8'h0c;
        rom[328] = 8'h27;
        rom[329] = 8'hbd;
        rom[330] = 8'h00;
        rom[331] = 8'h10;
        rom[332] = 8'h03;
        rom[333] = 8'he0;
        rom[334] = 8'h00;
        rom[335] = 8'h08;
        rom[336] = 8'h00;
        rom[337] = 8'h00;
        rom[338] = 8'h00;
        rom[339] = 8'h00;
        rom[340] = 8'h27;
        rom[341] = 8'hbd;
        rom[342] = 8'hff;
        rom[343] = 8'he0;
        rom[344] = 8'haf;
        rom[345] = 8'hbf;
        rom[346] = 8'h00;
        rom[347] = 8'h1c;
        rom[348] = 8'haf;
        rom[349] = 8'hbe;
        rom[350] = 8'h00;
        rom[351] = 8'h18;
        rom[352] = 8'h03;
        rom[353] = 8'ha0;
        rom[354] = 8'hf0;
        rom[355] = 8'h25;
        rom[356] = 8'haf;
        rom[357] = 8'hc4;
        rom[358] = 8'h00;
        rom[359] = 8'h20;
        rom[360] = 8'haf;
        rom[361] = 8'hc5;
        rom[362] = 8'h00;
        rom[363] = 8'h24;
        rom[364] = 8'h10;
        rom[365] = 8'h00;
        rom[366] = 8'h00;
        rom[367] = 8'h46;
        rom[368] = 8'h00;
        rom[369] = 8'h00;
        rom[370] = 8'h00;
        rom[371] = 8'h00;
        rom[372] = 8'haf;
        rom[373] = 8'hc0;
        rom[374] = 8'h00;
        rom[375] = 8'h10;
        rom[376] = 8'h10;
        rom[377] = 8'h00;
        rom[378] = 8'h00;
        rom[379] = 8'h3b;
        rom[380] = 8'h00;
        rom[381] = 8'h00;
        rom[382] = 8'h00;
        rom[383] = 8'h00;
        rom[384] = 8'h8f;
        rom[385] = 8'hc2;
        rom[386] = 8'h00;
        rom[387] = 8'h10;
        rom[388] = 8'h00;
        rom[389] = 8'h00;
        rom[390] = 8'h00;
        rom[391] = 8'h00;
        rom[392] = 8'h00;
        rom[393] = 8'h02;
        rom[394] = 8'h10;
        rom[395] = 8'h80;
        rom[396] = 8'h8f;
        rom[397] = 8'hc3;
        rom[398] = 8'h00;
        rom[399] = 8'h20;
        rom[400] = 8'h00;
        rom[401] = 8'h00;
        rom[402] = 8'h00;
        rom[403] = 8'h00;
        rom[404] = 8'h00;
        rom[405] = 8'h62;
        rom[406] = 8'h10;
        rom[407] = 8'h21;
        rom[408] = 8'h8c;
        rom[409] = 8'h43;
        rom[410] = 8'h00;
        rom[411] = 8'h00;
        rom[412] = 8'h8f;
        rom[413] = 8'hc2;
        rom[414] = 8'h00;
        rom[415] = 8'h10;
        rom[416] = 8'h00;
        rom[417] = 8'h00;
        rom[418] = 8'h00;
        rom[419] = 8'h00;
        rom[420] = 8'h24;
        rom[421] = 8'h42;
        rom[422] = 8'h00;
        rom[423] = 8'h01;
        rom[424] = 8'h00;
        rom[425] = 8'h02;
        rom[426] = 8'h10;
        rom[427] = 8'h80;
        rom[428] = 8'h8f;
        rom[429] = 8'hc4;
        rom[430] = 8'h00;
        rom[431] = 8'h20;
        rom[432] = 8'h00;
        rom[433] = 8'h00;
        rom[434] = 8'h00;
        rom[435] = 8'h00;
        rom[436] = 8'h00;
        rom[437] = 8'h82;
        rom[438] = 8'h10;
        rom[439] = 8'h21;
        rom[440] = 8'h8c;
        rom[441] = 8'h42;
        rom[442] = 8'h00;
        rom[443] = 8'h00;
        rom[444] = 8'h00;
        rom[445] = 8'h00;
        rom[446] = 8'h00;
        rom[447] = 8'h00;
        rom[448] = 8'h00;
        rom[449] = 8'h43;
        rom[450] = 8'h10;
        rom[451] = 8'h2b;
        rom[452] = 8'h10;
        rom[453] = 8'h40;
        rom[454] = 8'h00;
        rom[455] = 8'h24;
        rom[456] = 8'h00;
        rom[457] = 8'h00;
        rom[458] = 8'h00;
        rom[459] = 8'h00;
        rom[460] = 8'h8f;
        rom[461] = 8'hc2;
        rom[462] = 8'h00;
        rom[463] = 8'h10;
        rom[464] = 8'h00;
        rom[465] = 8'h00;
        rom[466] = 8'h00;
        rom[467] = 8'h00;
        rom[468] = 8'h00;
        rom[469] = 8'h02;
        rom[470] = 8'h10;
        rom[471] = 8'h80;
        rom[472] = 8'h8f;
        rom[473] = 8'hc3;
        rom[474] = 8'h00;
        rom[475] = 8'h20;
        rom[476] = 8'h00;
        rom[477] = 8'h00;
        rom[478] = 8'h00;
        rom[479] = 8'h00;
        rom[480] = 8'h00;
        rom[481] = 8'h62;
        rom[482] = 8'h10;
        rom[483] = 8'h21;
        rom[484] = 8'h8c;
        rom[485] = 8'h42;
        rom[486] = 8'h00;
        rom[487] = 8'h00;
        rom[488] = 8'h00;
        rom[489] = 8'h00;
        rom[490] = 8'h00;
        rom[491] = 8'h00;
        rom[492] = 8'haf;
        rom[493] = 8'hc2;
        rom[494] = 8'h00;
        rom[495] = 8'h14;
        rom[496] = 8'h8f;
        rom[497] = 8'hc2;
        rom[498] = 8'h00;
        rom[499] = 8'h10;
        rom[500] = 8'h00;
        rom[501] = 8'h00;
        rom[502] = 8'h00;
        rom[503] = 8'h00;
        rom[504] = 8'h24;
        rom[505] = 8'h42;
        rom[506] = 8'h00;
        rom[507] = 8'h01;
        rom[508] = 8'h00;
        rom[509] = 8'h02;
        rom[510] = 8'h10;
        rom[511] = 8'h80;
        rom[512] = 8'h8f;
        rom[513] = 8'hc3;
        rom[514] = 8'h00;
        rom[515] = 8'h20;
        rom[516] = 8'h00;
        rom[517] = 8'h00;
        rom[518] = 8'h00;
        rom[519] = 8'h00;
        rom[520] = 8'h00;
        rom[521] = 8'h62;
        rom[522] = 8'h18;
        rom[523] = 8'h21;
        rom[524] = 8'h8f;
        rom[525] = 8'hc2;
        rom[526] = 8'h00;
        rom[527] = 8'h10;
        rom[528] = 8'h00;
        rom[529] = 8'h00;
        rom[530] = 8'h00;
        rom[531] = 8'h00;
        rom[532] = 8'h00;
        rom[533] = 8'h02;
        rom[534] = 8'h10;
        rom[535] = 8'h80;
        rom[536] = 8'h8f;
        rom[537] = 8'hc4;
        rom[538] = 8'h00;
        rom[539] = 8'h20;
        rom[540] = 8'h00;
        rom[541] = 8'h00;
        rom[542] = 8'h00;
        rom[543] = 8'h00;
        rom[544] = 8'h00;
        rom[545] = 8'h82;
        rom[546] = 8'h10;
        rom[547] = 8'h21;
        rom[548] = 8'h8c;
        rom[549] = 8'h63;
        rom[550] = 8'h00;
        rom[551] = 8'h00;
        rom[552] = 8'h00;
        rom[553] = 8'h00;
        rom[554] = 8'h00;
        rom[555] = 8'h00;
        rom[556] = 8'hac;
        rom[557] = 8'h43;
        rom[558] = 8'h00;
        rom[559] = 8'h00;
        rom[560] = 8'h8f;
        rom[561] = 8'hc2;
        rom[562] = 8'h00;
        rom[563] = 8'h10;
        rom[564] = 8'h00;
        rom[565] = 8'h00;
        rom[566] = 8'h00;
        rom[567] = 8'h00;
        rom[568] = 8'h24;
        rom[569] = 8'h42;
        rom[570] = 8'h00;
        rom[571] = 8'h01;
        rom[572] = 8'h00;
        rom[573] = 8'h02;
        rom[574] = 8'h10;
        rom[575] = 8'h80;
        rom[576] = 8'h8f;
        rom[577] = 8'hc3;
        rom[578] = 8'h00;
        rom[579] = 8'h20;
        rom[580] = 8'h00;
        rom[581] = 8'h00;
        rom[582] = 8'h00;
        rom[583] = 8'h00;
        rom[584] = 8'h00;
        rom[585] = 8'h62;
        rom[586] = 8'h10;
        rom[587] = 8'h21;
        rom[588] = 8'h8f;
        rom[589] = 8'hc3;
        rom[590] = 8'h00;
        rom[591] = 8'h14;
        rom[592] = 8'h00;
        rom[593] = 8'h00;
        rom[594] = 8'h00;
        rom[595] = 8'h00;
        rom[596] = 8'hac;
        rom[597] = 8'h43;
        rom[598] = 8'h00;
        rom[599] = 8'h00;
        rom[600] = 8'h8f;
        rom[601] = 8'hc2;
        rom[602] = 8'h00;
        rom[603] = 8'h10;
        rom[604] = 8'h00;
        rom[605] = 8'h00;
        rom[606] = 8'h00;
        rom[607] = 8'h00;
        rom[608] = 8'h24;
        rom[609] = 8'h42;
        rom[610] = 8'h00;
        rom[611] = 8'h01;
        rom[612] = 8'haf;
        rom[613] = 8'hc2;
        rom[614] = 8'h00;
        rom[615] = 8'h10;
        rom[616] = 8'h8f;
        rom[617] = 8'hc2;
        rom[618] = 8'h00;
        rom[619] = 8'h24;
        rom[620] = 8'h00;
        rom[621] = 8'h00;
        rom[622] = 8'h00;
        rom[623] = 8'h00;
        rom[624] = 8'h24;
        rom[625] = 8'h42;
        rom[626] = 8'hff;
        rom[627] = 8'hff;
        rom[628] = 8'h8f;
        rom[629] = 8'hc3;
        rom[630] = 8'h00;
        rom[631] = 8'h10;
        rom[632] = 8'h00;
        rom[633] = 8'h00;
        rom[634] = 8'h00;
        rom[635] = 8'h00;
        rom[636] = 8'h00;
        rom[637] = 8'h62;
        rom[638] = 8'h10;
        rom[639] = 8'h2b;
        rom[640] = 8'h14;
        rom[641] = 8'h40;
        rom[642] = 8'hff;
        rom[643] = 8'hbf;
        rom[644] = 8'h00;
        rom[645] = 8'h00;
        rom[646] = 8'h00;
        rom[647] = 8'h00;
        rom[648] = 8'h8f;
        rom[649] = 8'hc5;
        rom[650] = 8'h00;
        rom[651] = 8'h24;
        rom[652] = 8'h8f;
        rom[653] = 8'hc4;
        rom[654] = 8'h00;
        rom[655] = 8'h20;
        rom[656] = 8'h0c;
        rom[657] = 8'h00;
        rom[658] = 8'h00;
        rom[659] = 8'h25;
        rom[660] = 8'h00;
        rom[661] = 8'h00;
        rom[662] = 8'h00;
        rom[663] = 8'h00;
        rom[664] = 8'h38;
        rom[665] = 8'h42;
        rom[666] = 8'h00;
        rom[667] = 8'h01;
        rom[668] = 8'h30;
        rom[669] = 8'h42;
        rom[670] = 8'h00;
        rom[671] = 8'hff;
        rom[672] = 8'h14;
        rom[673] = 8'h40;
        rom[674] = 8'hff;
        rom[675] = 8'hb4;
        rom[676] = 8'h00;
        rom[677] = 8'h00;
        rom[678] = 8'h00;
        rom[679] = 8'h00;
        rom[680] = 8'h00;
        rom[681] = 8'h00;
        rom[682] = 8'h00;
        rom[683] = 8'h00;
        rom[684] = 8'h00;
        rom[685] = 8'h00;
        rom[686] = 8'h00;
        rom[687] = 8'h00;
        rom[688] = 8'h03;
        rom[689] = 8'hc0;
        rom[690] = 8'he8;
        rom[691] = 8'h25;
        rom[692] = 8'h8f;
        rom[693] = 8'hbf;
        rom[694] = 8'h00;
        rom[695] = 8'h1c;
        rom[696] = 8'h8f;
        rom[697] = 8'hbe;
        rom[698] = 8'h00;
        rom[699] = 8'h18;
        rom[700] = 8'h27;
        rom[701] = 8'hbd;
        rom[702] = 8'h00;
        rom[703] = 8'h20;
        rom[704] = 8'h03;
        rom[705] = 8'he0;
        rom[706] = 8'h00;
        rom[707] = 8'h08;
        rom[708] = 8'h00;
        rom[709] = 8'h00;
        rom[710] = 8'h00;
        rom[711] = 8'h00;
        rom[712] = 8'h27;
        rom[713] = 8'hbd;
        rom[714] = 8'hff;
        rom[715] = 8'hc8;
        rom[716] = 8'haf;
        rom[717] = 8'hbf;
        rom[718] = 8'h00;
        rom[719] = 8'h34;
        rom[720] = 8'haf;
        rom[721] = 8'hbe;
        rom[722] = 8'h00;
        rom[723] = 8'h30;
        rom[724] = 8'h03;
        rom[725] = 8'ha0;
        rom[726] = 8'hf0;
        rom[727] = 8'h25;
        rom[728] = 8'h24;
        rom[729] = 8'h02;
        rom[730] = 8'h00;
        rom[731] = 8'h02;
        rom[732] = 8'haf;
        rom[733] = 8'hc2;
        rom[734] = 8'h00;
        rom[735] = 8'h10;
        rom[736] = 8'h24;
        rom[737] = 8'h02;
        rom[738] = 8'h00;
        rom[739] = 8'h05;
        rom[740] = 8'haf;
        rom[741] = 8'hc2;
        rom[742] = 8'h00;
        rom[743] = 8'h14;
        rom[744] = 8'h24;
        rom[745] = 8'h02;
        rom[746] = 8'h00;
        rom[747] = 8'h01;
        rom[748] = 8'haf;
        rom[749] = 8'hc2;
        rom[750] = 8'h00;
        rom[751] = 8'h18;
        rom[752] = 8'h24;
        rom[753] = 8'h02;
        rom[754] = 8'h00;
        rom[755] = 8'h0f;
        rom[756] = 8'haf;
        rom[757] = 8'hc2;
        rom[758] = 8'h00;
        rom[759] = 8'h1c;
        rom[760] = 8'h24;
        rom[761] = 8'h02;
        rom[762] = 8'h00;
        rom[763] = 8'h07;
        rom[764] = 8'haf;
        rom[765] = 8'hc2;
        rom[766] = 8'h00;
        rom[767] = 8'h20;
        rom[768] = 8'h24;
        rom[769] = 8'h02;
        rom[770] = 8'h00;
        rom[771] = 8'h03;
        rom[772] = 8'haf;
        rom[773] = 8'hc2;
        rom[774] = 8'h00;
        rom[775] = 8'h24;
        rom[776] = 8'h24;
        rom[777] = 8'h02;
        rom[778] = 8'h00;
        rom[779] = 8'h0a;
        rom[780] = 8'haf;
        rom[781] = 8'hc2;
        rom[782] = 8'h00;
        rom[783] = 8'h28;
        rom[784] = 8'haf;
        rom[785] = 8'hc0;
        rom[786] = 8'h00;
        rom[787] = 8'h2c;
        rom[788] = 8'h24;
        rom[789] = 8'h05;
        rom[790] = 8'h00;
        rom[791] = 8'h08;
        rom[792] = 8'h27;
        rom[793] = 8'hc2;
        rom[794] = 8'h00;
        rom[795] = 8'h10;
        rom[796] = 8'h00;
        rom[797] = 8'h40;
        rom[798] = 8'h20;
        rom[799] = 8'h25;
        rom[800] = 8'h0c;
        rom[801] = 8'h00;
        rom[802] = 8'h00;
        rom[803] = 8'h55;
        rom[804] = 8'h00;
        rom[805] = 8'h00;
        rom[806] = 8'h00;
        rom[807] = 8'h00;
        rom[808] = 8'h00;
        rom[809] = 8'h00;
        rom[810] = 8'h10;
        rom[811] = 8'h25;
        rom[812] = 8'h03;
        rom[813] = 8'hc0;
        rom[814] = 8'he8;
        rom[815] = 8'h25;
        rom[816] = 8'h8f;
        rom[817] = 8'hbf;
        rom[818] = 8'h00;
        rom[819] = 8'h34;
        rom[820] = 8'h8f;
        rom[821] = 8'hbe;
        rom[822] = 8'h00;
        rom[823] = 8'h30;
        rom[824] = 8'h27;
        rom[825] = 8'hbd;
        rom[826] = 8'h00;
        rom[827] = 8'h38;
        rom[828] = 8'h03;
        rom[829] = 8'he0;
        rom[830] = 8'h00;
        rom[831] = 8'h08;
        rom[832] = 8'h00;
        rom[833] = 8'h00;
        rom[834] = 8'h00;
        rom[835] = 8'h00;
    end
    logic [Constants::WIDTH-1:0]          pc_wb;
    logic [Constants::BYTE-1:0] ram [0:Constants::RAM_SIZE-1];
    logic                                 rd_wb;
    logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_wb;
    logic [Constants::WIDTH-1:0]          rd_data_wb;
    logic [Constants::WIDTH-1:0] reg_file [0:Constants::REG_COUNT-1-1];

    logic clk_divided_25_000_000;
    divider #(
        .STOPPER(25_000_000)
    ) divider_25_000_000 (
        .clk(clk),
        .rst(rst_debounced_synced),
        .out(clk_divided_25_000_000)
    );
    
    logic clk_divided_turbo;
    divider #(
        .STOPPER(100_000)
    ) divider_turbo (
        .clk(clk),
        .rst(rst_debounced_synced),
        .out(clk_divided_turbo)
    );
    
    logic mips_r2000_clk;
    always_comb begin
        mips_r2000_clk = turbo ? clk_divided_turbo : clk_divided_25_000_000;
    end
    
    logic stall_stepped;
    stall_stepper stall_stepper_inst (
        .clk(mips_r2000_clk),
        .rst(rst_debounced_synced),
        .stall(stall_debounced_synced),
        .step(step_debounced_synced),
        .out(stall_stepped)
    );

    mips_r2000 mips_r2000_inst(
        .clk(mips_r2000_clk),
        .rst(rst_debounced_synced),
        .rom(rom),
        .stall(stall_stepped),

        .pc_wb(pc_wb),
        .ram(ram),
        .rd_wb(rd_wb),
        .rd_address_wb(rd_address_wb),
        .rd_data_wb(rd_data_wb),
        .reg_file(reg_file)
    );

    localparam logic[Constants::WIDTH-1:0] STACK_ARRAY_POINTER = Constants::RAM_SIZE - 56 + 16;
    logic [Constants::WIDTH-1:0] sevseg_number;
    logic [5-1:0] sevseg_selector;
    always_comb begin
        sevseg_selector = { show_pc, show_stack_array, show_ra, show_s8, show_sp };
        case (sevseg_selector)
            5'b10000: sevseg_number = pc_wb;
            5'b01000: sevseg_number = {
                ram[STACK_ARRAY_POINTER + (4*0) + 3][3:0],
                ram[STACK_ARRAY_POINTER + (4*1) + 3][3:0],
                ram[STACK_ARRAY_POINTER + (4*2) + 3][3:0],
                ram[STACK_ARRAY_POINTER + (4*3) + 3][3:0],
                ram[STACK_ARRAY_POINTER + (4*4) + 3][3:0],
                ram[STACK_ARRAY_POINTER + (4*5) + 3][3:0],
                ram[STACK_ARRAY_POINTER + (4*6) + 3][3:0],
                ram[STACK_ARRAY_POINTER + (4*7) + 3][3:0]
            };
            5'b00100: sevseg_number = reg_file[30]; // ra
            5'b00010: sevseg_number = reg_file[29]; // s8
            5'b00001: sevseg_number = reg_file[28]; // sp
            default: sevseg_number = 32'hABBABABA;
        endcase
    end
    
    sevseg sevseg_inst(
        .clk(clk),
        .rst(rst_debounced_synced),
        .number(sevseg_number),
        .an(an),
        .seg(seg),
        .dp(dp)
    );
endmodule