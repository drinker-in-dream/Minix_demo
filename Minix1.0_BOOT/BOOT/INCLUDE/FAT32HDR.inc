
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               FAT32.inc
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                                                     Toby Du, 2013
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


; FAT32 Header 
; ----------------------------------------------------------------------

BS_OEMName				DB 'DOX_V1.0'	; OEM String, 8 byte must

BPB_BytsPerSec			DW 512		; Bytes in per sector
BPB_SecPerClus			DB 8		; Sectors Per cluster
BPB_ReservePerSec		DW 1140		; Reserved sectors
BPB_NumFATs				DB 2		; FAT table count
BPB_RootEntCnt			DW 0		; Root directory file amount
BPB_TotalSecs			DW 0		; Total Sectors (if more than 32MB use offset 0x20~0x23,so here should be 0 in my 4GB U Disk)
BPB_MediaDescribe		DB 248		; 248 stands for local disk
BPB_SecsPerFATTable		DW 0		; Sectors per FAT Table (for FAT32 should be 0)
BPB_SecPerTrk			DW 63		; Sectors per track
BPB_NumHeads			DW 255		; Header amount
BPB_SecBeforeFormat		DD 63		; Sectors used before format ** care about this value **
BPB_FsSecNum			DD 7757C1h	; File System Sector amount
BPB_SecsPerFATTable32	DD 1DC6h	; Sectors per FAT Table
BPB_Marks				DW 0		; Mark
BPB_Version				DW 0		; Version
BPB_RootDirSec			DD 2		; Root dirctory cluster number
BPB_FSINFO				DW 1		; File system information sector
BPB_BootSecBackUp		DW 6		; Boot sector backup sector location
						DD 0		; Reserved
						DD 0		; Reserved
						DD 0		; Reserved
BPB_BIOSint13DevNum		DB 80h		; BIOS int 13 device number
						DB 0		; Reserved
BPB_ExBootFlag			DB 29h		; extend boot flag
BPB_VolumeNum			DD 7AACF2AAh   ; Volume Number
BPB_VolumeLable			DB 'TOBY_DOX    ' ;Volume Lable
BPB_FileSystemtype		DB 'FAT32   '	  ; Filesystem type
