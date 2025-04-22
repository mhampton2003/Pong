

module DE10_Standard_Computer
//=======================================================
//  PARAMETER declarations
//=======================================================
// #(
// )


//=======================================================
//  PORT declarations
//=======================================================
(
////////////////////////////////////
// FPGA Pins
////////////////////////////////////

// Clock pins
input               CLOCK_50,
input               CLOCK2_50,
input               CLOCK3_50,
input               CLOCK4_50,

// ADC
output              ADC_CONVST,
output              ADC_SCLK,
output              ADC_SDI,
input               ADC_SDO,

// Audio
input               AUD_ADCDAT,
inout               AUD_ADCLRCK,
inout               AUD_BCLK,
output              AUD_DACDAT,
inout               AUD_DACLRCK,
output              AUD_XCK,

// SDRAM
output      [12: 0] DRAM_ADDR,
output      [ 1: 0] DRAM_BA,
output              DRAM_CAS_N,
output              DRAM_CKE,
output              DRAM_CLK,
output              DRAM_CS_N,
inout       [15: 0] DRAM_DQ,
output              DRAM_LDQM,
output              DRAM_RAS_N,
output              DRAM_UDQM,
output              DRAM_WE_N,

// I2C Bus for Configuration of the Audio and Video-In Chips
output              FPGA_I2C_SCLK,
inout               FPGA_I2C_SDAT,

// 40-pin headers
inout       [35: 0] GPIO,

// Seven Segment Displays
output      [ 6: 0] HEX0,
output      [ 6: 0] HEX1,
output      [ 6: 0] HEX2,
output      [ 6: 0] HEX3,
output      [ 6: 0] HEX4,
output      [ 6: 0] HEX5,

// IR
input               IRDA_RXD,
output              IRDA_TXD,

// Pushbuttons
input       [ 3: 0] KEY,

// LEDs
output      [ 9: 0] LEDR,

// PS2 Ports
inout               PS2_CLK,
inout               PS2_DAT,

inout               PS2_CLK2,
inout               PS2_DAT2,

// Slider Switches
input       [ 9: 0] SW,

// Video-In
input               TD_CLK27,
input       [ 7: 0] TD_DATA,
input               TD_HS,
output              TD_RESET_N,
input               TD_VS,

// VGA
output      [ 7: 0] VGA_B,
output              VGA_BLANK_N,
output              VGA_CLK,
output      [ 7: 0] VGA_G,
output              VGA_HS,
output      [ 7: 0] VGA_R,
output              VGA_SYNC_N,
output              VGA_VS,

////////////////////////////////////
// HPS Pins
////////////////////////////////////
    
// DDR3 SDRAM
output      [14: 0] HPS_DDR3_ADDR,
output      [ 2: 0] HPS_DDR3_BA,
output              HPS_DDR3_CAS_N,
output              HPS_DDR3_CKE,
output              HPS_DDR3_CK_N,
output              HPS_DDR3_CK_P,
output              HPS_DDR3_CS_N,
output      [ 3: 0] HPS_DDR3_DM,
inout       [31: 0] HPS_DDR3_DQ,
inout       [ 3: 0] HPS_DDR3_DQS_N,
inout       [ 3: 0] HPS_DDR3_DQS_P,
output              HPS_DDR3_ODT,
output              HPS_DDR3_RAS_N,
output              HPS_DDR3_RESET_N,
input               HPS_DDR3_RZQ,
output              HPS_DDR3_WE_N,

// Ethernet
output              HPS_ENET_GTX_CLK,
inout               HPS_ENET_INT_N,
output              HPS_ENET_MDC,
inout               HPS_ENET_MDIO,
input               HPS_ENET_RX_CLK,
input       [ 3: 0] HPS_ENET_RX_DATA,
input               HPS_ENET_RX_DV,
output      [ 3: 0] HPS_ENET_TX_DATA,
output              HPS_ENET_TX_EN,

// Flash
inout       [ 3: 0] HPS_FLASH_DATA,
output              HPS_FLASH_DCLK,
output              HPS_FLASH_NCSO,

// Accelerometer
inout               HPS_GSENSOR_INT,

// I2C
inout               HPS_I2C_CONTROL,
inout               HPS_I2C1_SCLK,
inout               HPS_I2C1_SDAT,
inout               HPS_I2C2_SCLK,
inout               HPS_I2C2_SDAT,

// Pushbutton
inout               HPS_KEY,

// LCD
inout               HPS_LCM_BK,
inout               HPS_LCM_D_C,
inout               HPS_LCM_RST_N,
input               HPS_LCM_SPIM_CLK,
output              HPS_LCM_SPIM_MOSI,
input               HPS_LCM_SPIM_MISO,
output              HPS_LCM_SPIM_SS,

// Green LED
inout               HPS_LED,

// LTC Connector
inout               HPS_LTC_GPIO,

// Reset
inout               HPS_RESET_N,

// SD Card
output              HPS_SD_CLK,
inout               HPS_SD_CMD,
inout       [ 3: 0] HPS_SD_DATA,

// SPI
output              HPS_SPIM_CLK,
input               HPS_SPIM_MISO,
output              HPS_SPIM_MOSI,
output              HPS_SPIM_SS,

// UART
input               HPS_UART_RX,
output              HPS_UART_TX,

// USB
inout               HPS_CONV_USB_N,
input               HPS_USB_CLKOUT,
inout       [ 7: 0] HPS_USB_DATA,
input               HPS_USB_DIR,
input               HPS_USB_NXT,
output              HPS_USB_STP
);

//=======================================================
//  REG/WIRE declarations
//=======================================================

wire        [31: 0] hex3_hex0;
wire        [15: 0] hex5_hex4;

//assign HEX0 = ~hex3_hex0[ 6: 0];
//assign HEX1 = ~hex3_hex0[14: 8]; // comment out for pong
assign HEX2 = ~hex3_hex0[22:16];
assign HEX3 = ~hex3_hex0[30:24];
assign HEX4 = ~hex5_hex4[ 6: 0];
assign HEX5 = ~hex5_hex4[14: 8];

//=======================================================
//  Structural coding
//=======================================================

Computer_System The_System (
    ////////////////////////////////////
    // FPGA Side
    ////////////////////////////////////

    // Global signals
    .system_pll_ref_clk_clk				(CLOCK_50),
    .system_pll_ref_reset_reset			(1'b0),
	.video_pll_ref_clk_clk				(CLOCK2_50),
	.video_pll_ref_reset_reset			(1'b0),

    // ADC
	.adc_sclk							(ADC_SCLK),
	.adc_cs_n							(ADC_CONVST),
	.adc_dout							(ADC_SDO),
	.adc_din							(ADC_SDI),

    // AV Config
    .av_config_SCLK                     (FPGA_I2C_SCLK),
    .av_config_SDAT                     (FPGA_I2C_SDAT),

    // Audio Subsystem
    .audio_pll_ref_clk_clk              (CLOCK3_50),
    .audio_pll_ref_reset_reset          (1'b0),
    .audio_pll_clk_clk                  (AUD_XCK),
    .audio_ADCDAT                       (AUD_ADCDAT),
    .audio_ADCLRCK                      (AUD_ADCLRCK),
    .audio_BCLK                         (AUD_BCLK),
    .audio_DACDAT                       (AUD_DACDAT),
    .audio_DACLRCK                      (AUD_DACLRCK),

    // Slider Switches
    .slider_switches_export             (SW),

    // Pushbuttons
    .pushbuttons_export                 (~KEY[3:0]),

    // Expansion JP1
    .expansion_jp1_export               (GPIO[35:0]),

    // LEDs
    .leds_export                        (LEDR),
    
    // Seven Segs
    .hex3_hex0_export                   (hex3_hex0),
    .hex5_hex4_export                   (hex5_hex4),
    
    // PS2 Ports
    .ps2_port_CLK                       (PS2_CLK),
    .ps2_port_DAT                       (PS2_DAT),
    .ps2_port_dual_CLK                  (PS2_CLK2),
    .ps2_port_dual_DAT                  (PS2_DAT2),

    // IrDA
    .irda_RXD                           (IRDA_RXD),
    .irda_TXD                           (IRDA_TXD),

    // VGA Subsystem
    .vga_CLK                            (VGA_CLK),
    .vga_BLANK                          (VGA_BLANK_N),
    .vga_SYNC                           (VGA_SYNC_N),
    .vga_HS                             (VGA_HS),
    .vga_VS                             (VGA_VS),
    .vga_R                              (VGA_R),
    .vga_G                              (VGA_G),
    .vga_B                              (VGA_B),
    
    // Video In Subsystem
    .video_in_TD_CLK27                  (TD_CLK27),
    .video_in_TD_DATA                   (TD_DATA),
    .video_in_TD_HS                     (TD_HS),
    .video_in_TD_VS                     (TD_VS),
    .video_in_clk27_reset               (),
    .video_in_TD_RESET                  (TD_RESET_N),
    .video_in_overflow_flag             (),
    
    // SDRAM
    .sdram_clk_clk                      (DRAM_CLK),
    .sdram_addr                         (DRAM_ADDR),
    .sdram_ba                           (DRAM_BA),
    .sdram_cas_n                        (DRAM_CAS_N),
    .sdram_cke                          (DRAM_CKE),
    .sdram_cs_n                         (DRAM_CS_N),
    .sdram_dq                           (DRAM_DQ),
    .sdram_dqm                          ({DRAM_UDQM,DRAM_LDQM}),
    .sdram_ras_n                        (DRAM_RAS_N),
    .sdram_we_n                         (DRAM_WE_N),
    
    ////////////////////////////////////
    // HPS Side
    ////////////////////////////////////
	// Accelerometer
	.hps_io_hps_io_gpio_inst_GPIO61		(HPS_GSENSOR_INT),

    // DDR3 SDRAM
    .memory_mem_a                       (HPS_DDR3_ADDR),
    .memory_mem_ba                      (HPS_DDR3_BA),
    .memory_mem_ck                      (HPS_DDR3_CK_P),
    .memory_mem_ck_n                    (HPS_DDR3_CK_N),
    .memory_mem_cke                     (HPS_DDR3_CKE),
    .memory_mem_cs_n                    (HPS_DDR3_CS_N),
    .memory_mem_ras_n                   (HPS_DDR3_RAS_N),
    .memory_mem_cas_n                   (HPS_DDR3_CAS_N),
    .memory_mem_we_n                    (HPS_DDR3_WE_N),
    .memory_mem_reset_n                 (HPS_DDR3_RESET_N),
    .memory_mem_dq                      (HPS_DDR3_DQ),
    .memory_mem_dqs                     (HPS_DDR3_DQS_P),
    .memory_mem_dqs_n                   (HPS_DDR3_DQS_N),
    .memory_mem_odt                     (HPS_DDR3_ODT),
    .memory_mem_dm                      (HPS_DDR3_DM),
    .memory_oct_rzqin                   (HPS_DDR3_RZQ),
          
    // Ethernet
    .hps_io_hps_io_gpio_inst_GPIO35     (HPS_ENET_INT_N),
    .hps_io_hps_io_emac1_inst_TX_CLK    (HPS_ENET_GTX_CLK),
    .hps_io_hps_io_emac1_inst_TXD0      (HPS_ENET_TX_DATA[0]),
    .hps_io_hps_io_emac1_inst_TXD1      (HPS_ENET_TX_DATA[1]),
    .hps_io_hps_io_emac1_inst_TXD2      (HPS_ENET_TX_DATA[2]),
    .hps_io_hps_io_emac1_inst_TXD3      (HPS_ENET_TX_DATA[3]),
    .hps_io_hps_io_emac1_inst_RXD0      (HPS_ENET_RX_DATA[0]),
    .hps_io_hps_io_emac1_inst_MDIO      (HPS_ENET_MDIO),
    .hps_io_hps_io_emac1_inst_MDC       (HPS_ENET_MDC),
    .hps_io_hps_io_emac1_inst_RX_CTL    (HPS_ENET_RX_DV),
    .hps_io_hps_io_emac1_inst_TX_CTL    (HPS_ENET_TX_EN),
    .hps_io_hps_io_emac1_inst_RX_CLK    (HPS_ENET_RX_CLK),
    .hps_io_hps_io_emac1_inst_RXD1      (HPS_ENET_RX_DATA[1]),
    .hps_io_hps_io_emac1_inst_RXD2      (HPS_ENET_RX_DATA[2]),
    .hps_io_hps_io_emac1_inst_RXD3      (HPS_ENET_RX_DATA[3]),

    // Flash
    .hps_io_hps_io_qspi_inst_IO0        (HPS_FLASH_DATA[0]),
    .hps_io_hps_io_qspi_inst_IO1        (HPS_FLASH_DATA[1]),
    .hps_io_hps_io_qspi_inst_IO2        (HPS_FLASH_DATA[2]),
    .hps_io_hps_io_qspi_inst_IO3        (HPS_FLASH_DATA[3]),
    .hps_io_hps_io_qspi_inst_SS0        (HPS_FLASH_NCSO),
    .hps_io_hps_io_qspi_inst_CLK        (HPS_FLASH_DCLK),

    // I2C
    .hps_io_hps_io_gpio_inst_GPIO48     (HPS_I2C_CONTROL),
    .hps_io_hps_io_i2c0_inst_SDA        (HPS_I2C1_SDAT),
    .hps_io_hps_io_i2c0_inst_SCL        (HPS_I2C1_SCLK),
    .hps_io_hps_io_i2c1_inst_SDA        (HPS_I2C2_SDAT),
    .hps_io_hps_io_i2c1_inst_SCL        (HPS_I2C2_SCLK),

    // Pushbutton
    .hps_io_hps_io_gpio_inst_GPIO54     (HPS_KEY),

    // LCD
    .hps_io_hps_io_gpio_inst_GPIO37     (HPS_LCM_BK),
    .hps_io_hps_io_gpio_inst_GPIO41     (HPS_LCM_D_C),
    .hps_io_hps_io_gpio_inst_GPIO44     (HPS_LCM_RST_N),
    .hps_io_hps_io_spim0_inst_CLK       (HPS_LCM_SPIM_CLK),
    .hps_io_hps_io_spim0_inst_MOSI      (HPS_LCM_SPIM_MOSI),
    .hps_io_hps_io_spim0_inst_MISO      (HPS_LCM_SPIM_MISO),
    .hps_io_hps_io_spim0_inst_SS0       (HPS_LCM_SPIM_SS),

    // LED
    .hps_io_hps_io_gpio_inst_GPIO53     (HPS_LED),

    // LTC Connector
    .hps_io_hps_io_gpio_inst_GPIO40     (HPS_LTC_GPIO),

    // SD Card
    .hps_io_hps_io_sdio_inst_CMD        (HPS_SD_CMD),
    .hps_io_hps_io_sdio_inst_D0         (HPS_SD_DATA[0]),
    .hps_io_hps_io_sdio_inst_D1         (HPS_SD_DATA[1]),
    .hps_io_hps_io_sdio_inst_CLK        (HPS_SD_CLK),
    .hps_io_hps_io_sdio_inst_D2         (HPS_SD_DATA[2]),
    .hps_io_hps_io_sdio_inst_D3         (HPS_SD_DATA[3]),

    // SPI
    .hps_io_hps_io_spim1_inst_CLK       (HPS_SPIM_CLK),
    .hps_io_hps_io_spim1_inst_MOSI      (HPS_SPIM_MOSI),
    .hps_io_hps_io_spim1_inst_MISO      (HPS_SPIM_MISO),
    .hps_io_hps_io_spim1_inst_SS0       (HPS_SPIM_SS),

    // UART
    .hps_io_hps_io_uart0_inst_RX        (HPS_UART_RX),
    .hps_io_hps_io_uart0_inst_TX        (HPS_UART_TX),

    // USB
    .hps_io_hps_io_gpio_inst_GPIO09     (HPS_CONV_USB_N),
    .hps_io_hps_io_usb1_inst_D0         (HPS_USB_DATA[0]),
    .hps_io_hps_io_usb1_inst_D1         (HPS_USB_DATA[1]),
    .hps_io_hps_io_usb1_inst_D2         (HPS_USB_DATA[2]),
    .hps_io_hps_io_usb1_inst_D3         (HPS_USB_DATA[3]),
    .hps_io_hps_io_usb1_inst_D4         (HPS_USB_DATA[4]),
    .hps_io_hps_io_usb1_inst_D5         (HPS_USB_DATA[5]),
    .hps_io_hps_io_usb1_inst_D6         (HPS_USB_DATA[6]),
    .hps_io_hps_io_usb1_inst_D7         (HPS_USB_DATA[7]),
    .hps_io_hps_io_usb1_inst_CLK        (HPS_USB_CLKOUT),
    .hps_io_hps_io_usb1_inst_STP        (HPS_USB_STP),
    .hps_io_hps_io_usb1_inst_DIR        (HPS_USB_DIR),
    .hps_io_hps_io_usb1_inst_NXT        (HPS_USB_NXT)
);

// Wires for score counter
wire [3:0] score_ones;
wire [3:0] score_tens;

// initialize counter
score_counter scr_counter (
    .clk(CLOCK_50),
    .rst(SW[9]),           // active high reset
    .incr(GPIO[0]),			// pulse from game logic
    .ones(score_ones),
    .tens(score_tens)
);

// initialize bcd decoder for each HEX display
bcd_7segment ones_counter (.bcd_in(score_ones), .seven_segment_out(HEX0[6:0]));
bcd_7segment tens_counter (.bcd_in(score_tens), .seven_segment_out(HEX1[6:0]));


endmodule