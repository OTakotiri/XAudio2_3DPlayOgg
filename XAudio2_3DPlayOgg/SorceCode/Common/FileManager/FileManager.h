#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
class CFileManager
{
public:
	CFileManager();
	~CFileManager();

	// バイナリデータを読み込む.
	template <class In>
	static HRESULT LoadBinary(const std::string Path, In& InData)
	{
		std::ifstream fin(Path.c_str(), std::ios::in | std::ios::binary);

		if (!fin) {
			return E_FAIL;
		}
		// ファイルを読み込む.
		fin.read(reinterpret_cast<char*>(std::addressof(InData)), sizeof(In));

		fin.close();  //ファイルを閉じる
		return S_OK;
	};
	// バイナリデータを作成する.
	template <class Out>
	static HRESULT CreateBinary(const std::string Path, Out OutData)
	{
		std::ofstream fout;

		fout.open(Path.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

		if (!fout) {
			return E_FAIL;
		}

		fout.write(reinterpret_cast<char*>(std::addressof(OutData)), sizeof(Out));
		fout.close();  //ファイルを閉じる

		return S_OK;
	};

	// テキストファイルの読み込み.
	static std::vector<std::string> TextLoading(const std::string& fileName, char delimiter = ',');

	// テキストファイルの書き出し.
	static void TextOutPut(const std::string& fileName,const std::string& Char);
private:

};


#endif // !FILEMANAGER_H
