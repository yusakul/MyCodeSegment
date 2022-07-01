$VMDetected=$False
$Arguments= @{
 Class = 'Win32_ComputerSystem'
 Filter = 'NumberOfLogicalProcessors < 2 OR TotalPhysicalMemory < 2147483648'
}
if (Get-WmiObject @Arguments) { $VMDetected=$True }
$VMDetected

$VMwareDetected = $False
$VMAdapter = Get-WmiObject Win32_NetworkAdapter -Filter 'Manufacturer LIKE "%VMware%" OR Name LIKE "%VMware%"'
$VMBios = Get-WmiObject Win32_BIOS -Filter 'SerialNumber LIKE "%VMware%"'
$VMToolsRunning = Get-WmiObject Win32_Process -Filter 'Name="vmtoolsd.exe"'
if ($VMAdapter -or $VMBios -or $VMToolsRunning) { $VMwareDetected = $True }
$VMwareDetected
