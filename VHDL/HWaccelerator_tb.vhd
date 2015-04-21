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
         clk : IN  std_logic;
         Exists : IN  std_logic;
         DataIn : IN  std_logic_vector(31 downto 0);
         FSL_read : OUT  std_logic;
         FSL_write : OUT  std_logic;
         DataOut : OUT  std_logic_vector(31 downto 0)
        );
    END COMPONENT;


   --Inputs
   signal clk : std_logic := '0';
   signal Exists : std_logic := '0';
   signal DataIn : std_logic_vector(31 downto 0) := (others => '0');

 	--Outputs
   signal FSL_read : std_logic;
   signal FSL_write : std_logic;
   signal DataOut : std_logic_vector(31 downto 0);

   -- Clock period definitions
   constant clk_period : time := 30 ns;

BEGIN

	-- Instantiate the Unit Under Test (UUT)
   uut: HWaccelerator PORT MAP (
          clk => clk,
          Exists => Exists,
          DataIn => DataIn,
          FSL_read => FSL_read,
          FSL_write => FSL_write,
          DataOut => DataOut
        );

   -- Clock process definitions
   clk_process :process
   begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
   end process;


   -- Stimulus process
   stim_proc: process
   begin
      -- hold reset state for 100 ns.
      wait for clk_period*6;
      -- insert stimulus here

      Exists <= '1';
      DataIn <= X"328831E0";
		wait for clk_period;
      DataIn <= X"435A3137";
		wait for clk_period;
      DataIn <= X"F6309807";
		wait for clk_period;
      DataIn <= X"A88DA234";
		wait for clk_period;
      Exists <= '0';

      wait;
   end process;

END;
