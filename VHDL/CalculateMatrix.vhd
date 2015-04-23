library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity CalculateMatrix is
    Port ( 	ROWIN0, ROWIN1, ROWIN2, ROWIN3 		: in 	STD_LOGIC_VECTOR (31 downto 0);
				ROWOUT0, ROWOUT1, ROWOUT2, ROWOUT3 	: out 	STD_LOGIC_VECTOR (31 downto 0)
         );
end CalculateMatrix;

architecture Behavioral of CalculateMatrix is
	signal Cx0, Cx1, Cx2, Cx3 		: STD_LOGIC_VECTOR(31 downto 0);
	signal Cy0, Cy1, Cy2, Cy3 		: STD_LOGIC_VECTOR(31 downto 0);

	component CalculateColumn is
    Port ( 	COLUMN_IN	  	: in 	STD_LOGIC_VECTOR(31 downto 0);
				COLUMN_OUT 		: out 	STD_LOGIC_VECTOR(31 downto 0)
         );
	end component;

begin

-- Convert ROWS to COLUMNS
	Cx0 <= ROWIN0(31 downto 24) 	& ROWIN1(31 downto 24) 	& ROWIN2(31 downto 24) 	& ROWIN3(31 downto 24);
	Cx1 <= ROWIN0(23 downto 16) 	& ROWIN1(23 downto 16) 	& ROWIN2(23 downto 16) 	& ROWIN3(23 downto 16);
	Cx2 <= ROWIN0(15 downto 8) 	& ROWIN1(15 downto 8) 	& ROWIN2(15 downto 8) 	& ROWIN3(15 downto 8);
	Cx3 <= ROWIN0(7 downto 0) 		& ROWIN1(7 downto 0) 	& ROWIN2(7 downto 0)		& ROWIN3(7 downto 0);

-- Instantiations --
	Column0 : CalculateColumn port map 
		(  COLUMN_IN	=> Cx0,
    	   COLUMN_OUT 	=> Cy0);

    Column1 : CalculateColumn port map 
		(  COLUMN_IN 	=> Cx1,
    	   COLUMN_OUT 	=> Cy1);

    Column2 : CalculateColumn port map 
		(  COLUMN_IN 	=> Cx2,
    	   COLUMN_OUT 	=> Cy2);

    Column3 : CalculateColumn port map 
		(  COLUMN_IN 	=> Cx3,
    	   COLUMN_OUT 	=> Cy3);
-- End of Instantiations --

-- Convert COLUMNS back to ROWS
	ROWOUT0 <= Cy0(31 downto 24)	& Cy1(31 downto 24)	& Cy2(31 downto 24) 	& Cy3(31 downto 24);
	ROWOUT1 <= Cy0(23 downto 16)	& Cy1(23 downto 16)	& Cy2(23 downto 16) 	& Cy3(23 downto 16);
	ROWOUT2 <= Cy0(15 downto 8)	& Cy1(15 downto 8)	& Cy2(15 downto 8) 	& Cy3(15 downto 8);
	ROWOUT3 <= Cy0(7 downto 0)		& Cy1(7 downto 0)		& Cy2(7 downto 0) 	& Cy3(7 downto 0);

end Behavioral;