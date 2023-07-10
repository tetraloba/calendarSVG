from datetime import datetime as dt
from .calendar import Calendar

my_calendar = Calendar(row=5, column=10)
my_calendar.draw_calendar(dt.now()) # 現在日時を渡す
