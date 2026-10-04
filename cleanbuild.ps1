Remove-Item -Recurse -Force bin, build -ErrorAction SilentlyContinue
cmake -DCMAKE_BUILD_TYPE=Release -B build -S .
cmake --build build --config Release -j4
$cstart = Get-ChildItem -Path . -Recurse -Filter cstart.exe -File | Select-Object -First 1 | Resolve-Path -Relative
Write-Host "Run with `"$cstart`""
