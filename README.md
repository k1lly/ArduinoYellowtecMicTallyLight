## About
Control and drive your Yellowtec Mika Mic Tally Light using an Arduino and Adafruit Motor Shield via Stream Deck.  
## Usage
Just upload the `MicTallyLightControl.ino` file and add 3 buttons to your Stream Deck profile with the following commands (COM4 Serial port is used in these examples, your port number might differ, so don't forget to check):
I've found that using the following arguments makes running the PowerShell scripts practically invisible and non-distructing. 
```
cmd /c start /min "" powershell -NoLogo -WindowStyle Hidden -ExecutionPolicy Bypass -File "[path_to_your_script]hot.ps1"

cmd /c start /min "" powershell -NoLogo -WindowStyle Hidden -ExecutionPolicy Bypass -File "[path_to_your_script]idle.ps1"

cmd /c start /min "" powershell -NoLogo -WindowStyle Hidden -ExecutionPolicy Bypass -File "[path_to_your_script]off.ps1"
```
## How to make your own
Feel free to look at all the supporting files, I've tried to document and explain every decision with links to sources and authors.
## How to contribute
If you spot an error or have an idea how to improve existing solution feel free to open a PR.