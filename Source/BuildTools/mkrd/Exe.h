#ifndef EXE_H
#define EXE_H
#pragma pack(push, 1)

typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;

typedef struct MZHDR {
    u16 e_magic;
    u16 e_cblp;
    u16 e_cp;
    u16 e_crlc;
    u16 e_cparhdr;
    u16 e_minalloc;
    u16 e_maxalloc;
    u16 e_ss;
    u16 e_sp;
    u16 e_csum;
    u16 e_ip;
    u16 e_cs;
    u16 e_lsarlc;
    u16 e_ovno;
    u16 e_res[4];
    u16 e_oemid;
    u16 e_oeminfo;
    u16 e_res2[10];
    u32 e_lfanew;
} MZHDR;

typedef struct PEHDR {
	u32 PeSig;
    u16 Machine;
    u16 Sections;
    u32 TimeStamp;
    u32 PtrSymbols;
    u32 NumSymbols;
    u16 SizeOpHdr;
    u16 Characteristics;
} PEHDR;

typedef struct OPHDR {
    u16 Magic;
    u8  MajorLinker;
    u8  MinorLinker;
    u32 SizeCode;
    u32 SizeInitialized;
    u32 SizeUninitialized;
    u32 Entry;
    u32 CodeBase;
    u32 DataBase;
    u32 ImageBase;
    u32 SectAlign;
    u32 FileAlign;
    u16 MajorOs;
    u16 MinorOs;
    u16 MajorImage;
    u16 MinorImage;
    u16 MajorSubSystem;
    u16 MinorSubSystem;
    u32 Win32Version;
    u32 ImageSize;
    u32 HeaderSize;
    u32 CheckSum;
    u16 SubSystem;
    u16 DllCharacteristics;
    u32 SizeStackReserve;
    u32 SizeStackCommit;
    u32 SizeHeapReserve;
    u32 SizeHeapCommit;
    u32 LoaderFlags;
    u32 NumRvas;
} OPHDR;

typedef struct PESECT {
    char Name[8];
    u32 VirtSize;
    u32 VirtAddress;
    u32 SizeRawData;
    u32 PtrRawData;
    u32 PtrRelocs;
    u32 PtrLineNumbers;
    u16 NumRelocs;
    u16 NumLineNumbers;
    u32 Characteristics;
} PESECT;

typedef struct PEDIR {
    u32 ExportAddress;
    u32 ExportSize;
    u32 ImportAddress;
    u32 ImportSize;
    u32 ResourceAddress;
    u32 ResourceSize;
    u32 ExcepAddress;
    u32 ExcepSize;
    u32 CertAddress;
    u32 CertSize;
    u32 RelocAddress;
    u32 RelocSize;
    u32 DebugAddress;
    u32 DebugSize;
    u32 ArchAddress;
    u32 ArchSize;
    u32 GprAddress;
    u32 GprSize;
    u32 TlsAddress;
    u32 TlsSize;
    u32 ConfigAddress;
    u32 ConfigSize;
    u32 BoundAddress;
    u32 BoundSize;
    u32 ImportTableAddress;
    u32 ImportTableSize;
    u32 DelayAddress;
    u32 DelaySize;
    u32 ClrAddress;
    u32 ClrSize;
    u32 ReserveAddress;
    u32 ReserveSize;
} PEDIR;

typedef struct PEEXPORT {
    u32 Characteristics;
    u32 TimeStamp;
    u16 MajorVersion;
    u16 MinorVersion;
    u32 Name;
    u32 Base;
    u32 NumFunctions;
    u32 NumNames;
    u32 PtrFunctions;
    u32 PtrNames;
    u32 PtrOrdinals;
} PEEXPORT;

typedef struct PEIMPORT {
    u32 Characteristics;
    u32 TimeStamp;
    u32 ForwardChain;
    u32 Name;
    u32 FirstThunk;
} PEIMPORT;

typedef struct PEIMPORTNAME {
    u16 Hint;
    char Name[1];
} PEIMPORTNAME;

typedef struct PERELOC {
    u32 Address;
    u32 BlockSize;
} PERELOC;

#pragma pack(pop)
#endif // EXE_H