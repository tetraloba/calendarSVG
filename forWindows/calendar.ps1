./calendar.exe
./calendar_small.exe | Out-Null

$date = Get-Date -Format "yyMMdd"
$svgFilePath = "C:\users\tetraloba\Desktop\Calendar\${date}.svg"
$svgFilePath_small = "C:\users\tetraloba\Desktop\Calendar\${date}_small.svg"
$pngFilePath_small = "C:\users\tetraloba\Desktop\Calendar\${date}_small.png"

C:\Users\Public\Documents\Shared_Applications\InkscapePortable_1.1.2\App\Inkscape\bin\inkscape.com --export-type="png" --export-width="1920" ${svgFilePath}
C:\Users\Public\Documents\Shared_Applications\InkscapePortable_1.1.2\App\Inkscape\bin\inkscape.com --export-type="png" --export-width="1920" ${svgFilePath_small}

./changeWallPaper.bat $pngFilePath_small

# Remove .png and .svg created more than 1 day ago
Get-ChildItem | Where-Object {($_.Mode -eq "-a----") -and ($_.CreationTime -lt (Get-Date).AddDays(-1))} | Remove-Item -in *.png,*.svg

# Read-Host "Please press the enter key..."
pause