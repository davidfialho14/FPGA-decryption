LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY TestColumn_tb IS
END TestColumn_tb;
 
ARCHITECTURE behavior OF TestColumn_tb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT CalculateColumn
    PORT(
         COLUMN_IN : IN  std_logic_vector(31 downto 0);
         COLUMN_OUT : OUT  std_logic_vector(31 downto 0)
        );
    END COMPONENT;
    

   --Inputs
   signal COLUMN_IN : std_logic_vector(31 downto 0) := (others => '0');

 	--Outputs
   signal COLUMN_OUT : std_logic_vector(31 downto 0);
   -- No clocks detected in port list. Replace <clock> below with 
   -- appropriate port name 
 
   constant period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: CalculateColumn PORT MAP (
          COLUMN_IN => COLUMN_IN,
          COLUMN_OUT => COLUMN_OUT
        );

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
      wait for 100 ns;	
		COLUMN_IN <= X"3243F6A8";

      -- insert stimulus here 

      wait;
   end process;

END;
