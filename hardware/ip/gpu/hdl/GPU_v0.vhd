library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity GPU is
	generic (
		-- Users to add parameters here

		-- User parameters ends
		-- Do not modify the parameters beyond this line


		-- Parameters of Axi Master Bus Interface M_AXI
		C_M_AXI_TARGET_SLAVE_BASE_ADDR	: std_logic_vector	:= x"40000000";
		C_M_AXI_BURST_LEN	: integer	:= 16;
		C_M_AXI_ID_WIDTH	: integer	:= 1;
		C_M_AXI_ADDR_WIDTH	: integer	:= 32;
		C_M_AXI_DATA_WIDTH	: integer	:= 32;

		-- Parameters of Axi Slave Bus Interface S_AXI
		C_S_AXI_DATA_WIDTH	: integer	:= 32;
		C_S_AXI_ADDR_WIDTH	: integer	:= 7
	);
	port (
		-- Users to add ports here

		-- User ports ends
		-- Do not modify the ports beyond this line


		-- Ports of Axi Master Bus Interface M_AXI
		m_axi_aclk	: in std_logic;
		m_axi_aresetn	: in std_logic;
		m_axi_awid	: out std_logic_vector(C_M_AXI_ID_WIDTH-1 downto 0);
		m_axi_awaddr	: out std_logic_vector(C_M_AXI_ADDR_WIDTH-1 downto 0);
		m_axi_awlen	: out std_logic_vector(7 downto 0);
		m_axi_awsize	: out std_logic_vector(2 downto 0);
		m_axi_awburst	: out std_logic_vector(1 downto 0);
		m_axi_awlock	: out std_logic_vector(1 downto 0);
		m_axi_awcache	: out std_logic_vector(3 downto 0);
		m_axi_awprot	: out std_logic_vector(2 downto 0);
		m_axi_awqos	: out std_logic_vector(3 downto 0);
		m_axi_awvalid	: out std_logic;
		m_axi_awready	: in std_logic;
		m_axi_wid   : out std_logic_vector(5 downto 0);
		m_axi_wdata	: out std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		m_axi_wstrb	: out std_logic_vector(C_M_AXI_DATA_WIDTH/8-1 downto 0);
		m_axi_wlast	: out std_logic;
		m_axi_wvalid	: out std_logic;
		m_axi_wready	: in std_logic;
		m_axi_bid	: in std_logic_vector(C_M_AXI_ID_WIDTH-1 downto 0);
		m_axi_bresp	: in std_logic_vector(1 downto 0);
		m_axi_bvalid	: in std_logic;
		m_axi_bready	: out std_logic;
		m_axi_arid	: out std_logic_vector(C_M_AXI_ID_WIDTH-1 downto 0);
		m_axi_araddr	: out std_logic_vector(C_M_AXI_ADDR_WIDTH-1 downto 0);
		m_axi_arlen	: out std_logic_vector(7 downto 0);
		m_axi_arsize	: out std_logic_vector(2 downto 0);
		m_axi_arburst	: out std_logic_vector(1 downto 0);
		m_axi_arlock	: out std_logic_vector(1 downto 0);
		m_axi_arcache	: out std_logic_vector(3 downto 0);
		m_axi_arprot	: out std_logic_vector(2 downto 0);
		m_axi_arqos	: out std_logic_vector(3 downto 0);
		m_axi_arvalid	: out std_logic;
		m_axi_arready	: in std_logic;
		m_axi_rid	: in std_logic_vector(C_M_AXI_ID_WIDTH-1 downto 0);
		m_axi_rdata	: in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		m_axi_rresp	: in std_logic_vector(1 downto 0);
		m_axi_rlast	: in std_logic;
		m_axi_rvalid	: in std_logic;
		m_axi_rready	: out std_logic;

		-- Ports of Axi Slave Bus Interface S_AXI
		s_axi_aclk	: in std_logic;
		s_axi_aresetn	: in std_logic;
		s_axi_awaddr	: in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
		s_axi_awprot	: in std_logic_vector(2 downto 0);
		s_axi_awvalid	: in std_logic;
		s_axi_awready	: out std_logic;
		s_axi_wdata	: in std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		s_axi_wstrb	: in std_logic_vector((C_S_AXI_DATA_WIDTH/8)-1 downto 0);
		s_axi_wvalid	: in std_logic;
		s_axi_wready	: out std_logic;
		s_axi_bresp	: out std_logic_vector(1 downto 0);
		s_axi_bvalid	: out std_logic;
		s_axi_bready	: in std_logic;
		s_axi_araddr	: in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
		s_axi_arprot	: in std_logic_vector(2 downto 0);
		s_axi_arvalid	: in std_logic;
		s_axi_arready	: out std_logic;
		s_axi_rdata	: out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		s_axi_rresp	: out std_logic_vector(1 downto 0);
		s_axi_rvalid	: out std_logic;
		s_axi_rready	: in std_logic
	);
end GPU;

architecture arch_imp of GPU is
        
        signal reg0 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg1 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg2 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg3 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg4 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg5 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg6 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg7 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg8 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg9 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg10 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg11 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg12 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg13 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg14 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg15 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg16 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg17 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		signal reg18 : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	
	   signal status : std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);

	-- component declaration
	component GPU_M_AXI is
		generic (
		C_M_TARGET_SLAVE_BASE_ADDR	: std_logic_vector	:= x"40000000";
		C_M_AXI_BURST_LEN	: integer	:= 16;
		C_M_AXI_ID_WIDTH	: integer	:= 1;
		C_M_AXI_ADDR_WIDTH	: integer	:= 32;
		C_M_AXI_DATA_WIDTH	: integer	:= 32
		);
		port (
		M_AXI_ACLK	: in std_logic;
		M_AXI_ARESETN	: in std_logic;
		M_AXI_AWID	: out std_logic_vector(C_M_AXI_ID_WIDTH-1 downto 0);
		M_AXI_AWADDR	: out std_logic_vector(C_M_AXI_ADDR_WIDTH-1 downto 0);
		M_AXI_AWLEN	: out std_logic_vector(7 downto 0);
		M_AXI_AWSIZE	: out std_logic_vector(2 downto 0);
		M_AXI_AWBURST	: out std_logic_vector(1 downto 0);
		M_AXI_AWLOCK	: out std_logic_vector(1 downto 0);
		M_AXI_AWCACHE	: out std_logic_vector(3 downto 0);
		M_AXI_AWPROT	: out std_logic_vector(2 downto 0);
		M_AXI_AWQOS	: out std_logic_vector(3 downto 0);
		M_AXI_AWVALID	: out std_logic;
		M_AXI_AWREADY	: in std_logic;
		M_AXI_WID   :   out std_logic_vector(5 downto 0);
		M_AXI_WDATA	: out std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		M_AXI_WSTRB	: out std_logic_vector(C_M_AXI_DATA_WIDTH/8-1 downto 0);
		M_AXI_WLAST	: out std_logic;
		M_AXI_WVALID	: out std_logic;
		M_AXI_WREADY	: in std_logic;
		M_AXI_BID	: in std_logic_vector(C_M_AXI_ID_WIDTH-1 downto 0);
		M_AXI_BRESP	: in std_logic_vector(1 downto 0);
		M_AXI_BVALID	: in std_logic;
		M_AXI_BREADY	: out std_logic;
		M_AXI_ARID	: out std_logic_vector(C_M_AXI_ID_WIDTH-1 downto 0);
		M_AXI_ARADDR	: out std_logic_vector(C_M_AXI_ADDR_WIDTH-1 downto 0);
		M_AXI_ARLEN	: out std_logic_vector(7 downto 0);
		M_AXI_ARSIZE	: out std_logic_vector(2 downto 0);
		M_AXI_ARBURST	: out std_logic_vector(1 downto 0);
		M_AXI_ARLOCK	: out std_logic_vector(1 downto 0);
		M_AXI_ARCACHE	: out std_logic_vector(3 downto 0);
		M_AXI_ARPROT	: out std_logic_vector(2 downto 0);
		M_AXI_ARQOS	: out std_logic_vector(3 downto 0);
		M_AXI_ARVALID	: out std_logic;
		M_AXI_ARREADY	: in std_logic;
		M_AXI_RID	: in std_logic_vector(C_M_AXI_ID_WIDTH-1 downto 0);
		M_AXI_RDATA	: in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		M_AXI_RRESP	: in std_logic_vector(1 downto 0);
		M_AXI_RLAST	: in std_logic;
		M_AXI_RVALID	: in std_logic;
		M_AXI_RREADY	: out std_logic;
		
		erase_x1 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		erase_x2 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		erase_x3 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		erase_x4 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		
		erase_y1 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		erase_y2 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		erase_y3 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		erase_y4 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		
		draw_x1 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		draw_x2 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		draw_x3 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		draw_x4 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		
		draw_y1 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		draw_y2 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		draw_y3 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		draw_y4 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		
		extra_reg0 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		extra_reg1 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		extra_reg2 : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		
		status : in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0)
		);
	end component GPU_M_AXI;

	component GPU_S_AXI is
		generic (
		C_S_AXI_DATA_WIDTH	: integer	:= 32;
		C_S_AXI_ADDR_WIDTH	: integer	:= 4
		);
		port (
		S_AXI_ACLK	: in std_logic;
		S_AXI_ARESETN	: in std_logic;
		S_AXI_AWADDR	: in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
		S_AXI_AWPROT	: in std_logic_vector(2 downto 0);
		S_AXI_AWVALID	: in std_logic;
		S_AXI_AWREADY	: out std_logic;
		S_AXI_WDATA	: in std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		S_AXI_WSTRB	: in std_logic_vector((C_S_AXI_DATA_WIDTH/8)-1 downto 0);
		S_AXI_WVALID	: in std_logic;
		S_AXI_WREADY	: out std_logic;
		S_AXI_BRESP	: out std_logic_vector(1 downto 0);
		S_AXI_BVALID	: out std_logic;
		S_AXI_BREADY	: in std_logic;
		S_AXI_ARADDR	: in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
		S_AXI_ARPROT	: in std_logic_vector(2 downto 0);
		S_AXI_ARVALID	: in std_logic;
		S_AXI_ARREADY	: out std_logic;
		S_AXI_RDATA	: out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		S_AXI_RRESP	: out std_logic_vector(1 downto 0);
		S_AXI_RVALID	: out std_logic;
		S_AXI_RREADY	: in std_logic;
		
		erase_x1 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		erase_x2 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		erase_x3 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		erase_x4 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		
		erase_y1 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		erase_y2 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		erase_y3 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		erase_y4 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		
		draw_x1 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		draw_x2 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		draw_x3 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		draw_x4 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		
		draw_y1 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		draw_y2 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		draw_y3 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		draw_y4 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		
		extra_reg0 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		extra_reg1 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		extra_reg2 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		
		status : out std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0)
		);
	end component GPU_S_AXI;

begin

-- Instantiation of Axi Bus Interface M_AXI
GPU_M_AXI_inst : GPU_M_AXI
	generic map (
		C_M_TARGET_SLAVE_BASE_ADDR	=> C_M_AXI_TARGET_SLAVE_BASE_ADDR,
		C_M_AXI_BURST_LEN	=> C_M_AXI_BURST_LEN,
		C_M_AXI_ID_WIDTH	=> C_M_AXI_ID_WIDTH,
		C_M_AXI_ADDR_WIDTH	=> C_M_AXI_ADDR_WIDTH,
		C_M_AXI_DATA_WIDTH	=> C_M_AXI_DATA_WIDTH
	)
	port map (
		M_AXI_ACLK	=> m_axi_aclk,
		M_AXI_ARESETN	=> m_axi_aresetn,
		M_AXI_AWID	=> m_axi_awid,
		M_AXI_AWADDR	=> m_axi_awaddr,
		M_AXI_AWLEN	=> m_axi_awlen,
		M_AXI_AWSIZE	=> m_axi_awsize,
		M_AXI_AWBURST	=> m_axi_awburst,
		M_AXI_AWLOCK	=> m_axi_awlock,
		M_AXI_AWCACHE	=> m_axi_awcache,
		M_AXI_AWPROT	=> m_axi_awprot,
		M_AXI_AWQOS	=> m_axi_awqos,
		M_AXI_AWVALID	=> m_axi_awvalid,
		M_AXI_AWREADY	=> m_axi_awready,
		M_AXI_WID => m_axi_wid,
		M_AXI_WDATA	=> m_axi_wdata,
		M_AXI_WSTRB	=> m_axi_wstrb,
		M_AXI_WLAST	=> m_axi_wlast,
		M_AXI_WVALID	=> m_axi_wvalid,
		M_AXI_WREADY	=> m_axi_wready,
		M_AXI_BID	=> m_axi_bid,
		M_AXI_BRESP	=> m_axi_bresp,
		M_AXI_BVALID	=> m_axi_bvalid,
		M_AXI_BREADY	=> m_axi_bready,
		M_AXI_ARID	=> m_axi_arid,
		M_AXI_ARADDR	=> m_axi_araddr,
		M_AXI_ARLEN	=> m_axi_arlen,
		M_AXI_ARSIZE	=> m_axi_arsize,
		M_AXI_ARBURST	=> m_axi_arburst,
		M_AXI_ARLOCK	=> m_axi_arlock,
		M_AXI_ARCACHE	=> m_axi_arcache,
		M_AXI_ARPROT	=> m_axi_arprot,
		M_AXI_ARQOS	=> m_axi_arqos,
		M_AXI_ARVALID	=> m_axi_arvalid,
		M_AXI_ARREADY	=> m_axi_arready,
		M_AXI_RID	=> m_axi_rid,
		M_AXI_RDATA	=> m_axi_rdata,
		M_AXI_RRESP	=> m_axi_rresp,
		M_AXI_RLAST	=> m_axi_rlast,
		M_AXI_RVALID	=> m_axi_rvalid,
		M_AXI_RREADY	=> m_axi_rready,
		
		erase_x1 => reg0,
		erase_x2 => reg1,
		erase_x3 => reg2,
		erase_x4 => reg3,
		
        erase_y1 => reg4,
		erase_y2 => reg5,
		erase_y3 => reg6,
		erase_y4 => reg7,
		
		draw_x1 => reg8,
		draw_x2 => reg9,
		draw_x3 => reg10,
		draw_x4 => reg11,
		
		draw_y1 => reg12,
		draw_y2 => reg13,
		draw_y3 => reg14,
		draw_y4 => reg15,
		
		extra_reg0 => reg16,
		extra_reg1 => reg17,
		extra_reg2 => reg18,

        status => status
	);

-- Instantiation of Axi Bus Interface S_AXI
GPU_S_AXI_inst : GPU_S_AXI
	generic map (
		C_S_AXI_DATA_WIDTH	=> C_S_AXI_DATA_WIDTH,
		C_S_AXI_ADDR_WIDTH	=> C_S_AXI_ADDR_WIDTH
	)
	port map (
		S_AXI_ACLK	=> s_axi_aclk,
		S_AXI_ARESETN	=> s_axi_aresetn,
		S_AXI_AWADDR	=> s_axi_awaddr,
		S_AXI_AWPROT	=> s_axi_awprot,
		S_AXI_AWVALID	=> s_axi_awvalid,
		S_AXI_AWREADY	=> s_axi_awready,
		S_AXI_WDATA	=> s_axi_wdata,
		S_AXI_WSTRB	=> s_axi_wstrb,
		S_AXI_WVALID	=> s_axi_wvalid,
		S_AXI_WREADY	=> s_axi_wready,
		S_AXI_BRESP	=> s_axi_bresp,
		S_AXI_BVALID	=> s_axi_bvalid,
		S_AXI_BREADY	=> s_axi_bready,
		S_AXI_ARADDR	=> s_axi_araddr,
		S_AXI_ARPROT	=> s_axi_arprot,
		S_AXI_ARVALID	=> s_axi_arvalid,
		S_AXI_ARREADY	=> s_axi_arready,
		S_AXI_RDATA	=> s_axi_rdata,
		S_AXI_RRESP	=> s_axi_rresp,
		S_AXI_RVALID	=> s_axi_rvalid,
		S_AXI_RREADY	=> s_axi_rready,
		
		erase_x1 => reg0,
		erase_x2 => reg1,
		erase_x3 => reg2,
		erase_x4 => reg3,
		
        erase_y1 => reg4,
		erase_y2 => reg5,
		erase_y3 => reg6,
		erase_y4 => reg7,
		
		draw_x1 => reg8,
		draw_x2 => reg9,
		draw_x3 => reg10,
		draw_x4 => reg11,
		
		draw_y1 => reg12,
		draw_y2 => reg13,
		draw_y3 => reg14,
		draw_y4 => reg15,
		
		extra_reg0 => reg16,
		extra_reg1 => reg17,
		extra_reg2 => reg18,
		
		status => status
	);

	-- Add user logic here

	-- User logic ends

end arch_imp;
