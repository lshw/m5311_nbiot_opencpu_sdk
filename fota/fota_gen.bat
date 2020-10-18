::Created by XGL,for FOTA firmware package generation,2029/2/7
@set cur=%2 
@md %~dp0..\fota\%2
@xcopy /y %1\flash_download.cfg %~dp0..\fota\%2 >nul
@xcopy /y %1\mt2625_bootloader.bin %~dp0..\fota\%2 >nul
@xcopy /y %1\nbiot_m2m_demo.bin %~dp0..\fota\%2 >nul
@xcopy /y %1\nbiot_m2m_demo.elf %~dp0..\fota\%2 >nul
@xcopy /y  package.xml %~dp0..\fota\%2 >nul
@copy  %1\nbiot_m2m_demo.bin %~dp0..\fota\%2\ROM.bin
@copy  %1\nbiot_m2m_demo.elf %~dp0..\fota\%2\ROM.elf
@7-Zip\7z.exe a -tzip  %~dp0..\fota\%2\%2.zip %~dp0..\fota\%2\ROM.* %~dp0..\fota\%2\package.xml