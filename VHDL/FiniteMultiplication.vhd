library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity FiniteMultiplication is
    Port ( Input : in  STD_LOGIC_VECTOR(7 downto 0);
			Multiplicant : in STD_LOGIC(1 downto 0);
         Output : out  STD_LOGIC_VECTOR(7 downto 0));
end FiniteMultiplication;

architecture Behavioral of FiniteMultiplication is

	signal first, second, third : STD_LOGIC_VECTOR(7 downto 0);
	signal first_power, second_power : STD_LOGIC_VECTOR(1 downto 0);
	
	component MultiplyBy2 is
    Port ( Input 	: in 		STD_LOGIC_VECTOR (7 downto 0);
				Power : in STD_LOGIC_VECTOR(1 downto 0);
           Output	: out 	STD_LOGIC_VECTOR (7 downto 0)
         );
	end component;

begin

with Multiplicant select
first_power <= "11" when "00", -- 09
					"10" when "01", -- 0B
					"01" when "10", -- 0D
					"01" when "11", -- 0E
					"00" when others;
					
second_power <= "01" when "01", -- 0B
					"10" when "10", -- 0D
					"01" when "11", -- 0E
					"00" when others;


-- 09
firstStage : FiniteMultiplication port map 
		(Input 	=> Input,
		 Power => first_power,
    	 Output => first);
		 
		 X8 xor Input;
		 
-- 0B
firstStage : FiniteMultiplication port map 
		(Input => Input,
		 Power => first_power,
    	 Output => first);
		 
secondStage : FiniteMultiplication port map 
		(Input 	=> first xor Input,
		 Power => second_power,
    	 Output => second);
		 
		 second xor Input;

-- 0D		 
firstStage : FiniteMultiplication port map 
		(Input => Input,
		 Power => first_power,
    	 Output => first);
		 
secondStage : FiniteMultiplication port map 
		(Input 	=> first xor Input,
		 Power => second_power,
    	 Output => second);
		 
		 second xor Input;
		 
-- 0E
firstStage : FiniteMultiplication port map 
		(Input => Input,
		 Power => first_power,
    	 Output => first);
		 
secondStage : FiniteMultiplication port map 
		(Input 	=> first xor Input,
		 Power => second_power,
    	 Output => second);
		 
		 second xor Input;

thirdStage : FiniteMultiplication port map 
		(Input 	=> second xor Input,
		 Power => 1,
    	 Output => third);
		 
		 third;

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
				 
with Multiplicant select
Output <= X2 when "00", -- 09
			X4 when "01", -- 0B
			X8 when "10", -- 0D
			Input when "11", --0E
			X"00" when others;

end Behavioral;

