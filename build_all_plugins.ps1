$plugins = dir vst\Builds
foreach ($plugin in $plugins) {
	Write-Host "Building $plugin ..."
	& "C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe" vst\Builds\$plugin\VisualStudio2015\Blankenhain.sln | Out-File -Encoding utf8 builtPlugins\$plugin.buildlog.txt
}
# pause
Write-Host "Press any key to continue ..."
$x = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")