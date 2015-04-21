library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Does part of computation for the AES Inv Mix Columns step (one column)

entity MixOneColumn is
    Port ( COL_IN 	: in 	STD_LOGIC_VECTOR (31 downto 0);
           COL_OUT 	: out 	STD_LOGIC_VECTOR (31 downto 0)
         );
end MixOneColumn;

architecture Behavioral of MixOneColumn is
	signal x0, x1, x2, x3 		: STD_LOGIC_VECTOR(7 downto 0);
	signal mult_09_3				: STD_LOGIC_VECTOR(7 downto 0);
	signal mult_0B_1,mult_0B_2	: STD_LOGIC_VECTOR(7 downto 0);
	signal mult_0D_1,mult_0D_2	: STD_LOGIC_VECTOR(7 downto 0);
	signal mult_0B_1	: STD_LOGIC_VECTOR(7 downto 0);
	signal y0, y1, y2, y3 		: STD_LOGIC_VECTOR(7 downto 0);

	component FiniteMultiplication is
    Port ( Input 	: in 		STD_LOGIC_VECTOR (7 downto 0);
				Multiplicant : in STD_LOGIC_VECTOR(1 downto 0);
           Output	: out 	STD_LOGIC_VECTOR (7 downto 0)
         );
	end component;

begin

-- ASSUMING LITTLE ENDIAN --
	x0 <= COL_IN(7 downto 0);
	x1 <= COL_IN(15 downto 8);
	x2 <= COL_IN(23 downto 16);
	x3 <= COL_IN(31 downto 24);

	mult_09_3 : FiniteMultiplication port map 
		(Input 	=> x0,
		 Power => 3,
    	 Output => x1);

	mult_09_3 : FiniteMultiplication port map 
		(Input 	=> x0,
		 Power => 3,
    	 Output => x1);
		 
	mult_09_3 : FiniteMultiplication port map 
		(Input 	=> x0,
		 Power => 3,
    	 Output => x1);
		 
	mult_09_3 : FiniteMultiplication port map 
		(Input 	=> x0,
		 Power => 3,
    	 Output => x1);		 


	sh1x0 <= (x0(6 downto 0) & "0");
	sh2x0 <= (x0(5 downto 0) & "00");
	sh3x0 <= (x0(4 downto 0) & "000");

	sh1x1 <= (x1(6 downto 0) & "0");
	sh2x1 <= (x1(5 downto 0) & "00");
	sh3x1 <= (x1(4 downto 0) & "000");

	sh1x2 <= (x2(6 downto 0) & "0");
	sh2x2 <= (x2(5 downto 0) & "00");
	sh3x2 <= (x2(4 downto 0) & "000");

	sh1x3 <= (x3(6 downto 0) & "0");
	sh2x3 <= (x3(5 downto 0) & "00");
	sh3x3 <= (x3(4 downto 0) & "000");

-- 09: sh3x0 xor x0					: 0E 0B 0D 09
-- 0B: sh3x0 xor sh1x0 xor x0		: 09 0E 0B 0D
-- 0D: sh3x0 xor sh2x0 xor x0		: 0D 09 0E 0B
-- oE: sh3x0 xor sh2x0 xor sh1x0	: 0B 0D 09 0E

	y0 <= (sh3x0 xor sh2x0 xor sh1x0)	xor (sh3x1 xor sh1x1 xor x1) 	xor  (sh3x2 xor sh2x2 xor x2) 		xor (sh3x3 xor x3);
	y1 <= (sh3x0 xor x0) 				xor (sh3x1 xor sh2x1 xor sh1x1) xor  (sh3x2 xor sh1x2 xor x2) 		xor (sh3x3 xor sh2x2 xor x3);
	y2 <= (sh3x0 xor sh2x0 xor x0) 		xor (sh3x1 xor x1) 				xor  (sh3x2 xor sh2x2 xor sh1x2) 	xor (sh3x3 xor sh1x3 xor x3);
	y3 <= (sh3x0 xor sh1x0 xor x0) 		xor (sh3x1 xor sh2x1 xor x1) 	xor  (sh3x2 xor x2) 				xor (sh3x3 xor sh2x3 xor sh1x3);


-- ASSUMING LITTLE ENDIAN --
	COL_OUT(7 downto 0)		<= y0;
	COL_OUT(15 downto 8)	<= y1;
	COL_OUT(23 downto 16)	<= y2;
	COL_OUT(31 downto 24)	<= y3;

end Behavioral;