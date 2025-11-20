module registers();    
    reg zero;        // 0 Constant 0
    reg at;          // 1 Reserved for assembler
    reg v0;          // 2 Expression evaluation and
    reg v1;          // 3 results of a function
    reg a0;          // 4 Argument 1
    reg a1;          // 5 Argument 2
    reg a2;          // 6 Argument 3
    reg a3;          // 7 Argument 4
    reg t0;          // 8 Temporary (not preserved across call)
    reg t1;          // 9 Temporary (not preserved across call)
    reg t2;          // 10 Temporary (not preserved across call)
    reg t3;          // 11 Temporary (not preserved across call)
    reg t4;          // 12 Temporary (not preserved across call)
    reg t5;          // 13 Temporary (not preserved across call)
    reg t6;          // 14 Temporary (not preserved across call)
    reg t7;          // 15 Temporary (not preserved across call)
    reg s0;          // 16 Saved temporary (preserved across call)
    reg s1;          // 17 Saved temporary (preserved across call)
    reg s2;          // 18 Saved temporary (preserved across call)
    reg s3;          // 19 Saved temporary (preserved across call)
    reg s4;          // 20 Saved temporary (preserved across call)
    reg s5;          // 21 Saved temporary (preserved across call)
    reg s6;          // 22 Saved temporary (preserved across call)
    reg s7;          // 23 Saved temporary (preserved across call)
    reg t8;          // 24 Temporary (not preserved across call)
    reg t9;          // 25 Temporary (not preserved across call)
    reg k0;          // 26 Reserved for OS kernel
    reg k1;          // 27 Reserved for OS kernel
    reg gp;          // 28 Pointer to global area
    reg sp;          // 29 Stack pointer
    reg fp_or_s8;    // 30 Frame pointer
    reg ra;          // 31 Return address (used by function call) 
endmodule