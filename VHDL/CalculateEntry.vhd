library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Does part of computation for the AES Inv Mix Columns step (one column)

entity CalculateEntry is
    Port ( 	COL_IN	: in 	STD_LOGIC_VECTOR(31 downto 0);
				LINE		: in 	STD_LOGIC_VECTOR(1 downto 0);
				Y_OUT 	: out STD_LOGIC_VECTOR(7 downto 0)
         );
end CalculateEntry;

architecture Behavioral of CalculateEntry is
	signal x0, x1, x2, x3 		: STD_LOGIC_VECTOR(7 downto 0);
	signal t0, t1, t2, t3		: STD_LOGIC_VECTOR(7 downto 0);
	signal y0, y1, y2, y3 		: STD_LOGIC_VECTOR(7 downto 0);

	component Multiply is
    Port ( 	X		 			: in 	STD_LOGIC_VECTOR(7 downto 0);
				Multiplicant 	: in 	STD_LOGIC_VECTOR(1 downto 0);
				Output			: out STD_LOGIC_VECTOR(7 downto 0)
         );
	end component;

begin

-- ASSUMING LITTLE ENDIAN --
	x0 <= COL_IN(7 downto 0);
	x1 <= COL_IN(15 downto 8);
	x2 <= COL_IN(23 downto 16);
	x3 <= COL_IN(31 downto 24);

with LINE select -- to be multiplied by 09
t0 <= x0 when "00", 
		x3 when "01",
		x2 when "10",
		x1 when "11",
		X"00" when others;
		
with LINE select -- to be multiplied by 0B
t1 <= x2 when "00",
		x1 when "01",
		x0 when "10",
		x3 when "11",
		X"00" when others;

with LINE select -- to be multiplied by 0D
t2 <= x1 when "00",
		x0 when "01",
		x3 when "10",
		x2 when "11",
		X"00" when others;
		
with LINE select -- to be multiplied by 0E
t3 <= x3 when "00",
		x2 when "01",
		x1 when "10",
		x0 when "11",
		X"00" when others;

-- 09 : 00
-- 0B : 01
-- 0D : 10
-- 0E : 11

	mult_09 : Multiply port map 
		(X 	=> t0,
		 Multiplicant => "00",
    	 Output => y0);
		 
	mult_0B : Multiply port map 
		(X 	=> t1,
		 Multiplicant => "01",
    	 Output => y1);
		 
	mult_0D : Multiply port map 
		(X		=> t2,
		 Multiplicant => "10",
    	 Output => y2);
		 
	mult_0E : Multiply port map 
		(X 	=> t3,
		 Multiplicant => "11",
    	 Output => y3);

Y_OUT <= y3 xor y2 xor y1 xor y0;

end Behavioral;