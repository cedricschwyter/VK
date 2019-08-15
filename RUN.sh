#!/bin/bash
echo "Copying executable to runtime directory..."
cp "bin/Linux/x64/VK by D3PSI" "VK/VK by D3PSI"
echo "Running VK..."
function code { cd VK/; }
code
./"VK by D3PSI" &
disown
