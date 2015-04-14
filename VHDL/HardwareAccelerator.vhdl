library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Does part of computation for the AES Inv Mix Columns step (one column)

entity HWaccelerator is
    Port ( 	clk				: in 	STD_LOGIC;
    		DataIn 			: in 	STD_LOGIC_VECTOR (31 downto 0); -- The Matrix should 
           	CurrentCPUaddr	: in 	STD_LOGIC_VECTOR (31 downto 0);
           	ResultWriteaddr	: out 	STD_LOGIC_VECTOR (31 downto 0);
           	DataOut			: out 	STD_LOGIC_VECTOR (31 downto 0)
         );
end HWaccelerator;

architecture Behavioral of HWaccelerator is
	signal Rx0, Rx1, Rx2, Rx3 						: STD_LOGIC_VECTOR(31 downto 0);
	signal Ry0, Ry1, Ry2, Ry3 						: STD_LOGIC_VECTOR(31 downto 0);
	signal OutReg0, OutReg1, OutReg2, OutReg3 		: STD_LOGIC_VECTOR(31 downto 0);

	component MixColumns is
    Port ( ROWIN0, ROWIN1, ROWIN2, ROWIN3 		: in 	STD_LOGIC_VECTOR (31 downto 0);
           ROWOUT0, ROWOUT1, ROWOUT2, ROWOUT3 	: out 	STD_LOGIC_VECTOR (31 downto 0)
         );
	end component;

begin

Rx3 <= DataIn;

-- In Register Row 2
process(clk)
	begin
		if clk'event and clk = '1' then
			Rx2 <= DataIn;
		end if;
	end process;

-- In Register Row 1
process(clk)
	begin
		if clk'event and clk = '1' then
			Rx1 <= Rx2;
		end if;
	end process;

-- In Register Row 0
process(clk)
	begin
		if clk'event and clk = '1' then
			Rx0 <= Rx1;
		end if;
	end process;

-- Instantiations --
	MixAllColumns : MixColumns port map 
		(   ROWIN3 	=> Rx3,
			ROWIN2 	=> Rx2,
			ROWIN1 	=> Rx1,
			ROWIN0 	=> Rx0,

			ROWOUT3	=> Ry3,
			ROWOUT2	=> Ry2,
			ROWOUT1	=> Ry1,
			ROWOUT0	=> Ry0);


-- Out Register Row 3 -- Será que pode ser removido? 
process(clk)
	begin
		if clk'event and clk = '1' then
			OutReg3 <= Ry3;
		end if;
	end process;

-- Out Register Row 2
process(clk)
	begin
		if clk'event and clk = '1' then
			OutReg2 <= Ry2;
		end if;
	end process;

-- Out Register Row 1
process(clk)
	begin
		if clk'event and clk = '1' then
			OutReg1 <= Ry1;
		end if;
	end process;

-- Out Register Row 0
process(clk)
	begin
		if clk'event and clk = '1' then
			OutReg0 <= Ry0;
		end if;
	end process;


with CurrentCPUaddr(1 downto 0) select -- CHANGE THIS TO ADJUST THE TIMING
	DataOut <=	OutReg0 when "00", -- CPU is writing to the first address
				OutReg1 when "01",
				OutReg2 when "10",
				OutReg3 when "11",
				X"00000000"	when others;

ResultWriteaddr <= CurrentCPUaddr; -- CHANGE THIS TO ADJUST THE TIMING

end Behavioral;