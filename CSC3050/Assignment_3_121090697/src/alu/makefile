test:
	iverilog -o run.out test_alu.v alu.v
	vvp -n run.out
	gtkwave test.vcd
