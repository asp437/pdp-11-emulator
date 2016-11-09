#include <iostream>
#include <c++/fstream>

unsigned char char_table[256];

int main() {
    for (char ch = 'a'; ch <= 'z'; ch++)
        char_table[ch] = (unsigned char) (ch - 'a');
    for (char ch = 'A'; ch <= 'Z'; ch++)
        char_table[ch] = (unsigned char) (ch - 'A' + 26);
    for (char ch = '0'; ch <= '9'; ch++)
        char_table[ch] = (unsigned char) (ch - '0' + 52);
    char_table['~'] = 62;
    char_table['@'] = 63;
    char_table['['] = 64;
    char_table[']'] = 65;
    char_table['{'] = 66;
    char_table['}'] = 67;
    char_table['<'] = 68;
    char_table['>'] = 69;

    char_table[':'] = 70;
    char_table[';'] = 71;
    char_table[','] = 72;
    char_table['.'] = 73;
    char_table['"'] = 74;
    char_table['`'] = 75;
    char_table['\''] = 76;
    char_table['?'] = 77;
    char_table['-'] = 78;
    char_table['='] = 79;

    char_table['_'] = 80;
    char_table['+'] = 81;
    char_table['('] = 82;
    char_table[')'] = 83;
    char_table['&'] = 84;
    char_table['*'] = 85;
    char_table['%'] = 86;
    char_table['^'] = 87;
    char_table['#'] = 88;
    char_table['$'] = 89;
    char_table['!'] = 90;
    char_table['|'] = 91;
    char_table['/'] = 92;
    char_table['\\'] = 93;
    char_table[' '] = 94;

    std::string hello_message =
        "Hi. You're using yet another PDP-11 emulator. This emulator supports few commands. \"halt\" command halts the PDP-11 Machine. \"logo\" command displays logo on the screen. \"help\" command prints this help/greeting message. \"clr\" clears screen content. The pipeline and cache emulation still in testing and could cause some crashes or non-valid results.";
    std::ofstream ofs("msg.bin", std::ios_base::binary | std::ios_base::trunc);
    unsigned short message_size = hello_message.size();

    ofs.write((const char *) &message_size, sizeof(unsigned short));

    for (int i = 0; i < hello_message.size(); i++) {
        ofs.put(char_table[hello_message.c_str()[i]]);
    }
    ofs.flush();
    ofs.close();
    return 0;
}