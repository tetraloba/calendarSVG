from datetime import datetime
import calendar

# 色を変える用の文字列
u_line = '\033[4m'
green = '\033[32m'
red = '\033[31m'
blue = '\033[36m'
yellow_b = '\033[43m'
black_b = '\033[40m'

today = datetime.now() # 現在日時を取得
print(green + today.strftime("%B"), today.year) # 年月を出力
print(u_line + red + " Sun" + green + " Mon Tue Wed Thu Fri" + blue + " Sat " + green) # 曜日を出力

# 日付部分を出力
first_wday, date_max = calendar.monthrange(today.year, today.month)
offset = (first_wday + 1) % 7
for i in range(6):
    print('|', end='')
    for j in range(7):
        date = j + 1 - offset + i * 7 # [i][j]マスの日付

        if j == 0: # 土,日曜日は色を変える
            print(red, end='')
        elif j == 6:
            print(blue, end='')

        if date == today.day: # 今日は背景色を変える
            print(yellow_b, end='')

        date_str = str(date).rjust(3) if 1 <= date and date <= date_max else '   '
        print(date_str, end='') # 日付を出力

        if date == today.day: # 今日の背景色を元に戻す
            print(black_b, end='')

        if j == 0 or j == 6: # 土,日曜日の色を元に戻す
            print(green, end='')

        print('|', end='')
    print()

print('\033[m') # 修飾を解除
