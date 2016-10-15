@if -%1-==-- goto error_message

@pdp11-asm %1.asm %1.bin
@python font_injector.py %1.bin

@goto end
:error_message
@echo "Usage build asm_file_without_extension"

:end