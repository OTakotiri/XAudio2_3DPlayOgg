#include "FileManager.h"

CFileManager::CFileManager()
{
}

CFileManager::~CFileManager()
{
}
// �e�L�X�g�t�@�C���̓ǂݍ���.
std::vector<std::string> CFileManager::TextLoading(const std::string& fileName, char delimiter)
{
	std::vector<std::string> readList;	// �ǂݍ��݃��X�g.

										// �t�@�C�����J��.
	std::fstream fileStream(fileName);
	// �t�@�C�����J���Ȃ��ꍇ.
	if (!fileStream.is_open()) {
		_ASSERT_EXPR(false, "�e�L�X�g�t�@�C�� | " + fileName + " | ���J���܂���ł���");
		return readList;
	}

	std::string line;	// 1�s����ǂݍ��ނ��߂̕���.

	// ��s�Âǂݍ���.
	// �e�L�X�g�t�@�C����SJIS�ŕۑ�����.
	while (std::getline(fileStream, line)) {
		// �擾���������� '//' ������΂�蒼��.
		if (line.find("//") != std::string::npos) continue;

		std::string buff = "";				// �������ꎞ�I�Ɋi�[����.
		std::istringstream stream(line);

		// �f�t�H���g ',' ��؂�ŕ������擾.
		while (std::getline(stream, buff, delimiter)) {
			// �擾���������� '#' ������΂�蒼��.
			if (buff.find('#') != std::string::npos) continue;

			// �ǂݍ��݃��X�g�ɒǉ�.
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
