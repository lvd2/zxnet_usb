onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -height 15 /tb/fclk
add wave -noupdate -height 15 /tb/rst_n
add wave -noupdate -height 15 /tb/clk
add wave -noupdate -divider <NULL>
add wave -noupdate -height 15 -radix hexadecimal /tb/a
add wave -noupdate -height 15 -radix hexadecimal -childformat {{{/tb/d[7]} -radix hexadecimal} {{/tb/d[6]} -radix hexadecimal} {{/tb/d[5]} -radix hexadecimal} {{/tb/d[4]} -radix hexadecimal} {{/tb/d[3]} -radix hexadecimal} {{/tb/d[2]} -radix hexadecimal} {{/tb/d[1]} -radix hexadecimal} {{/tb/d[0]} -radix hexadecimal}} -subitemconfig {{/tb/d[7]} {-height 15 -radix hexadecimal} {/tb/d[6]} {-height 15 -radix hexadecimal} {/tb/d[5]} {-height 15 -radix hexadecimal} {/tb/d[4]} {-height 15 -radix hexadecimal} {/tb/d[3]} {-height 15 -radix hexadecimal} {/tb/d[2]} {-height 15 -radix hexadecimal} {/tb/d[1]} {-height 15 -radix hexadecimal} {/tb/d[0]} {-height 15 -radix hexadecimal}} /tb/d
add wave -noupdate -height 15 /tb/mreq_n
add wave -noupdate -height 15 /tb/iorq_n
add wave -noupdate -height 15 /tb/rd_n
add wave -noupdate -height 15 /tb/wr_n
add wave -noupdate -height 15 /tb/int_n
add wave -noupdate -height 15 /tb/blkrom
add wave -noupdate -height 15 /tb/iorqge
add wave -noupdate -height 15 /tb/csrom_n
add wave -noupdate -divider <NULL>
add wave -noupdate -height 15 -radix hexadecimal /tb/where_rom
add wave -noupdate -height 15 /tb/usb_power
add wave -noupdate -divider <NULL>
add wave -noupdate -height 15 /tb/brd_n
add wave -noupdate -height 15 /tb/bwr_n
add wave -noupdate -height 15 -radix hexadecimal /tb/bd
add wave -noupdate -divider <NULL>
add wave -noupdate -height 15 /tb/sl811_a0
add wave -noupdate -height 15 /tb/sl811_cs_n
add wave -noupdate -height 15 /tb/sl811_intrq
add wave -noupdate -height 15 /tb/sl811_ms_n
add wave -noupdate -height 15 /tb/sl811_rst_n
add wave -noupdate -height 15 /tb/s/rd
add wave -noupdate -height 15 /tb/s/wr
add wave -noupdate -divider <NULL>
add wave -noupdate -height 15 /tb/s/rst_n
add wave -noupdate -height 15 /tb/s/a0
add wave -noupdate -height 15 /tb/s/cs_n
add wave -noupdate -height 15 /tb/s/rd_n
add wave -noupdate -height 15 /tb/s/wr_n
add wave -noupdate -height 15 /tb/s/ms
add wave -noupdate -height 15 /tb/s/intrq
add wave -noupdate -height 15 -radix hexadecimal /tb/s/d
add wave -noupdate -height 15 /tb/s/access_addr
add wave -noupdate -height 15 /tb/s/access_rnw
add wave -noupdate -height 15 -radix hexadecimal /tb/s/wr_data
add wave -noupdate -height 15 -radix hexadecimal /tb/s/rd_data
add wave -noupdate -height 15 /tb/s/rd
add wave -noupdate -height 15 /tb/s/wr
add wave -noupdate -divider <NULL>
add wave -noupdate -height 15 /tb/DUT/ports/w5300_ports
add wave -noupdate -height 15 /tb/DUT/ports/w5300_hi
add wave -noupdate -divider <NULL>
add wave -noupdate -height 15 -radix hexadecimal -childformat {{{/tb/w5300_addr[9]} -radix hexadecimal} {{/tb/w5300_addr[8]} -radix hexadecimal} {{/tb/w5300_addr[7]} -radix hexadecimal} {{/tb/w5300_addr[6]} -radix hexadecimal} {{/tb/w5300_addr[5]} -radix hexadecimal} {{/tb/w5300_addr[4]} -radix hexadecimal} {{/tb/w5300_addr[3]} -radix hexadecimal} {{/tb/w5300_addr[2]} -radix hexadecimal} {{/tb/w5300_addr[1]} -radix hexadecimal} {{/tb/w5300_addr[0]} -radix hexadecimal}} -subitemconfig {{/tb/w5300_addr[9]} {-height 15 -radix hexadecimal} {/tb/w5300_addr[8]} {-height 15 -radix hexadecimal} {/tb/w5300_addr[7]} {-height 15 -radix hexadecimal} {/tb/w5300_addr[6]} {-height 15 -radix hexadecimal} {/tb/w5300_addr[5]} {-height 15 -radix hexadecimal} {/tb/w5300_addr[4]} {-height 15 -radix hexadecimal} {/tb/w5300_addr[3]} {-height 15 -radix hexadecimal} {/tb/w5300_addr[2]} {-height 15 -radix hexadecimal} {/tb/w5300_addr[1]} {-height 15 -radix hexadecimal} {/tb/w5300_addr[0]} {-height 15 -radix hexadecimal}} /tb/w5300_addr
add wave -noupdate -height 15 /tb/w5300_cs_n
add wave -noupdate -height 15 /tb/w5300_int_n
add wave -noupdate -height 15 /tb/w5300_rst_n
add wave -noupdate -height 15 /tb/w/rd
add wave -noupdate -height 15 /tb/w/wr
add wave -noupdate -divider <NULL>
add wave -noupdate -height 15 /tb/w/rst_n
add wave -noupdate -height 15 -radix hexadecimal /tb/w/addr
add wave -noupdate -height 15 /tb/w/cs_n
add wave -noupdate -height 15 /tb/w/rd_n
add wave -noupdate -height 15 /tb/w/wr_n
add wave -noupdate -height 15 /tb/w/int_n
add wave -noupdate -height 15 -radix hexadecimal /tb/w/d
add wave -noupdate -height 15 -radix hexadecimal /tb/w/access_addr
add wave -noupdate -height 15 /tb/w/access_rnw
add wave -noupdate -height 15 -radix hexadecimal /tb/w/rd_data
add wave -noupdate -height 15 -radix hexadecimal /tb/w/wr_data
add wave -noupdate -height 15 /tb/w/rd
add wave -noupdate -height 15 /tb/w/wr
add wave -noupdate -divider <NULL>
add wave -noupdate -height 15 /tb/DUT/fclk
add wave -noupdate -height 15 /tb/DUT/usb_clk
add wave -noupdate -height 15 /tb/DUT/usb_ckreg
add wave -noupdate -height 15 -radix hexadecimal /tb/DUT/za
add wave -noupdate -height 15 -radix hexadecimal /tb/DUT/zd
add wave -noupdate -height 15 -radix hexadecimal /tb/DUT/bd
add wave -noupdate -height 15 /tb/DUT/ziorq_n
add wave -noupdate -height 15 /tb/DUT/zrd_n
add wave -noupdate -height 15 /tb/DUT/zwr_n
add wave -noupdate -height 15 /tb/DUT/zmreq_n
add wave -noupdate -height 15 /tb/DUT/ziorqge
add wave -noupdate -height 15 /tb/DUT/zblkrom
add wave -noupdate -height 15 /tb/DUT/zcsrom_n
add wave -noupdate -height 15 /tb/DUT/zrst_n
add wave -noupdate -height 15 /tb/DUT/zint_n
add wave -noupdate -height 15 /tb/DUT/brd_n
add wave -noupdate -height 15 /tb/DUT/bwr_n
add wave -noupdate -height 15 /tb/DUT/w5300_rst_n
add wave -noupdate -height 15 /tb/DUT/w5300_addr
add wave -noupdate -height 15 /tb/DUT/w5300_cs_n
add wave -noupdate -height 15 /tb/DUT/w5300_int_n
add wave -noupdate -height 15 /tb/DUT/sl811_rst_n
add wave -noupdate -height 15 /tb/DUT/sl811_intrq
add wave -noupdate -height 15 /tb/DUT/sl811_ms_n
add wave -noupdate -height 15 /tb/DUT/sl811_cs_n
add wave -noupdate -height 15 /tb/DUT/sl811_a0
add wave -noupdate -height 15 /tb/DUT/usb_power
add wave -noupdate -height 15 /tb/DUT/pre_brd_n
add wave -noupdate -height 15 /tb/DUT/bwr_n_r
add wave -noupdate -height 15 /tb/DUT/bwr_n_rr
add wave -noupdate -height 15 /tb/DUT/shreg
add wave -noupdate -height 15 /tb/DUT/ena_w5300_int
add wave -noupdate -height 15 /tb/DUT/ena_sl811_int
add wave -noupdate -height 15 /tb/DUT/ena_zxbus_int
add wave -noupdate -height 15 /tb/DUT/internal_int
add wave -noupdate -height 15 /tb/DUT/ports_wrdata
add wave -noupdate -height 15 /tb/DUT/ports_rddata
add wave -noupdate -height 15 /tb/DUT/ports_addr
add wave -noupdate -height 15 /tb/DUT/ports_wrena
add wave -noupdate -height 15 /tb/DUT/ports_wrstb_n
add wave -noupdate -height 15 /tb/DUT/rommap_win
add wave -noupdate -height 15 /tb/DUT/rommap_ena
add wave -noupdate -height 15 /tb/DUT/w5300_a0inv
add wave -noupdate -height 15 /tb/DUT/w5300_ports
add wave -noupdate -height 15 /tb/DUT/w5300_hi
add wave -noupdate -height 15 /tb/DUT/pre_w5300_addr
add wave -noupdate -divider <NULL>
add wave -noupdate -height 15 /tb/DUT/zbus/fclk
add wave -noupdate -height 15 -radix hexadecimal /tb/DUT/zbus/za
add wave -noupdate -height 15 -radix hexadecimal /tb/DUT/zbus/zd
add wave -noupdate -height 15 -radix hexadecimal /tb/DUT/zbus/bd
add wave -noupdate -height 15 /tb/DUT/zbus/ziorq_n
add wave -noupdate -height 15 /tb/DUT/zbus/zrd_n
add wave -noupdate -height 15 /tb/DUT/zbus/zwr_n
add wave -noupdate -height 15 /tb/DUT/zbus/zmreq_n
add wave -noupdate -height 15 /tb/DUT/zbus/ziorqge
add wave -noupdate -height 15 /tb/DUT/zbus/zblkrom
add wave -noupdate -height 15 /tb/DUT/zbus/zcsrom_n
add wave -noupdate -height 15 /tb/DUT/zbus/zrst_n
add wave -noupdate -height 15 /tb/DUT/zbus/ports_wrena
add wave -noupdate -height 15 /tb/DUT/zbus/ports_wrstb_n
add wave -noupdate -height 15 -radix hexadecimal /tb/DUT/zbus/ports_addr
add wave -noupdate -height 15 -radix hexadecimal /tb/DUT/zbus/ports_wrdata
add wave -noupdate -height 15 -radix hexadecimal /tb/DUT/zbus/ports_rddata
add wave -noupdate -height 15 /tb/DUT/zbus/rommap_win
add wave -noupdate -height 15 /tb/DUT/zbus/rommap_ena
add wave -noupdate -height 15 /tb/DUT/zbus/sl811_cs_n
add wave -noupdate -height 15 /tb/DUT/zbus/sl811_a0
add wave -noupdate -height 15 /tb/DUT/zbus/w5300_cs_n
add wave -noupdate -height 15 /tb/DUT/zbus/w5300_ports
add wave -noupdate -height 15 -radix hexadecimal /tb/DUT/zbus/async_w5300_addr
add wave -noupdate -height 15 -radix hexadecimal /tb/DUT/zbus/w5300_addr
add wave -noupdate -height 15 /tb/DUT/zbus/bwr_n
add wave -noupdate -height 15 /tb/DUT/zbus/brd_n
add wave -noupdate -height 15 /tb/DUT/zbus/io_addr_ok
add wave -noupdate -height 15 /tb/DUT/zbus/mrd
add wave -noupdate -height 15 /tb/DUT/zbus/mwr
add wave -noupdate -height 15 /tb/DUT/zbus/ena_dbuf
add wave -noupdate -height 15 /tb/DUT/zbus/ena_din
add wave -noupdate -height 15 /tb/DUT/zbus/ena_dout
add wave -noupdate -height 15 /tb/DUT/zbus/async_w5300_cs_n
add wave -noupdate -height 15 /tb/DUT/zbus/async_sl811_cs_n
add wave -noupdate -height 15 /tb/DUT/zbus/r_w5300_cs_n
add wave -noupdate -height 15 /tb/DUT/zbus/r_sl811_cs_n
add wave -noupdate -height 15 /tb/DUT/zbus/async_sl811_a0
add wave -noupdate -height 15 /tb/DUT/zbus/r_sl811_a0
add wave -noupdate -height 15 /tb/DUT/zbus/wr_regs
add wave -noupdate -height 15 /tb/DUT/zbus/rd_regs
add wave -noupdate -height 15 /tb/DUT/zbus/wr_state
add wave -noupdate -height 15 /tb/DUT/zbus/rd_state
add wave -noupdate -height 15 /tb/DUT/zbus/wr_start
add wave -noupdate -height 15 /tb/DUT/zbus/rd_start
add wave -noupdate -height 15 -radix unsigned /tb/DUT/zbus/ctr_5
add wave -noupdate -height 15 -radix hexadecimal /tb/DUT/zbus/read_latch
add wave -noupdate -height 15 -radix hexadecimal /tb/DUT/zbus/write_latch
add wave -noupdate -height 15 /tb/DUT/zbus/ports_rd
add wave -noupdate -height 15 /tb/DUT/zbus/b_ena_dbuf
add wave -noupdate -height 15 /tb/DUT/zbus/b_ena_din
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {6481100 ps} 0}
configure wave -namecolwidth 257
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 70
configure wave -griddelta 8
configure wave -timeline 1
configure wave -timelineunits ns
update
WaveRestoreZoom {5792900 ps} {6894500 ps}
