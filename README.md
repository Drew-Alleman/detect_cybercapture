# detect_cybercapture
Showcases two different methods to detect avast cyber capture sandbox. Source files are found in the `examples/` folder.

## Methods 
- [Windows Class Detection](https://drewalleman.xyz/red-teaming/malware-development/2026/08/02/detecting-avast-cybercapture-using-window-classes)
- [String Based Detection](https://drewalleman.xyz/red-teaming/malware-development/2026/08/04/detecting-avast-cybercapture-using-strings-in-memory)

### Windows Class Detection
Real-time detection of the Avast antivirus overlay. Because the CyberCapture overlay window is globally visible during a session, any new application launched while a scan is active will also detect the `snxhk_border_mywnd` class and incorrectly flag itself as sandboxed.
<img width="1697" height="1016" alt="detect_demo" src="https://github.com/user-attachments/assets/1a76efd5-c291-4a0b-bc90-1352437f24b7" />


### String Based Detection
Searches the current processes memory for the `SnxHk_InstallHook` string.
<img width="1697" height="1018" alt="string_demo" src="https://github.com/user-attachments/assets/3b8ae4de-198c-47f8-ad9b-ce419636010c" />
