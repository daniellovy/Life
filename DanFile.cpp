#include "stdafx.h"
#include "Windowsx.h"

#include <windows.h>
#include <shobjidl.h> 
#include "DanFile.h"

DanFile::DanFile()
{
	fileopen = FALSE; 
	FilePath = (TCHAR *)GlobalAlloc(GPTR, MAXPATH * sizeof(TCHAR));
}

bool DanFile::SaveDialog()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	bool retval = FALSE;
	if (SUCCEEDED(hr))
	{

		//IFileOpenDialog *pFileOpen;
		IFileSaveDialog *pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
			IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						//MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
						OpenStreamWrite(pszFilePath);
						CoTaskMemFree(pszFilePath);
						retval = TRUE;
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	return retval;
}


bool DanFile::OpenDialog()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	bool retval = FALSE;
	if (SUCCEEDED(hr))
	{

		IFileOpenDialog *pFileOpen;
		//IFileSaveDialog *pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						//MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
						OpenStreamRead(pszFilePath);
						CoTaskMemFree(pszFilePath);

						retval = TRUE;
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	return retval;
}

bool DanFile::OpenStreamWrite(TCHAR *openpath)
{
	_tcscpy_s(FilePath, MAXPATH,openpath);
	


	// creating, opening and writing/appending data to file
	outputfile.open(FilePath, ios::out | ios::trunc);
	// simple error handling for file creating/opening for writing
	// test if fail to open the file, do…
	if (outputfile.fail())
	{
		MessageBox(NULL, FilePath, L"File Path Failed to Open", MB_OK);
		fileopen = FALSE;
		return FALSE;
		// 0-normal, non zero - some error
	}
	//MessageBox(NULL, FilePath, L"File Path OPENED", MB_OK);
	fileopen = TRUE;
	return TRUE;
}

bool DanFile::OpenStreamRead(TCHAR *openpath)
{
	_tcscpy_s(FilePath, MAXPATH, openpath);

	// creating, opening and writing/appending data to file
	inputfile.open(FilePath);

	// simple error handling for file creating/opening for writing
	// test if fail to open the file, do…
	if (inputfile.fail())
	{
		MessageBox(NULL, FilePath, L"File Path Failed to Open", MB_OK);
		fileopen = FALSE;
		return FALSE;
		// 0-normal, non zero - some error
	}
	//MessageBox(NULL, FilePath, L"File Path OPENED", MB_OK);
	fileopen = TRUE;

	return TRUE;
}

bool DanFile::WriteStream(char *inputbuf)
{
	//MessageBox(NULL, inputbuf, L"Text about to write", MB_OK);

	outputfile << inputbuf;
	return TRUE;
}
void DanFile::ReadStreamLine(char *inbuff)
{
	//getline(outputfile, inbuff);
	//std::getline(inputfile, inbuff);
	//getline(inputfile, inbuff);
	//inputfile.get(chr);
	inputfile.getline(inbuff, MAX_FILESTRING);

	
	//MessageBox(NULL, L"about to read", L"Text about to read", MB_OK);

}
bool DanFile::CloseStream()
{
	if (fileopen)
	{
		outputfile.close();
		inputfile.close();
		fileopen = FALSE;
	}
	return TRUE;
}

bool DanFile::IsEOF()
{
	return inputfile.eof();
}

DanFile::~DanFile()
{
	if (fileopen)
	{
		outputfile.close();
		inputfile.close();
		fileopen = FALSE;
	}
	// FIX LATER
	//if (FilePath)
	//	delete[] FilePath;
}

/// Construct from UTF-16
cUTF::cUTF(const wchar_t * ws)
{
	// store copy of UTF16
	myString16 = (wchar_t *)malloc(wcslen(ws) * 2 + 2);
	wcscpy_s(myString16, wcslen(ws) * 2 + 2, ws);
	// How long will the UTF-8 string be
	int len = WideCharToMultiByte(CP_UTF8, 0,
		ws, wcslen(ws),
		NULL, NULL, NULL, NULL);
	// allocate a buffer
	myString8 = (char *)malloc(len + 1);
	// convert to UTF-8
	WideCharToMultiByte(CP_UTF8, 0,
		ws, wcslen(ws),
		myString8, len, NULL, NULL);
	// null terminate
	*(myString8 + len) = '\0';
}
///  Construct from UTF8
cUTF::cUTF(const char * s)
{
	myString8 = (char *)malloc(strlen(s) + 1);
	strcpy_s(myString8, strlen(s) + 1, s);
	// How long will the UTF-16 string be
	int len = MultiByteToWideChar(CP_UTF8, 0,
		s, strlen(s),
		NULL, NULL);
	// allocate a buffer
	myString16 = (wchar_t *)malloc(len * 2 + 2);
	// convert to UTF-16
	MultiByteToWideChar(CP_UTF8, 0,
		s, strlen(s),
		myString16, len);
	// null terminate
	*(myString16 + len) = L'\0';
}
