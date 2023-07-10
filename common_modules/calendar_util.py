from datetime import timedelta, datetime as dt
def calendar_dates(today: dt.date, start_on_monday: bool = False):
    # today = dt.date(today) # dt が渡されたときのため
    first_day = today.replace(day=1)
    date = first_day - timedelta(first_day.isoweekday() % 7 - (1 if start_on_monday else 0)) # 先月または1日で直近の日曜日
    while date < first_day:
        yield date
        # yield None
        date = date + timedelta(1)
    while date.month <= first_day.month:
        yield date
        date = date + timedelta(1)
    while date.isoweekday() % 7 != 6:
        yield date
        # yield None
        date = date + timedelta(1)
    yield date
    # yield None

if __name__ == '__main__':
    date_gen_itr = calendar_dates(dt.today().replace(day=14))
    for date in date_gen_itr:
        if date is not None:
            print(date, date.isoweekday() % 7)
        else:
            print(date)