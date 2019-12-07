#!/bin/bash
echo "Copying executable to runtime directory..."
cp "bin/Linux/x64/VK by D3PSI" "VK/VK by D3PSI"
echo "Debugging VK..."
function code { cd VK/; }
code
gdb -ex run "VK by D3PSI"
