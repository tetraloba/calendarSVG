from datetime import datetime
import calendar

from .clifig import Clifig

class Calendar(Clifig):
    def draw_calendar(self, today:datetime):
        # 色を変える用の文字列
        u_line = '\033[4m'
        green = '\033[32m'
        red = '\033[31m'
        blue = '\033[36m'
        yellow_b = '\033[43m'
        black_b = '\033[40m'

        # print(green + today.strftime("%B"), today.year) # 年月を出力
        super().append(green + today.strftime("%B") + ' ' + str(today.year))
        # print(u_line + red + " Sun" + green + " Mon Tue Wed Thu Fri" + blue + " Sat " + green) # 曜日を出力
        super().append(u_line + red + "  Sun" + green + "  Mon  Tue  Wed  Thu  Fri" + blue + "  Sat " + green)
        # 日付部分を出力
        first_wday, date_max = calendar.monthrange(today.year, today.month)
        offset = (first_wday + 1) % 7
        for i in range(6):
            # print('|', end='')
            line = '|'
            for j in range(7):
                date = j + 1 - offset + i * 7 # [i][j]マスの日付

                if j == 0: # 土,日曜日は色を変える
                    # print(red, end='')
                    line += red
                elif j == 6:
                    # print(blue, end='')
                    line += blue

                if date == today.day: # 今日は背景色を変える
                    # print(yellow_b, end='')
                    line += yellow_b

                date_str = str(date).ljust(4) if 1 <= date and date <= date_max else '    '
                if date == today.day:
                    date_str = str(date).ljust(2) + '🐈'
                # print(date_str, end='') # 日付を出力
                line += date_str

                if date == today.day: # 今日の背景色を元に戻す
                    # print(black_b, end='')
                    line += black_b

                if j == 0 or j == 6: # 土,日曜日の色を元に戻す
                    # print(green, end='')
                    line += green

                # print('|', end='')
                line += '|'
            # print()
            super().append(line)

        # print('\033[m') # 修飾を解除
        super().append('\033[m')
        super().print()
