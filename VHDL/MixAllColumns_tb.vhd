--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   18:09:16 04/21/2015
-- Design Name:   
-- Module Name:   C:/Users/public.SCDEEC.000/Downloads/FPGA-decryption-dev/accelerator/MixAllColumns_tb.vhd
-- Project Name:  accelerator
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: MixColumns
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
 
ENTITY MixAllColumns_tb IS
END MixAllColumns_tb;
 
ARCHITECTURE behavior OF MixAllColumns_tb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT MixColumns
    PORT(
         ROWIN0 : IN  std_logic_vector(31 downto 0);
         ROWIN1 : IN  std_logic_vector(31 downto 0);
         ROWIN2 : IN  std_logic_vector(31 downto 0);
         ROWIN3 : IN  std_logic_vector(31 downto 0);
         ROWOUT0 : OUT  std_logic_vector(31 downto 0);
         ROWOUT1 : OUT  std_logic_vector(31 downto 0);
         ROWOUT2 : OUT  std_logic_vector(31 downto 0);
         ROWOUT3 : OUT  std_logic_vector(31 downto 0)
        );
    END COMPONENT;
    

   --Inputs
   signal ROWIN0 : std_logic_vector(31 downto 0) := (others => '0');
   signal ROWIN1 : std_logic_vector(31 downto 0) := (others => '0');
   signal ROWIN2 : std_logic_vector(31 downto 0) := (others => '0');
   signal ROWIN3 : std_logic_vector(31 downto 0) := (others => '0');

 	--Outputs
   signal ROWOUT0 : std_logic_vector(31 downto 0);
   signal ROWOUT1 : std_logic_vector(31 downto 0);
   signal ROWOUT2 : std_logic_vector(31 downto 0);
   signal ROWOUT3 : std_logic_vector(31 downto 0);
   -- No clocks detected in port list. Replace <clock> below with 
   -- appropriate port name 
 
   constant <clock>_period : time := 3 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: MixColumns PORT MAP (
          ROWIN0 => ROWIN0,
          ROWIN1 => ROWIN1,
          ROWIN2 => ROWIN2,
          ROWIN3 => ROWIN3,
          ROWOUT0 => ROWOUT0,
          ROWOUT1 => ROWOUT1,
          ROWOUT2 => ROWOUT2,
          ROWOUT3 => ROWOUT3
        );

   -- Clock process definitions
   <clock>_process :process
   begin
		<clock> <= '0';
		wait for <clock>_period/2;
		<clock> <= '1';
		wait for <clock>_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
      wait for 100 ns;	

     -- wait for <clock>_period*10;

      -- insert stimulus here 

		--ROWIN0 <= X"328831E0";
      --ROWIN1 <= X"435A3137";
      --ROWIN2 <= X"F6309807";
      --ROWIN3 <= X"A88DA234";

      wait;
   end process;

END;
