# 右記のサイトからコピペした https://neos21.net/blog/2021/08/08-01.html

# デスクトップ背景設定スクリプト : 実行する度に「指定の画像を背景に設定する」「黒一色の背景に変更する」を切り替える

# 変更したい壁紙のパスを指定する : 黒背景に変更する場合はこの変数を空文字に変更して流用・続行する
$wallPaperPath = 'C:\PATH\TO\WALL-PAPER.jpg';

# 背景を変更する C# 関数
$code = @'
using System;
using System.Drawing;
using System.Runtime.InteropServices;
using Microsoft.Win32;

public class WallPaper {
  [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
  private static extern int SystemParametersInfo(int uAction, int uParm, string lpvParam, int fuWinIni);
  
  [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
  private static extern int SetSysColors(int cElements, int[] lpaElements, int[] lpRgbValues);
  
  public const int SetDesktopWallpaper = 0x0014;
  public const int UpdateIniFile = 0x01;
  public const int SendWinIniChange = 0x02;
  
  public const int colourDesktop = 1;
  public int[] first = {colourDesktop};
  
  // 背景画像を変更する : 空文字を指定すれば背景画像なしになる
  // レジストリの WallPaper 値をセットすることで壁紙を設定できるが、SystemParametersInfo() を呼び出さないと即座に反映されない
  public static void SetWallPaper(string wallPaperPath) {
    SystemParametersInfo(SetDesktopWallpaper, 0, wallPaperPath, UpdateIniFile | SendWinIniChange);
    
    RegistryKey regKey = Registry.CurrentUser.OpenSubKey("Control Panel\\Desktop", true);
    regKey.SetValue("WallPaper", wallPaperPath);
    // アスペクト比を維持して画面全体の大きさにあわせて拡大・画面に収まらない部分ははみ出すよう表示方法を設定する
    regKey.SetValue("WallpaperStyle", "10");
    regKey.SetValue("TileWallpaper", "0");
    regKey.Close();
  }
  
  // 背景色を設定する
  public static void SetBackgroundColour(byte r, byte g, byte b) {
    int[] elements = {colourDesktop};
    System.Drawing.Color colours = System.Drawing.Color.FromArgb(r,g,b);
    int[] colors = {System.Drawing.ColorTranslator.ToWin32(colours)};
    SetSysColors(elements.Length, elements, colors);
    
    RegistryKey regKey = Registry.CurrentUser.OpenSubKey("Control Panel\\Colors", true);
    regKey.SetValue("Background", string.Format("{0} {1} {2}", colours.R, colours.G, colours.B));
    regKey.Close();
  }
}
'@;
Add-Type -TypeDefinition $code -ReferencedAssemblies System.Drawing.dll;

# 現在の値を取得する
$currentWallPaper = (Get-Item -Path 'Registry::HKEY_CURRENT_USER\Control Panel\Desktop').GetValue('WallPaper');

$message = '';
if([string]::IsNullOrEmpty("$currentWallPaper")) {
  $message = '現在黒背景なので、背景画像を設定します。';
} else {
  $message = '現在背景画像が設定されているので、黒背景にします。';
  $wallPaperPath = '';
}

Write-Host "$message";
#Read-Host "${message}Enter で開始します。";

# 背景を固定で黒色にする
[WallPaper]::SetBackgroundColour(0, 0, 0);
# 背景画像を設定する
[WallPaper]::SetWallPaper($wallPaperPath);

Write-Host '完了・終了します。';
#Read-Host '完了・Enter で終了します。';
exit;