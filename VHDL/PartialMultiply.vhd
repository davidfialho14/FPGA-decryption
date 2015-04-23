library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity PartialMultiply is
    Port ( 	Input : in  STD_LOGIC_VECTOR(7 downto 0);
				Output : out  STD_LOGIC_VECTOR(23 downto 0));
end PartialMultiply;

architecture Behavioral of PartialMultiply is

	signal X2, X4, X8 : STD_LOGIC_VECTOR(7 downto 0);

begin

with Input(7) select -- multiply by 2
	X2 <= Input(6 downto 0) & '0' when '0',
			(Input(6 downto 0) & '0') xor X"1B" when '1',
			X"00" when others;
			
with X2(7) select -- multiply by 4
	X4 <= X2(6 downto 0) & '0' when '0',
			(X2(6 downto 0) & '0') xor X"1B" when '1',
			X"00" when others;
				 
with X4(7) select -- multiply by 8
	X8 <= X4(6 downto 0) & '0' when '0',
			(X4(6 downto 0) & '0') xor X"1B" when '1',
			X"00" when others;				 

Output <= 	X8 & X4 & X2;

end Behavioral;

