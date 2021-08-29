$portName = [System.IO.Ports.SerialPort]::getportnames()
$port= new-Object System.IO.Ports.SerialPort $portName,9600,None,8,one
$port.open()
$port.WriteLine("off")
$port.Close()