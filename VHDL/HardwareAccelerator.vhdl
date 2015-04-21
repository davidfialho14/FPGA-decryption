library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Does part of computation for the AES Inv Mix Columns step (one column)

entity HWaccelerator is
    Port ( 	clk				: in 	STD_LOGIC;
       		Exists			: in 	STD_LOGIC;
       		DataIn 			: in 	STD_LOGIC_VECTOR (31 downto 0); -- The Matrix should 
    		FSL_read 		: out 	STD_LOGIC;
    		FSL_write		: out 	STD_LOGIC;
           	DataOut			: out 	STD_LOGIC_VECTOR (31 downto 0)
         );
end HWaccelerator;

architecture Behavioral of HWaccelerator is
	constant DelayAmmount : Integer := 4+1; -- cycles + 1

	signal Input_clk, Output_clk					: STD_LOGIC := '0';
	signal DataOutSelect							: STD_LOGIC_VECTOR(1 downto 0) := (others => '0');
	signal Rx0, Rx1, Rx2, Rx3 						: STD_LOGIC_VECTOR(31 downto 0) := (others => '0');
	signal Ry0, Ry1, Ry2, Ry3 						: STD_LOGIC_VECTOR(31 downto 0) := (others => '0');
	signal OutReg0, OutReg1, OutReg2, OutReg3 		: STD_LOGIC_VECTOR(31 downto 0) := (others => '0');

	-- registos de atraso do contador --
	type  delayArray is array (0 to DelayAmmount) of std_logic; 
	signal counterDelay : delayArray;

	component MixColumns is
    Port ( ROWIN0, ROWIN1, ROWIN2, ROWIN3 		: in 	STD_LOGIC_VECTOR (31 downto 0);
           ROWOUT0, ROWOUT1, ROWOUT2, ROWOUT3 	: out 	STD_LOGIC_VECTOR (31 downto 0)
         );
	end component;

begin

FSL_read <= Exists; -- Reads as soon as it shows up
Input_clk <= clk and Exists;

-- The output clk is the input clk delayd by DelayAmmount cycles
process(clk)
begin
	if clk'event and clk='1' then
		counterDelay(0) <= Input_clk;
	end if;
end process;

delay: for k in 0 to DelayAmmount-1 generate
begin
	process(clk)
	begin
		if clk'event and clk='1' then
			counterDelay(k+1) <= counterDelay(k);
		end if;
	end process;
end generate delay;


Output_clk <= counterDelay(DelayAmmount-3); -- D(3)

-- D(0) or (D(1) and not D(2))
DataOutSelect(0) <= counterDelay(DelayAmmount-0) or (counterDelay(DelayAmmount-2) and not counterDelay(DelayAmmount-1)); 
DataOutSelect(1) <= counterDelay(DelayAmmount-1); -- D(1)

-- In Register Row 3
process(Input_clk)
	begin
		if Input_clk'event and Input_clk = '1' then
			Rx3 <= DataIn;
		end if;
	end process;

-- In Register Row 2
process(Input_clk)
	begin
		if Input_clk'event and Input_clk = '1' then
			Rx2 <= Rx3;
		end if;
	end process;

-- In Register Row 1
process(Input_clk)
	begin
		if Input_clk'event and Input_clk = '1' then
			Rx1 <= Rx2;
		end if;
	end process;

-- In Register Row 0
process(Input_clk)
	begin
		if Input_clk'event and Input_clk = '1' then
			Rx0 <= Rx1;
		end if;
	end process;

-- Instantiations --
	MixAllColumns : MixColumns port map 
	(   
		ROWIN3 	=> Rx3,
		ROWIN2 	=> Rx2,
		ROWIN1 	=> Rx1,
		ROWIN0 	=> Rx0,

		ROWOUT3	=> Ry3,
		ROWOUT2	=> Ry2,
		ROWOUT1	=> Ry1,
		ROWOUT0	=> Ry0
	);


-- Out Register Row 3 -- Será que pode ser removido? 
process(Output_clk)
	begin
		if Output_clk'event and Output_clk = '1' then
			OutReg3 <= Ry3;
		end if;
	end process;

-- Out Register Row 2
process(Output_clk)
	begin
		if Output_clk'event and Output_clk = '1' then
			OutReg2 <= Ry2;
		end if;
	end process;

-- Out Register Row 1
process(Output_clk)
	begin
		if Output_clk'event and Output_clk = '1' then
			OutReg1 <= Ry1;
		end if;
	end process;

-- Out Register Row 0
process(Output_clk)
	begin
		if Output_clk'event and Output_clk = '1' then
			OutReg0 <= Ry0;
		end if;
	end process;

with DataOutSelect select
	DataOut <=	OutReg0 when "00", 
				OutReg1 when "01",
				OutReg2 when "10",
				OutReg3 when "11",
				X"00000000"	when others;

end Behavioral;