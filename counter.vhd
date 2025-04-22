library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity score_counter is
    Port (
        clk   : in  std_logic;
        rst   : in  std_logic;
        incr  : in  std_logic;
        ones  : out std_logic_vector(3 downto 0);
        tens  : out std_logic_vector(3 downto 0)
    );
end score_counter;

architecture Behavioral of score_counter is
    signal ones_reg   : std_logic_vector(3 downto 0) := "0000";
    signal tens_reg   : std_logic_vector(3 downto 0) := "0000";
    signal incr_prev  : std_logic := '0';
begin

    process(clk, rst)
    begin
        if rst = '1' then
            ones_reg  <= "0000";
            tens_reg  <= "0000";
            incr_prev <= '0';
        elsif rising_edge(clk) then
            -- Only count if we see a rising edge on incr
            if (incr_prev = '0' and incr = '1') then
                if ones_reg = "1001" then
                    ones_reg <= "0000";
                    if tens_reg = "1001" then
                        tens_reg <= "0000";
                    else
                        tens_reg <= tens_reg + 1;
                    end if;
                else
                    ones_reg <= ones_reg + 1;
                end if;
            end if;
            -- Update previous value of incr
            incr_prev <= incr;
        end if;
    end process;

    ones <= ones_reg;
    tens <= tens_reg;

end Behavioral;
