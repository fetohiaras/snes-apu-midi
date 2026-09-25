// Plays a MIDI event script into midi_mailbox + apu_core and records audio.
//
// Script lines: "<clock> <status> <data1> <data2>" (decimal). <clock> counts
// clocks since the APU started running; the event is presented on the
// mailbox input for the clock edge that follows. Events that share a clock
// are injected on consecutive clocks.
//
// Outputs a stereo WAV and a summary file with the mailbox counters; the
// checks themselves live in check_apu_midi.py.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include "Vtb_apu_midi_top.h"
#include "verilated.h"

struct Event {
    std::uint64_t clock;
    int status, d1, d2;
};

static void put16(std::FILE* f, std::uint16_t v) {
    std::fputc(v & 0xff, f);
    std::fputc(v >> 8, f);
}

static void put32(std::FILE* f, std::uint32_t v) {
    put16(f, static_cast<std::uint16_t>(v));
    put16(f, static_cast<std::uint16_t>(v >> 16));
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    if (argc < 5) {
        std::fprintf(stderr, "usage: %s SCRIPT TOTAL_SAMPLES OUT.wav SUMMARY.txt\n", argv[0]);
        return 2;
    }
    const char* script_path = argv[1];
    const std::uint32_t total = static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 10));

    std::vector<Event> events;
    if (std::FILE* s = std::fopen(script_path, "r")) {
        Event e{};
        while (std::fscanf(s, "%llu %d %d %d", reinterpret_cast<unsigned long long*>(&e.clock), &e.status, &e.d1, &e.d2) == 4)
            events.push_back(e);
        std::fclose(s);
    } else {
        std::perror(script_path);
        return 1;
    }

    std::FILE* wav = std::fopen(argv[3], "wb");
    if (!wav) {
        std::perror(argv[3]);
        return 1;
    }
    // Header is rewritten with the real length at the end.
    auto header = [&](std::uint32_t frames) {
        std::fwrite("RIFF", 1, 4, wav);
        put32(wav, 36 + frames * 4);
        std::fwrite("WAVEfmt ", 1, 8, wav);
        put32(wav, 16);
        put16(wav, 1);
        put16(wav, 2);
        put32(wav, 31938);
        put32(wav, 31938 * 4);
        put16(wav, 4);
        put16(wav, 16);
        std::fwrite("data", 1, 4, wav);
        put32(wav, frames * 4);
    };
    header(0);

    auto* dut = new Vtb_apu_midi_top;
    auto tick = [&] {
        dut->clk = 1; dut->eval();
        dut->clk = 0; dut->eval();
    };

    dut->resetn = 0;
    dut->ev_valid = 0;
    for (int i = 0; i < 16; ++i) tick();
    dut->resetn = 1;

    std::uint32_t frames = 0;
    std::uint64_t run_clk = 0;
    std::size_t next = 0;
    std::uint64_t max_busy_run = 0, busy_run = 0;
    while (frames < total) {
        dut->ev_valid = 0;
        if (next < events.size() && dut->running && events[next].clock <= run_clk) {
            dut->ev_valid = 1;
            dut->ev_status = static_cast<std::uint8_t>(events[next].status);
            dut->ev_data1 = static_cast<std::uint8_t>(events[next].d1);
            dut->ev_data2 = static_cast<std::uint8_t>(events[next].d2);
            ++next;
        }
        tick();
        if (dut->running) ++run_clk;
        busy_run = dut->busy ? busy_run + 1 : 0;
        if (busy_run > max_busy_run) max_busy_run = busy_run;
        if (dut->running && dut->snd_rdy) {
            put16(wav, dut->audio_l);
            put16(wav, dut->audio_r);
            ++frames;
        }
    }

    std::fseek(wav, 0, SEEK_SET);
    header(frames);
    std::fclose(wav);

    std::FILE* sum = std::fopen(argv[4], "w");
    if (!sum) {
        std::perror(argv[4]);
        return 1;
    }
    std::fprintf(sum, "frames %u\ninjected %zu\nsent_count %u\ndropped %u\nack_timeout %u\n"
                      "busy_at_end %u\nmax_busy_clocks %llu\n",
                 frames, next, dut->sent_count, dut->dropped, dut->ack_timeout, dut->busy,
                 static_cast<unsigned long long>(max_busy_run));
    std::fclose(sum);
    std::printf("simulated %u frames, injected %zu events, mailbox sent %u\n", frames, next, dut->sent_count);
    delete dut;
    return 0;
}
