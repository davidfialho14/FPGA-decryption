library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity CalculateColumn is
    Port ( COLUMN_IN 		: in 	STD_LOGIC_VECTOR (31 downto 0); 
           COLUMN_OUT 		: out STD_LOGIC_VECTOR (31 downto 0)
         );
end CalculateColumn;

architecture Behavioral of CalculateColumn is 
	signal y3, y2, y1, y0 		: STD_LOGIC_VECTOR(7 downto 0);

	component CalculateEntry is
    Port ( 	COL_IN 	: in 	STD_LOGIC_VECTOR (31 downto 0);
				LINE		: in 	STD_LOGIC_VECTOR(1 downto 0);
				Y_OUT 	: out STD_LOGIC_VECTOR(7 downto 0)
         );
	end component;

begin

-- Instantiations --
	Entry0 : CalculateEntry port map 
		(  COL_IN 	=> COLUMN_IN,
			LINE		=> "00",
    	   Y_OUT => y3);

    Entry1 : CalculateEntry port map 
		(  COL_IN 	=> COLUMN_IN,
			LINE		=> "01",
    	   Y_OUT => y2);

    Entry2 : CalculateEntry port map 
		(  COL_IN 	=> COLUMN_IN,
			LINE		=> "10",
    	   Y_OUT => y1);

    Entry3 : CalculateEntry port map 
		(  COL_IN 	=> COLUMN_IN,
			LINE		=> "11",
    	   Y_OUT => y0);
-- End of Instantiations --

COLUMN_OUT <= y3 & y2 & y1 & y0;

end Behavioral;