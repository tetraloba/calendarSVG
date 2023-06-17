from svglib.svglib import svg2rlg
from reportlab.graphics import renderPM

def convert(svg_file_path, png_file_name):
    # svg_file_path = '230430_small.svg'
    # png_file_name = 'test.png'
    drawing = svg2rlg(svg_file_path)
    renderPM.drawToFile(drawing, png_file_name, fmt='PNG')
