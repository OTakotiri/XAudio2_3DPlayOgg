#include "FileManager.h"

CFileManager::CFileManager()
{
}

CFileManager::~CFileManager()
{
}
// テキストファイルの読み込み.
std::vector<std::string> CFileManager::TextLoading(const std::string& fileName, char delimiter)
{
	std::vector<std::string> readList;	// 読み込みリスト.

										// ファイルを開く.
	std::fstream fileStream(fileName);
	// ファイルが開けない場合.
	if (!fileStream.is_open()) {
		_ASSERT_EXPR(false, "テキストファイル | " + fileName + " | が開けませんでした");
		return readList;
	}

	std::string line;	// 1行分を読み込むための文字.

	// 一行づつ読み込み.
	// テキストファイルはSJISで保存する.
	while (std::getline(fileStream, line)) {
		// 取得した文字に '//' があればやり直し.
		if (line.find("//") != std::string::npos) continue;

		std::string buff = "";				// 文字を一時的に格納する.
		std::istringstream stream(line);

		// デフォルト ',' 区切りで文字を取得.
		while (std::getline(stream, buff, delimiter)) {
			// 取得した文字に '#' があればやり直し.
			if (buff.find('#') != std::string::npos) continue;

			// 読み込みリストに追加.
			readList.emplace_back(buff);
		}
	}

	fileStream.close();

	return readList;
}

void CFileManager::TextOutPut(const std::string & fileName, const std::string & Char)
{
	std::string FilePath = "Data\\" + fileName + ".txt";

	std::ofstream outputfile(FilePath);
	outputfile << Char;
	outputfile.close();
}
