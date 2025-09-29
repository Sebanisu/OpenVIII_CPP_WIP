param(
    [string]$TargetFile
)

$clang = "C:/Program Files/LLVM/bin/clang-format.exe"  # adjust path if needed

if ($TargetFile) {
    Write-Output "Formatting $TargetFile"
    & $clang -i --style=file $TargetFile
} else {
    # No file passed, format all tracked files
    $files = git ls-files *.cpp *.hpp *.h *.c *.cc *.hh
    foreach ($f in $files) {
        Write-Output "Formatting $f"
        & $clang -i --style=file $f
    }
}

Write-Output "Done!"
