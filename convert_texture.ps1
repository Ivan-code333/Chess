$bytes = [System.IO.File]::ReadAllBytes("assets/pieces/favicon.png")
$hex = [System.BitConverter]::ToString($bytes).Replace("-", ", 0x")
$hex = "0x" + $hex

$content = @"
#include "Favicon.h"

const unsigned int texture_data_size = $($bytes.Length);
const unsigned char texture_data[] = {
$hex
};
"@

$content | Out-File -Encoding ASCII "Favicon.cpp" 