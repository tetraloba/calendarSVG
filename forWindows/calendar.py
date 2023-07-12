from datetime import datetime

from .SVG4Python.svg4py.svg import SVG, RGB
from ..common_modules.calendar_util import calendar_dates as cd

class Calendar:
    def __init__(self, width, height, matrix_x1, matrix_y1, matrix_x2, matrix_y2, font_size):
        self.width = width
        self.height = height
        self.matrix_x1 = matrix_x1
        self.matrix_y1 = matrix_y1
        self.matrix_x2 = matrix_x2
        self.matrix_y2 = matrix_y2
        self.font_size = font_size
        self.background_color = RGB(0, 0, 0) # 背景色: 黒
        self.today = datetime.now().date()
        self.x1, self.x2 = 100, 1820
        self.wdays = ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat']
        self.x_list = [self.matrix_x1 + (self.matrix_x2 - self.matrix_x1) / self.wdays.__len__() * i for i in range(self.wdays.__len__() + 1)]
        self.y_list = [self.matrix_y1 + (self.matrix_y2 - self.matrix_y1) / 6 * i for i in range(6 + 1)]
        # print(self.x_list) # debug
        # print(self.y_list) # debug
    
    def __draw_matrix(self, stroke_color, svg: SVG):
        for i in range(self.x_list.__len__()):
            svg.line(self.x_list[i], self.y_list[0], self.x_list[i], self.y_list[-1], stroke_color)
        for i in range(self.y_list.__len__()):
            svg.line(self.x_list[0], self.y_list[i], self.x_list[-1], self.y_list[i], stroke_color)
        return None

    def __fill_sun_sat(self, svg: SVG):
        # todo rectのstrokeをnoneにしたい。 stroke_widthを0にしては？
        svg.rect(self.x_list[0], self.y_list[0], self.x_list[1] - self.x_list[0], self.y_list[-1] - self.y_list[0], RGB(100, 0, 0))
        svg.rect(self.x_list[6], self.y_list[0], self.x_list[7] - self.x_list[6], self.y_list[-1] - self.y_list[0], RGB(0, 0, 100))
        return None

    def __print_year_month(self, font_size, svg: SVG, color: RGB = None):
        if color is None:
            color = RGB(255, 255, 255)
        svg.text(self.x_list[0], self.y_list[0] - font_size * 2, self.today.strftime('%B') + '  ' + self.today.year.__str__(), font_size=font_size, fill_color=color, stroke_width=0)
        return None

    def __print_wdays(self, font_size, svg: SVG, color: RGB = None):
        padding = 20
        for i in range(self.wdays.__len__()):
            svg.text((self.x_list[i] + self.x_list[i + 1]) / 2 - font_size * 2 / 3, self.y_list[0] - padding, self.wdays[i], font_size=font_size, fill_color=color, stroke_width=0)
        return None

    def __print_dates(self, font_size, svg: SVG, color: RGB = None):
        if color is None:
            color = RGB(255, 255, 255)
        padding = 5
        gen_itr = cd(self.today)
        for y in self.y_list[:-1]:
            for x in self.x_list[:-1]:
                date = gen_itr.__next__()
                # print(date.month, self.today.month) # debug
                if date.month == self.today.month:
                    stroke_color = color
                else:
                    stroke_color = RGB(color.r // 2, color.g // 2, color.b // 2)
                svg.text(x + padding, y + font_size, str(date.day), None, font_size, stroke_color, stroke_width=0)
        return None
    
    def draw_calendar(self, file_name):
        svg = SVG(file_name, 0, 0, self.width, self.height, unit='px')
        svg.stroke_width = 3
        svg.fill_color = RGB(0, 255, 0)
        # 背景色の設定
        svg.rect(0, 0, '100%', '100%', self.background_color, stroke_width=0)
        # 日曜日と土曜日に色を付ける
        self.__fill_sun_sat(svg)
        # マトリックスを出力
        self.__draw_matrix(RGB(0, 255, 0), svg)
        # 曜日を出力
        self.__print_wdays(self.font_size, svg, RGB(0, 255, 0))
        # 年月を出力
        self.__print_year_month(self.font_size + 1.5, svg, RGB(0, 255, 0))
        # 日付を出力
        self.__print_dates(self.font_size, svg, RGB(0, 255, 0))

if __name__ == '__main__':
    # calendar = Calendar(1920, 1080, 1070, 600, 1870, 1030, 50)
    calendar = Calendar(1920, 1080, 200, 200, 1870, 1030, 50)
    calendar.draw_calendar()
