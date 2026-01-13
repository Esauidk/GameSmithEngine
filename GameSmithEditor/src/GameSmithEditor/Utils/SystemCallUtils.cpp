#include "gepch.h"
#include "SystemCallUtils.h"

#include "GameSmithEngine/Core/Log.h"

#ifdef GE_PLATFORM_WINDOWS

#include <wrl.h>
#include <shlobj_core.h>
#include <codecvt>

#endif // GE_PLATFORM_WINDOWS



namespace GameSmithEditor {
	bool PickFolderDialog(std::string path, std::string* outPath)
	{
#ifdef GE_PLATFORM_WINDOWS
		Microsoft::WRL::ComPtr<IFileDialog> pFileDialog;

		// Create the file open dialog
		bool res = FAILED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog)));
		GE_APP_ASSERT(!res, "Failed to create folder selection dialog box");

		pFileDialog->SetOptions(FOS_PICKFOLDERS);


		std::wstring convertPath(path.begin(), path.end());
		Microsoft::WRL::ComPtr<IShellItem> pShellItem;
		res = FAILED(SHCreateItemFromParsingName(convertPath.c_str(), NULL, IID_PPV_ARGS(&pShellItem)));

		pFileDialog->SetFolder(pShellItem.Get());
		HRESULT code = pFileDialog->Show(NULL);
		GE_APP_ASSERT(code == HRESULT_FROM_WIN32(ERROR_CANCELLED) || !FAILED(code), "Failed to show folder selection dialog box");

		Microsoft::WRL::ComPtr<IShellItem> pPickedFolder;
		pFileDialog->GetResult(&pPickedFolder);

		if (pPickedFolder.Get() == nullptr) {
			return false;
		}

		wchar_t* filepath;
		res = FAILED(pPickedFolder->GetDisplayName(SIGDN_FILESYSPATH, &filepath));
		GE_APP_ASSERT(!res, "Failed to get result folder name");

		std::wstring_convert<std::codecvt_utf8<wchar_t>> wtosConvert;
		std::wstring resultPath(filepath);
		*outPath = wtosConvert.to_bytes(resultPath);
	
		return true;

#endif // GE_PLATFORM_WINDOWS

	}

	std::string PickFileDialog(std::string path, std::string fileType, std::string filePrefix)
	{
#ifdef GE_PLATFORM_WINDOWS
		Microsoft::WRL::ComPtr<IFileDialog> pFileDialog;

		// Create the file open dialog
		bool res = FAILED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog)));
		GE_APP_ASSERT(!res, "Failed to create folder selection dialog box");

		
		pFileDialog->SetOptions(FOS_STRICTFILETYPES);

		std::wstring convertType(fileType.begin(), fileType.end());
		std::string regex = std::format("*.{0}", filePrefix);
		std::wstring convertPrefix(regex.begin(), regex.end());
		COMDLG_FILTERSPEC filter;
		filter.pszName = convertType.c_str();
		filter.pszSpec = convertPrefix.c_str();

		pFileDialog->SetFileTypes(1, &filter);

		std::wstring convertPath(path.begin(), path.end());
		Microsoft::WRL::ComPtr<IShellItem> pShellItem;
		res = FAILED(SHCreateItemFromParsingName(convertPath.c_str(), NULL, IID_PPV_ARGS(&pShellItem)));

		pFileDialog->SetFolder(pShellItem.Get());
		HRESULT code = pFileDialog->Show(NULL);
		GE_APP_ASSERT(code == ERROR_CANCELLED || !FAILED(code), "Failed to show folder selection dialog box");

		Microsoft::WRL::ComPtr<IShellItem> pPickedFolder;
		pFileDialog->GetResult(&pPickedFolder);

		wchar_t* filepath;
		res = FAILED(pPickedFolder->GetDisplayName(SIGDN_FILESYSPATH, &filepath));
		GE_APP_ASSERT(!res, "Failed to get result folder name");

		std::wstring_convert<std::codecvt_utf8<wchar_t>> wtosConvert;
		std::wstring resultPath(filepath);
		std::string convertResult = wtosConvert.to_bytes(resultPath);

		return convertResult;

#endif // GE_PLATFORM_WINDOWS
		return std::string();
	}

	std::string CreateFileDialog(std::string path, std::string fileType, std::string filePrefix)
	{
#ifdef GE_PLATFORM_WINDOWS
		Microsoft::WRL::ComPtr<IFileDialog> pFileDialog;

		// Create the file open dialog
		bool res = FAILED(CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog)));
		GE_APP_ASSERT(!res, "Failed to create folder selection dialog box");


		pFileDialog->SetOptions(FOS_STRICTFILETYPES | FOS_CREATEPROMPT);

		std::wstring convertType(fileType.begin(), fileType.end());
		std::string regex = std::format("*.{0}", filePrefix);
		std::string startingFilename = std::format("{0}.{1}", fileType, filePrefix);
		std::wstring convertPrefix(regex.begin(), regex.end());
		std::wstring convertFilename(startingFilename.begin(), startingFilename.end());
		COMDLG_FILTERSPEC filter;
		filter.pszName = convertType.c_str();
		filter.pszSpec = convertPrefix.c_str();

		pFileDialog->SetFileTypes(1, &filter);
		pFileDialog->SetFileName(convertFilename.c_str());

		std::wstring convertPath(path.begin(), path.end());
		Microsoft::WRL::ComPtr<IShellItem> pShellItem;
		res = FAILED(SHCreateItemFromParsingName(convertPath.c_str(), NULL, IID_PPV_ARGS(&pShellItem)));

		pFileDialog->SetFolder(pShellItem.Get());
		HRESULT code = pFileDialog->Show(NULL);
		GE_APP_ASSERT(code == ERROR_CANCELLED || !FAILED(code), "Failed to show folder selection dialog box");

		Microsoft::WRL::ComPtr<IShellItem> pPickedFolder;
		pFileDialog->GetResult(&pPickedFolder);

		wchar_t* filepath;
		res = FAILED(pPickedFolder->GetDisplayName(SIGDN_FILESYSPATH, &filepath));
		GE_APP_ASSERT(!res, "Failed to get result folder name");

		std::wstring_convert<std::codecvt_utf8<wchar_t>> wtosConvert;
		std::wstring resultPath(filepath);
		std::string convertResult = wtosConvert.to_bytes(resultPath);

		return convertResult;

#endif // GE_PLATFORM_WINDOWS
		return std::string();
	}


};

