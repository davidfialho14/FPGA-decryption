library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Multiply is
    Port ( 	X		 			: in  STD_LOGIC_VECTOR(7 downto 0);
				Multiplicant 	: in 	STD_LOGIC_VECTOR(1 downto 0);
				Output 			: out STD_LOGIC_VECTOR(7 downto 0));
end Multiply;

architecture Behavioral of Multiply is

	signal multiplications : STD_LOGIC_VECTOR(23 downto 0);
	signal X2, X4, X8 : STD_LOGIC_VECTOR(7 downto 0);
	
	component PartialMultiply is
    Port ( 	Input 	: in 		STD_LOGIC_VECTOR(7 downto 0);
				Output	: out 	STD_LOGIC_VECTOR(23 downto 0)
         );
	end component;

begin

CalculateMultiplications : PartialMultiply port map 
		(Input 	=> X,
    	 Output 	=> multiplications);

X2 <= multiplications(7 downto 0);
X4 <= multiplications(15 downto 8);
X8 <= multiplications(23 downto 16);
		 
with Multiplicant select
Output <= 	X8 xor X 				when "00", -- 09
				X8 xor X2 xor X 		when "01", -- 0B
				X8 xor X4 xor X		when "10", -- 0D
				X8 xor X4 xor X2		when "11", -- 0E
				X"00" when others;

end Behavioral;

