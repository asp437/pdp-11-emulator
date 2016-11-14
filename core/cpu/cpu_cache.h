//
// Created by Aleksandr Parfenov on 4.11.16.
//

#ifndef PDP_11_EMULATOR_CPU_CACHE_H
#define PDP_11_EMULATOR_CPU_CACHE_H

#include <vector>
#include "../../common.h"
#include "../unibus/unibus.h"

const uint CACHE_LINE_SIZE = 16; // In bytes
const uint CACHE_SIZE = 16; // In lines
const uint CACHE_HIT_TIME = 1;
const uint MEMORY_ACCESS_TIME = 5;

typedef struct _CACHE_LINE {
    uint8 data[CACHE_LINE_SIZE];
    uint18 base;
    uint32 time_stamp;
    bool dirty;
} CacheLine;

class CPUCache {
public:
    CPUCache();
    ~CPUCache();

    virtual void register_unibus(Unibus *unibus) { this->_unibus = unibus; };
    void set_enabled(bool enabled) { _enabled = enabled; }

    uint write_word(uint18 address, uint16 value);
    pair<uint16, uint> read_word(uint18 address);
    uint write_byte(uint18 address, uint8 value);
    pair<uint8, uint> read_byte(uint18 address);
private:
    bool is_cached_address(uint18 address);

    /**
     * Reads line, which contains specified address. Returns the index of readed line in cache memory.
     */
    int read_line(uint18 address);
    void write_back_line(int line_index);
    uint18 calculate_base(uint18 address) { return address - (address % CACHE_LINE_SIZE); }
    uint18 calculate_offset(uint18 address) { return address % CACHE_LINE_SIZE; }
    int least_recently_used();
    int find_in_cache(uint18 address);
    void write_into_line_word(int line_index, uint18 offset, uint16 value);
    void write_into_line_byte(int line_index, uint18 offset, uint8 value);
    uint16 read_from_line_word(int line_index, uint18 offset);
    uint8 read_from_line_byte(int line_index, uint18 offset);

    uint _time_counter;
    uint _cache_hits;
    uint _cache_misses;
    Unibus *_unibus;
    vector<CacheLine> _memory;
    bool _enabled;
    vector<pair<uint18, uint18>> _cached_segments;
};

#endif //PDP_11_EMULATOR_CPU_CACHE_H
