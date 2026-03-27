//Written By Rhezaagit

#include <processthreadsapi.h>
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include "vorbis_data.h"

//log so you can see Log Of the Modloader 
void LogMod(const char* text) {
    FILE* f = fopen("SimpleAsiLoader.log", "a");
    if (f) {
        fprintf(f, "%s\n", text);
        fclose(f);
    }
}

//Iterate Every asi and dll file to run loadlibraryA
void LoadAsi(const char* folderPath) {
    WIN32_FIND_DATA data;

    char paths[256] = {0}, Logs[256] = {0};
    sprintf(paths, "%s/*.asi", folderPath);
    HANDLE HFind = FindFirstFile(paths, &data);

    if (HFind != INVALID_HANDLE_VALUE) {
        do {
            strset(paths, 0);
            sprintf(paths, "%s/%s", folderPath, data.cFileName);

            strset(Logs, 0);
            LoadLibraryA(paths) ? 
                sprintf(Logs, "Success Loading %s", data.cFileName) : 
                sprintf(Logs, "Failed Loading %s", data.cFileName);

            LogMod(Logs);
        } while (FindNextFile(HFind, &data));
    }

    FindClose(HFind);
}

//Dont Think To Much About This Just create The Original vorbisFileHooked.dll That GTA SA Needed
void CreateOriginal() {
    FILE* check = fopen("vorbisFileHooked.dll", "rb");
    if (check) {
        fclose(check);
        return;    
    }

    FILE* f = fopen("vorbisFileHooked.dll", "wb");
    if (f) {
        fwrite(vorbisHooked_dll, 1, vorbisHooked_dll_len, f);
        fclose(f);
        ExitProcess(0); // the GTA SA still not load the dll yet so you need run it again
    }
}

// Clear The Log File Buffer
void ResetLog() {
	FILE* f = fopen("SimpleAsiLoader.log", "w");
	if (f) {
		fwrite(NULL, 1, 0, f);
        fclose(f);
	}
}

//The Main Function start When The Loader Begin 
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) { 
    if (fdwReason == DLL_PROCESS_ATTACH) {
    	ResetLog();
    
        CreateOriginal();
        LoadAsi("./");
        LoadAsi("./scripts/");
    }
    return TRUE;
}
