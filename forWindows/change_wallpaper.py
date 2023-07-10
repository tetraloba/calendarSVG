import ctypes

def change_wallpaper(filename):
    # fileName = "C:\users\tetraloba\Desktop\Calendar\230616_small.png"
    # print(fileName)
    ctypes.windll.user32.SystemParametersInfoW(20, 0, filename , 0)
