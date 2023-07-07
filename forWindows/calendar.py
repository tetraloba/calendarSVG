from datetime import datetime
from SVG4Python.SVG import Pos, SVG, RGB

viewBox_width = 1920
viewBox_height = 1080
background_color = RGB(0, 0, 0) # 背景色: 黒
now = datetime.now().date()
file_name = str(now.year)[2:] + str(now.month).zfill(2) + str(now.day).zfill(2) + ".svg"
print(file_name)

svg = SVG(file_name)
svg.set_unit('px')
svg.start(0, 0, viewBox_width, viewBox_height, viewBox_width, viewBox_height)
# 背景色の設定
svg.rect(0, 0, background_color, '100%', '100%')
# マトリックスを出力
# 曜日を出力
# 日曜日と土曜日に色を付ける
# 年月を出力
# 日付を出力
svg.finish()

    # pos1 = Pos{50.0, 150.0};
    # pos2 = Pos{SIZEX - pos1.x, SIZEY - 20};
    # rgb = RGB{128, 255, 128};
    # fillSunSat(pos1, pos2, fout);
    # matrix(pos1, pos2, 7, 5, rgb, fout);
    # printMonthYear(&now, rgb, fout);
    # printDate(pos1, pos2, &now, rgb, fout);
    # printWday(Pos{pos1.x, pos1.y - 10}, pos2, rgb, 32, fout);
    # finish(fout);
    # fout.close();