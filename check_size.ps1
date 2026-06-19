Add-Type -AssemblyName System.Drawing
$path = "C:\Users\ASUS\OneDrive\ドキュメント\GitHub\Crossing-Street\assets\ui\hud\hud_bar.png"
$img = [System.Drawing.Image]::FromFile($path)
$w = $img.Width
$h = $img.Height
$ratio = [math]::Round($w / $h, 2)
Write-Host "Width: $w"
Write-Host "Height: $h"
Write-Host "Ratio W/H: $ratio"
$img.Dispose()
