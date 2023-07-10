from datetime import datetime
from os import getcwd, chdir, name

from . import svg2png
from .calendar import Calendar
from . import change_wallpaper

# work_dir = R"C:\users\tetraloba\Desktop\Calendar\\"
word_dir = getcwd()
today = datetime.today()
date = str(today.year)[2:] + str(today.month).zfill(2) + str(today.day).zfill(2)
svg_filename = date + '.svg'
svg_filename_small = date + '_small.svg'
png_filenaame = date + '.png'
png_filenaame_small = date + '_small.png'

# chdir(word_dir)

# .\.venv\Scripts\Activate.ps1 # python venv start

# generate SVG
calendar = Calendar(1920, 1080, 1070, 600, 1870, 1030, 50) # regular
calendar.draw_calendar(svg_filename)
calendar = Calendar(1920, 1080, 200, 200, 1870, 1030, 50) # small
calendar.draw_calendar(svg_filename_small)

# SVG->PNG convert
svg2png.convert(svg_filename, png_filenaame)
svg2png.convert(svg_filename_small, png_filenaame_small)

# change Windows wallpaper
if name == 'nt':
    change_wallpaper.change_wallpaper(word_dir + png_filenaame_small)

# deactivate # python venv stop

# Remove .png and .svg created more than 1 day ago
# Get-ChildItem | Where-Object {($_.Mode -eq "-a----") -and ($_.CreationTime -lt (Get-Date).AddDays(-1))} | Remove-Item -in *.png,*.svg
