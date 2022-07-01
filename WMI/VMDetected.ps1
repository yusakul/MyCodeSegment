$VMDetected=$False
$Arguments= @{
 Class ="Win32_ComputerSystem"
 Filter ="NumberOfLogicalProcessors < 2 OR TotalPhysicalMemory < 2147483648"
}
if (Get-WmiObject @Arguments) { $VMDetected=$True }
$VMDetected
