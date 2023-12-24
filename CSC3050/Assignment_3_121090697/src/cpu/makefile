test:
	iverilog -o run.out test_cpu.v cpu.v
	vvp -n run.out
	gtkwave test.vcd
