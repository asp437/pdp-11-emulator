The directory util contains following bins:

* `font.bin` - pregenerated font, converted for PDP-11 friendly format.
* `keyboard_font_conversion_table.bin` - binfile contains table with information to convert keyboard keycodes into charcodes.
* `logo.bin` - pregenerated logo, converted for PDP-11 friendly format.
* `msg.bin` - pregenerated message, converted for PDP-11 friendly format.

All this files used during creation of binary ROM file (inserted into it to the end).

ROM files:

* `console.bin` - console program which supports a number of commands. Use command help in the console to get more information.
* `console_echo.bin` - console program which doesn't support any commands.
* `font_render_test.bin` - 'Hello World' program which display one word on the screen. Example of working with font.
* `new_video_mode_dma.bin` - Fill the screen with the lines of different colors (supported only 4 colors). Example of working with emulated video-controller via DMA.
