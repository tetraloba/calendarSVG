@echo off
rem 1.画像ファイルの設定
rem set WALLPAPER_IMG=C:\test.bmp
set WALLPAPER_IMG=%~1

rem debug
echo batch.echo: wallpaper is %WALLPAPER_IMG%

rem 2.デスクトップの背景に設定するレジストリ登録
reg add "HKCU\control panel\desktop" /v wallpaper /t REG_SZ /d %WALLPAPER_IMG% /f

rem 以下２つは他の人のやつ。メモ程度に。
rem reg add "HKEY_CURRENT_USER\Control Panel\Desktop" /v "Wallpaper" /t REG_SZ /d "C:\Windows\Web\Wallpaper\Theme1\img1.jpg" /f
rem reg add "HKEY_CURRENT_USER\Control Panel\Desktop" /v "Wallpaper" /t REG_SZ /d "C:\Windows\Web\Wallpaper\Windows\img0.jpg" /f

rem 3.画像の拡大表示設定
rem reg add "HKCU\control panel\desktop" /v WallPaperStyle /t REG_SZ /d 2 /f

rem 4.背景の変更を適用するコマンドを連打※
rem 1回では反応しないことが多いため10回挿入
RUNDLL32.EXE user32.dll,UpdatePerUserSystemParameters
RUNDLL32.EXE user32.dll,UpdatePerUserSystemParameters
RUNDLL32.EXE user32.dll,UpdatePerUserSystemParameters
RUNDLL32.EXE user32.dll,UpdatePerUserSystemParameters


rem pause
exit