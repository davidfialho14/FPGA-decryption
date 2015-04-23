--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   12:28:10 04/23/2015
-- Design Name:   
-- Module Name:   /home/david/Development/AES/VHDL/HWaccelerator_tb.vhd
-- Project Name:  AES
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: HWaccelerator
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY HWaccelerator_tb IS
END HWaccelerator_tb;
 
ARCHITECTURE behavior OF HWaccelerator_tb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT HWaccelerator
    PORT(
         FSL_Clk : IN  std_logic;
         FSL_Rst : IN  std_logic;
         FSL_S_Clk : IN  std_logic;
         FSL_S_Read : OUT  std_logic;
         FSL_S_Data : IN  std_logic_vector(31 downto 0);
         FSL_S_Control : IN  std_logic;
         FSL_S_Exists : IN  std_logic;
         FSL_M_Clk : IN  std_logic;
         FSL_M_Write : OUT  std_logic;
         FSL_M_Data : OUT  std_logic_vector(31 downto 0);
         FSL_M_Control : OUT  std_logic;
         FSL_M_Full : IN  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal FSL_Clk : std_logic := '0';
   signal FSL_Rst : std_logic := '0';
   signal FSL_S_Clk : std_logic := '0';
   signal FSL_S_Data : std_logic_vector(31 downto 0) := (others => '0');
   signal FSL_S_Control : std_logic := '0';
   signal FSL_S_Exists : std_logic := '0';
   signal FSL_M_Clk : std_logic := '0';
   signal FSL_M_Full : std_logic := '0';

 	--Outputs
   signal FSL_S_Read : std_logic;
   signal FSL_M_Write : std_logic;
   signal FSL_M_Data : std_logic_vector(31 downto 0);
   signal FSL_M_Control : std_logic;

   -- Clock period definitions
   constant FSL_Clk_period : time := 10 ns;
   constant FSL_S_Clk_period : time := 10 ns;
   constant FSL_M_Clk_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: HWaccelerator PORT MAP (
          FSL_Clk => FSL_Clk,
          FSL_Rst => FSL_Rst,
          FSL_S_Clk => FSL_S_Clk,
          FSL_S_Read => FSL_S_Read,
          FSL_S_Data => FSL_S_Data,
          FSL_S_Control => FSL_S_Control,
          FSL_S_Exists => FSL_S_Exists,
          FSL_M_Clk => FSL_M_Clk,
          FSL_M_Write => FSL_M_Write,
          FSL_M_Data => FSL_M_Data,
          FSL_M_Control => FSL_M_Control,
          FSL_M_Full => FSL_M_Full
        );

   -- Clock process definitions
   FSL_Clk_process :process
   begin
		FSL_Clk <= '0';
		wait for FSL_Clk_period/2;
		FSL_Clk <= '1';
		wait for FSL_Clk_period/2;
   end process;
 
   FSL_S_Clk_process :process
   begin
		FSL_S_Clk <= '0';
		wait for FSL_S_Clk_period/2;
		FSL_S_Clk <= '1';
		wait for FSL_S_Clk_period/2;
   end process;
 
   FSL_M_Clk_process :process
   begin
		FSL_M_Clk <= '0';
		wait for FSL_M_Clk_period/2;
		FSL_M_Clk <= '1';
		wait for FSL_M_Clk_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
      FSL_S_Exists <= '0';
		FSL_S_Control <= '0';
		wait for FSL_Clk_period*6;
      -- insert stimulus here

      FSL_S_Exists <= '1';
      FSL_S_Data <= X"328831E0";
		wait for FSL_Clk_period;
      FSL_S_Data <= X"435A3137";
		wait for FSL_Clk_period;
      FSL_S_Data <= X"F6309807";
		wait for FSL_Clk_period;
      FSL_S_Data <= X"A88DA234";
		FSL_S_Control <= '1';
		wait for FSL_Clk_period;
      FSL_S_Exists <= '0';

      wait;
   end process;

END;
