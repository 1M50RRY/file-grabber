#include "Utils.h"

char *HWID()
{
	HKEY hKey;
	DWORD cData = 255;
	TCHAR MachineGuid[255] = { '\0' };

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", NULL, KEY_READ | KEY_WOW64_64KEY, &hKey) == ERROR_SUCCESS)
		RegQueryValueEx(hKey, "MachineGuid", NULL, NULL, (LPBYTE)MachineGuid, &cData);
	RegCloseKey(hKey);

	return MachineGuid;
}

vector<string> ListDirectory(string path, string mask)
{
	WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    string sPath;
    vector<string> folders;
	
    sPath.assign(path+mask); // Mask should be regex (*.txt)
    hFind = FindFirstFile(sPath.data(), &FindFileData);
    do
    {
		folders.push_back(FindFileData.cFileName);
    }
	while (FindNextFile(hFind, &FindFileData));
    FindClose(hFind);
	
    return folders;
}

void ZipFolder(string name, string path)
{
	HZIP hz = (HZIP)CreateZip(name.c_str(), 0);
	vector<string> start = ListDirectory(path, "\\*"); // List everything in directory
	
	for(int i = 0; i < start.size(); i++)
	{
		if (start[i] !=  "." && start[i] != "..") //Skip upper dirs
		{
			string realPath = path + "\\" + start[i]; //Full path to file/directory
			ZipAdd(hz, (char*)start[i].c_str(), (char*)realPath.c_str()); //Add to archive if it is file

			vector<string> folders = ListDirectory(realPath, "\\*"); //Go inside the dir 
			for(int j = 0; j < folders.size(); j++)
			{
				if (folders[j] !=  "." && folders[j] != "..")
				{
					string zipPath = start[i] + "\\" + folders[j];
					realPath = path + "\\" + start[i] + "\\" + folders[j];
					ZipAdd(hz, (char*)zipPath.c_str(), (char*)realPath.c_str());
					
					vector<string> folders2 = ListDirectory(realPath, "\\*"); //Go one step deeper
					for(int k = 0; k < folders2.size(); k++)
					{
						if (folders2[k] !=  "." && folders2[k] != "..")
						{
							string zipPath = start[i]  + "/" + folders[j] + "/" + folders2[k];
							realPath = path + "\\" + start[i] + "\\" + folders[j]+  "\\" + folders2[k];
							ZipAdd(hz, (char*)zipPath.c_str(), (char*)realPath.c_str());
						}
					}
				}
			}
		}	
	}
	CloseZip(hz);
}

char* GetTempPath()
{
	char *tmpPath = (char*)malloc(MAX_PATH);
	strcpy(tmpPath, getenv("Temp"));
	strcat(tmpPath, "\\");
	strcat(tmpPath, HWID());
	return tmpPath;
}

BOOL FileExists(char *path)
{
	struct stat buffer;   
	return (stat (path, &buffer) == 0);
}