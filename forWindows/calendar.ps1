$workDir = "C:\users\tetraloba\Desktop\Calendar\"
$date = Get-Date -Format "yyMMdd"
$svgFileName = "${date}.svg"
$svgFileName_small = "${date}_small.svg"
$pngFileName = "${date}.png"
$pngFileName_small = "${date}_small.png"

$ErrorActionPreference = "stop" # Interrupts the process when there is an error.
cd ${workDir}

# generate SVG
.\calendar.exe
.\calendar_small.exe | Out-Null

.\.venv\Scripts\Activate.ps1 # python venv start

# SVG->PNG convert
python -c "import svg2png; svg2png.convert(\`"${svgFileName}\`",\`"${pngFileName}\`")"
python -c "import svg2png; svg2png.convert(\`"${svgFileName_small}\`",\`"${pngFileName_small}\`")"

# change Windows wallpaper
python .\changeWallpaper.py ${workDir}${pngFileName_small}

deactivate # python venv stop

# Remove .png and .svg created more than 1 day ago
Get-ChildItem | Where-Object {($_.Mode -eq "-a----") -and ($_.CreationTime -lt (Get-Date).AddDays(-1))} | Remove-Item -in *.png,*.svg

# Read-Host "Please press the enter key..."
pause