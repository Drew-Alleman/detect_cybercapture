
# detect_cybercapture

[Blog post going into detail](https://drewalleman.xyz/red-teaming/malware-development/2026/08/02/detecting-avast-cybercapture-using-window-classes)

Real-time detection of the Avast antivirus overlay. Because the CyberCapture overlay window is globally visible during a session, any new application launched while a scan is active will also detect the `snxhk_border_mywnd` class and incorrectly flag itself as sandboxed.
<img width="1697" height="1016" alt="detect_demo" src="https://github.com/user-attachments/assets/1a76efd5-c291-4a0b-bc90-1352437f24b7" />
