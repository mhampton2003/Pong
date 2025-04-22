LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY bcd_7segment IS
	PORT(bcd_in : STD_LOGIC_VECTOR (3 downto 0); seven_segment_out : out STD_LOGIC_VECTOR (6 downto 0));
END bcd_7segment;

architecture Behavioral of bcd_7segment is
begin
	process(bcd_in)
	begin
		case bcd_in is
			when "0000" =>
				seven_segment_out <= not "0111111"; --Active High 3F
			when "0001" =>
				seven_segment_out <= not "0000110"; --Active High 06
			when "0010" =>
				seven_segment_out <= not "1011011"; --Active High 5B
			when "0011" =>
				seven_segment_out <= not "1001111"; --Active High 4F
			when "0100" =>
				seven_segment_out <= not "1100110"; --Active High 66
			when "0101" =>
				seven_segment_out <= not "1101101"; --Active High 6D
			when "0110" =>
				seven_segment_out <= not "1111101"; --Active High 7D
			when "0111" =>
				seven_segment_out <= not "0000111"; --Active High 07
			when "1000" =>
				seven_segment_out <= not "1111111"; --Active High 7F
			when "1001" =>
				seven_segment_out <= not "1100111"; --Active High 67
			when others =>
				seven_segment_out <= not "1111111"; --null
		end case;
	end process;
end Behavioral;