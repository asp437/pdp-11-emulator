//
// Created by Aleksandr Parfenov on 4.11.16.
//

#include "cpu_cache.h"
#include <cstring>

CPUCache::CPUCache() {
    CacheLine empty_line;
    empty_line.base = 0000000;
    memset(empty_line.data, 0, sizeof(empty_line.data));
    empty_line.dirty = false;
    empty_line.time_stamp = 0;
    _memory.resize(CACHE_SIZE, empty_line);
    _time_counter = 0;
    _enabled = true;
    _cache_misses = 0;
    _cache_hits = 0;
    _cached_segments.push_back(make_pair(0, 16 * 1024)); // RAM
    _cached_segments.push_back(make_pair(0140000, 0140000 + 8 * 1024)); // ROM
    cout << "CPU Cache initialization complete. Cache size " << CACHE_SIZE << " lines, Line size " << CACHE_LINE_SIZE << " bytes."
         << endl;
}

CPUCache::~CPUCache() {
    cout << "Cache misses: " << _cache_misses << endl;
    cout << "Cache hits: " << _cache_hits << endl;
}

uint CPUCache::write_word(uint18 address, uint16 value) {
    if (is_cached_address(address) && _enabled) {
        uint latency = CACHE_HIT_TIME;
        int line_index = find_in_cache(address);
        if (line_index == -1) {
            line_index = read_line(address);
            latency = MEMORY_ACCESS_TIME * CACHE_LINE_SIZE;
            _cache_misses++;
        } else {
            _cache_hits++;
        }
        write_into_line_word(line_index, calculate_offset(address), value);
        return latency;
    } else {
        _unibus->write_word(address, value);
        return MEMORY_ACCESS_TIME;
    }
}

pair<uint16, uint> CPUCache::read_word(uint18 address) {
    if (is_cached_address(address) && _enabled) {
        uint latency = CACHE_HIT_TIME;
        int line_index = find_in_cache(address);
        if (line_index == -1) {
            line_index = read_line(address);
            latency = MEMORY_ACCESS_TIME * CACHE_LINE_SIZE;
            _cache_misses++;
        } else {
            _cache_hits++;
        }
        return make_pair(read_from_line_word(line_index, calculate_offset(address)), latency);
    } else {
        return make_pair(_unibus->read_word(address), MEMORY_ACCESS_TIME);
    }
}

uint CPUCache::write_byte(uint18 address, uint8 value) {
    if (is_cached_address(address) && _enabled) {
        uint latency = CACHE_HIT_TIME;
        int line_index = find_in_cache(address);
        if (line_index == -1) {
            line_index = read_line(address);
            latency = MEMORY_ACCESS_TIME * CACHE_LINE_SIZE;
            _cache_misses++;
        } else {
            _cache_hits++;
        }
        write_into_line_byte(line_index, calculate_offset(address), value);
        return latency;
    } else {
        _unibus->write_byte(address, value);
        return MEMORY_ACCESS_TIME;
    }
}

pair<uint8, uint> CPUCache::read_byte(uint18 address) {
    if (is_cached_address(address) && _enabled) {
        uint latency = CACHE_HIT_TIME;
        int line_index = find_in_cache(address);
        if (line_index == -1) {
            line_index = read_line(address);
            latency = MEMORY_ACCESS_TIME * CACHE_LINE_SIZE;
            _cache_misses++;
        } else {
            _cache_hits++;
        }
        return make_pair(read_from_line_byte(line_index, calculate_offset(address)), latency);
    } else {
        return make_pair(_unibus->read_byte(address), MEMORY_ACCESS_TIME);
    }
}

int CPUCache::read_line(uint18 address) {
    int line_index = least_recently_used();
    write_back_line(line_index);
    uint18 base = calculate_base(address);
    CacheLine line;
    for (int i = 0; i < CACHE_LINE_SIZE; i += 2) {
        uint16 word = _unibus->read_word(base + i);
        line.data[i] = (uint8) ((word) & 0xFF);
        line.data[i + 1] = (uint8) ((word >> 8) & 0xFF);
    }
    line.time_stamp = _time_counter;
    line.base = base;
    line.dirty = false;
    _memory[line_index] = line;
    return line_index;
}

void CPUCache::write_back_line(int line_index) {
    if (!_memory[line_index].dirty)
        return;
    for (int i = 0; i < CACHE_LINE_SIZE; i += 2) {
        uint16 word = (_memory[line_index].data[i + 1] << 8) | (_memory[line_index].data[i]);
        _unibus->write_word(_memory[line_index].base + i, word);
    }
}

int CPUCache::least_recently_used() {
    int result = 0;
    uint result_time_stamp = _memory[0].time_stamp;
    for (int i = 1; i < _memory.size(); i++) {
        if (_memory[i].time_stamp < result_time_stamp) {
            result = i;
            result_time_stamp = _memory[i].time_stamp;
        }
    }
    return result;
}

int CPUCache::find_in_cache(uint18 address) {
    uint18 base = calculate_base(address);
    for (int i = 0; i < _memory.size(); i++) {
        if (_memory[i].base == base)
            return i;
    }
    return -1;
}

void CPUCache::write_into_line_word(int line_index, uint18 offset, uint16 value) {
    _memory[line_index].time_stamp = _time_counter;
    _time_counter++;
    _memory[line_index].data[offset] = (uint8) (value & 0xFF);
    _memory[line_index].data[offset + 1] = (uint8) ((value >> 8) & 0xFF);
    _memory[line_index].dirty = true;
}

void CPUCache::write_into_line_byte(int line_index, uint18 offset, uint8 value) {
    _memory[line_index].time_stamp = _time_counter;
    _time_counter++;
    _memory[line_index].data[offset] = value;
    _memory[line_index].dirty = true;
}

uint16 CPUCache::read_from_line_word(int line_index, uint18 offset) {
    _memory[line_index].time_stamp = _time_counter;
    _time_counter++;
    uint16 result;
    result = _memory[line_index].data[offset];
    result |= _memory[line_index].data[offset + 1] << 8;
    return result;
}

uint8 CPUCache::read_from_line_byte(int line_index, uint18 offset) {
    _memory[line_index].time_stamp = _time_counter;
    _time_counter++;
    return _memory[line_index].data[offset];
}

bool CPUCache::is_cached_address(uint18 address) {
    for (auto it = _cached_segments.begin(); it != _cached_segments.end(); ++it) {
        if (it->first <= address && address < it->second)
            return true;
    }
    return false;
}