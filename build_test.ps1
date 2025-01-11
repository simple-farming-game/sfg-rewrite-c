# Remove existing Debug directory
if (Test-Path -Path .\build\Debug) {
    Remove-Item -Path .\build\Debug -Recurse -Force
    echo "[INFO] Existing Debug directory has been deleted."
} else {
    echo "[INFO] No Debug directory to delete. Proceeding with the next steps."
}

# Print separator
echo --------------------
echo build
echo --------------------

# Build Debug configuration
cmake --build build --config Debug --target SFG

# Print separator
echo --------------------
echo run
echo --------------------

# Navigate to directory and execute
.\build\Debug\SFG.exe
