# 1. Get the directory where this script is located
$InstallDir = $PSScriptRoot

# 2. Get the current User PATH
$UserPath = [Environment]::GetEnvironmentVariable("Path", "User")

# 3. Check if the directory is already in the PATH to prevent duplicates
if ($UserPath -match [regex]::Escape($InstallDir)) {
    Write-Host "fcu is already installed in your PATH." -ForegroundColor Yellow
} else {
    # 4. Append it safely and apply it permanently
    $NewPath = "$UserPath;$InstallDir"
    [Environment]::SetEnvironmentVariable("Path", $NewPath, "User")
    
    Write-Host "Success! Added $InstallDir to your PATH." -ForegroundColor Green
    Write-Host "Please restart your terminal to use the 'fcu' command." -ForegroundColor Cyan
}

# Keep the window open if they run it by double-clicking
Read-Host "Press Enter to exit"