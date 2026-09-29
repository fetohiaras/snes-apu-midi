`timescale 1ns/1ps

// Adaptador teclado USB HID -> mesma interface de eventos do parser MIDI
// (ev_valid/ev_status/ev_data1/ev_data2).
//
// Estrategia de oitava: o codigo de uso HID (USB HID Usage Tables, pagina
// 0x07) NAO numera as teclas por posicao fisica -- letras sao alfabeticas
// (a=0x04 .. z=0x1D), entao tratar a faixa de usages como uma escala
// cromatica continua embaralha o teclado. Em vez disso, cada FILEIRA
// fisica (numeros, QWERTY, ASDF, ZXCV) e mapeada explicitamente por
// posicao de coluna e vira uma oitava distinta (NOTE_BASE + fileira*12 +
// coluna), como um teclado musical por linhas.
//
// Le os relatorios do nucleo usb_hid_host (dominio clk_usb) e atravessa
// para o dominio dclk com um handshake de toggle: o dado so e capturado no
// lado dclk apos a borda do toggle ser sincronizada, garantindo que os 6
// bytes cruzem como um conjunto estavel (nunca uma mistura de bytes
// antigos/novos de fileiras diferentes).
module usb_hid_note_bridge #(
    parameter [6:0] VELOCITY  = 7'd100,
    parameter [6:0] NOTE_BASE = 7'd36
) (
    input  wire       clk_usb,
    input  wire       resetn_usb,
    input  wire [1:0] typ,
    input  wire       full_report,
    input  wire [7:0] key_0, key_1, key_2, key_3, key_4, key_5,

    input  wire        clk,
    input  wire        resetn,

    output reg         ev_valid,
    output reg  [7:0]  ev_status,
    output reg  [6:0]  ev_data1,
    output reg  [6:0]  ev_data2
);

    // ------------------------------------------------------------------
    // Codigo de uso HID -> {valido, fileira[1:0], coluna[3:0]}
    // Fileira 0 = ZXCV (mais grave) .. fileira 3 = numeros (mais aguda).
    // Ordem das colunas e a ordem FISICA da tecla na fileira, nao o valor
    // numerico do usage.
    // ------------------------------------------------------------------
    function [6:0] hid_to_group;  // bit6=valido, [5:4]=fileira, [3:0]=coluna
        input [7:0] usage;
        reg   [1:0] row;
        reg   [3:0] col;
        reg         found;
        begin
            found = 1'b1;
            row   = 2'd0;
            col   = 4'd0;
            case (usage)
                8'h1D: begin row = 2'd0; col = 4'd0;  end // Z
                8'h1B: begin row = 2'd0; col = 4'd1;  end // X
                8'h06: begin row = 2'd0; col = 4'd2;  end // C
                8'h19: begin row = 2'd0; col = 4'd3;  end // V
                8'h05: begin row = 2'd0; col = 4'd4;  end // B
                8'h11: begin row = 2'd0; col = 4'd5;  end // N
                8'h10: begin row = 2'd0; col = 4'd6;  end // M
                8'h36: begin row = 2'd0; col = 4'd7;  end // ,
                8'h37: begin row = 2'd0; col = 4'd8;  end // .
                8'h38: begin row = 2'd0; col = 4'd9;  end // /

                8'h04: begin row = 2'd1; col = 4'd0;  end // A
                8'h16: begin row = 2'd1; col = 4'd1;  end // S
                8'h07: begin row = 2'd1; col = 4'd2;  end // D
                8'h09: begin row = 2'd1; col = 4'd3;  end // F
                8'h0A: begin row = 2'd1; col = 4'd4;  end // G
                8'h0B: begin row = 2'd1; col = 4'd5;  end // H
                8'h0D: begin row = 2'd1; col = 4'd6;  end // J
                8'h0E: begin row = 2'd1; col = 4'd7;  end // K
                8'h0F: begin row = 2'd1; col = 4'd8;  end // L
                8'h33: begin row = 2'd1; col = 4'd9;  end // ;
                8'h34: begin row = 2'd1; col = 4'd10; end // '

                8'h14: begin row = 2'd2; col = 4'd0;  end // Q
                8'h1A: begin row = 2'd2; col = 4'd1;  end // W
                8'h08: begin row = 2'd2; col = 4'd2;  end // E
                8'h15: begin row = 2'd2; col = 4'd3;  end // R
                8'h17: begin row = 2'd2; col = 4'd4;  end // T
                8'h1C: begin row = 2'd2; col = 4'd5;  end // Y
                8'h18: begin row = 2'd2; col = 4'd6;  end // U
                8'h0C: begin row = 2'd2; col = 4'd7;  end // I
                8'h12: begin row = 2'd2; col = 4'd8;  end // O
                8'h13: begin row = 2'd2; col = 4'd9;  end // P

                8'h1E: begin row = 2'd3; col = 4'd0;  end // 1
                8'h1F: begin row = 2'd3; col = 4'd1;  end // 2
                8'h20: begin row = 2'd3; col = 4'd2;  end // 3
                8'h21: begin row = 2'd3; col = 4'd3;  end // 4
                8'h22: begin row = 2'd3; col = 4'd4;  end // 5
                8'h23: begin row = 2'd3; col = 4'd5;  end // 6
                8'h24: begin row = 2'd3; col = 4'd6;  end // 7
                8'h25: begin row = 2'd3; col = 4'd7;  end // 8
                8'h26: begin row = 2'd3; col = 4'd8;  end // 9
                8'h27: begin row = 2'd3; col = 4'd9;  end // 0
                default: found = 1'b0;
            endcase
            hid_to_group = {found, row, col};
        end
    endfunction

    localparam [5:0] NUM_COLS  = 6'd11;
    localparam [5:0] NUM_NOTES = 6'd44;   // 4 fileiras * 11 colunas, indexadas fileira*11+coluna

    // ------------------------------------------------------------------
    // Captura do relatorio no dominio clk_usb + pulso de toggle atrasado
    // um ciclo em relacao ao dado, para que o handshake so sinalize a
    // travessia depois que os 6 bytes ja estejam estaveis.
    // ------------------------------------------------------------------
    reg [7:0] keys_latched [0:5];
    reg       report_toggle;
    integer   li;

    always @(posedge clk_usb) begin
        if (!resetn_usb) begin
            for (li = 0; li < 6; li = li + 1)
                keys_latched[li] <= 8'h00;
            report_toggle <= 1'b0;
        end else begin
            if (full_report) begin
                if (typ == 2'd1) begin
                    keys_latched[0] <= key_0;
                    keys_latched[1] <= key_1;
                    keys_latched[2] <= key_2;
                    keys_latched[3] <= key_3;
                    keys_latched[4] <= key_4;
                    keys_latched[5] <= key_5;
                end else begin
                    for (li = 0; li < 6; li = li + 1)
                        keys_latched[li] <= 8'h00;  // dispositivo trocou de tipo: solta tudo
                end
            end
            report_toggle <= report_toggle ^ full_report;
        end
    end

    // ------------------------------------------------------------------
    // Travessia para o dominio dclk: sincronizador de toggle (1 bit, seguro)
    // + captura dos 6 bytes somente na borda sincronizada, quando ja se
    // sabe que keys_latched esta parado ha pelo menos um ciclo de clk_usb.
    // ------------------------------------------------------------------
    reg [2:0] toggle_sync;
    reg [7:0] keys_stable [0:5];
    integer   si;

    wire toggle_edge = toggle_sync[2] ^ toggle_sync[1];

    always @(posedge clk) begin
        if (!resetn) begin
            toggle_sync <= 3'b000;
            for (si = 0; si < 6; si = si + 1)
                keys_stable[si] <= 8'h00;
        end else begin
            toggle_sync <= {toggle_sync[1:0], report_toggle};
            if (toggle_edge) begin
                for (si = 0; si < 6; si = si + 1)
                    keys_stable[si] <= keys_latched[si];
            end
        end
    end

    // ------------------------------------------------------------------
    // Bitmap combinacional das notas atualmente pressionadas
    // ------------------------------------------------------------------
    reg [NUM_NOTES-1:0] cur_bits;
    integer ki;
    reg [6:0] grp;

    always @* begin
        cur_bits = {NUM_NOTES{1'b0}};
        for (ki = 0; ki < 6; ki = ki + 1) begin
            grp = hid_to_group(keys_stable[ki]);
            if (grp[6])
                cur_bits[grp[5:4] * NUM_COLS + {2'b00, grp[3:0]}] = 1'b1;
        end
    end

    // ------------------------------------------------------------------
    // Varredor continuo: 1 indice por ciclo de dclk (44 ciclos por volta),
    // emite Note On/Off a cada mudanca de bit.
    // ------------------------------------------------------------------
    localparam [5:0] WALK_LAST = NUM_NOTES - 6'd1;

    reg [NUM_NOTES-1:0] prev_bits;
    reg [5:0]           walk_idx;

    always @(posedge clk) begin
        ev_valid <= 1'b0;

        if (!resetn) begin
            prev_bits <= {NUM_NOTES{1'b0}};
            walk_idx  <= 6'd0;
            ev_status <= 8'h00;
            ev_data1  <= 7'd0;
            ev_data2  <= 7'd0;
        end else begin
            if (cur_bits[walk_idx] != prev_bits[walk_idx]) begin
                ev_valid            <= 1'b1;
                ev_status           <= cur_bits[walk_idx] ? 8'h90 : 8'h80;
                ev_data1            <= walk_idx + NOTE_BASE;
                ev_data2            <= cur_bits[walk_idx] ? VELOCITY : 7'd0;
                prev_bits[walk_idx] <= cur_bits[walk_idx];
            end

            walk_idx <= (walk_idx == WALK_LAST) ? 6'd0 : walk_idx + 6'd1;
        end
    end

endmodule
