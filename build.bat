::created by xgl,2019/1/24
@set temp_path=%path%
@set path=%path%;%~dp0\utils
@set param1=%1
@if NOT "%param1%"=="GB" if NOT "%param1%"=="LV" if not "%param1%"== "CM" if not "%param1%" == "CL" if not "%param1%" == "DB" if not "%param1%" == "CLEAN" (goto error1)
@if %param1% == LV (goto make_lv) 
@if %param1% == CM (goto make_cm) 
@if %param1% == CL (goto make_cl) 
@if %param1% == DB (goto make_db) 
@if %param1% == GB (goto make_gb) 
@if %param1% == CLEAN (goto make_clean) 

:error1
@echo usage:build.bat param
@echo 	LV:compile LV binary
@echo 	CM:compile CM binary
@echo 	CL:compile CL binary
@echo 	DB:compile DB binary
@echo 	GB:compile GB binary
@echo 	CLEAN:clean the project
@goto exit1
:make_lv
@if exist %~dp0\firmware\LV (goto cr1)
@md %~dp0\firmware\LV
@xcopy /y %~dp0\utils\flash_download.cfg %~dp0\firmware\LV >nul
@xcopy /y %~dp0\utils\mt2625_bootloader.bin %~dp0\firmware\LV  >nul
:cr1
@echo Making LV Version
@echo HWVER :=LV >%~dp0\utils\config.mk
@echo LINK_FILE:=link_option_lv.tmp >>%~dp0\utils\config.mk
@make
@goto exit1
:make_cm
@if  exist %~dp0\firmware\CM (goto cr2)
@md %~dp0\firmware\CM
@xcopy /y %~dp0\utils\flash_download.cfg %~dp0\firmware\CM >nul
@xcopy /y %~dp0\utils\mt2625_bootloader.bin %~dp0\firmware\CM  >nul
:cr2
@echo Making CM Version
@echo HWVER :=CM > %~dp0\utils\config.mk
@echo LINK_FILE:=link_option_cm.tmp >> %~dp0\utils\config.mk
@make
@goto exit1
:make_cl
@if  exist %~dp0\firmware\CL (goto cr3)
@md %~dp0\firmware\CL
@xcopy /y %~dp0\utils\flash_download.cfg %~dp0\firmware\CL >nul
@xcopy /y %~dp0\utils\mt2625_bootloader.bin %~dp0\firmware\CL  >nul
:cr3
@echo Making CL Version
@echo HWVER :=CL > %~dp0\utils\config.mk
@echo LINK_FILE:=link_option_cm.tmp >> %~dp0\utils\config.mk
@make
@goto exit1
:make_db
@if  exist %~dp0\firmware\DB (goto cr4)
@md %~dp0\firmware\DB
@xcopy /y %~dp0\utils\flash_download.cfg %~dp0\firmware\DB >nul
@xcopy /y %~dp0\utils\mt2625_bootloader.bin %~dp0\firmware\DB  >nul
:cr4
@echo Making DB Version
@echo HWVER :=DB > %~dp0\utils\config.mk
@echo LINK_FILE:=link_option_cm.tmp >> %~dp0\utils\config.mk
@make
@goto exit1
:make_gb
@if  exist %~dp0\firmware\GB (goto cr5)
@md %~dp0\firmware\GB
@xcopy /y %~dp0\utils\flash_download.cfg %~dp0\firmware\GB >nul
@xcopy /y %~dp0\utils\mt2625_bootloader.bin %~dp0\firmware\GB  >nul
:cr5
@echo Making GB Version
@echo HWVER :=GB > %~dp0\utils\config.mk
@echo LINK_FILE:=link_option_cm.tmp >> %~dp0\utils\config.mk
@make
@goto exit1
:make_clean
@make  clean
@set path=%temp_path%
:exit1







