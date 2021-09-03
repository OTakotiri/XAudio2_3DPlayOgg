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

	// �o�C�i���f�[�^��ǂݍ���.
	template <class In>
	static HRESULT LoadBinary(const std::string Path, In& InData)
	{
		std::ifstream fin(Path.c_str(), std::ios::in | std::ios::binary);

		if (!fin) {
			return E_FAIL;
		}
		// �t�@�C����ǂݍ���.
		fin.read(reinterpret_cast<char*>(std::addressof(InData)), sizeof(In));

		fin.close();  //�t�@�C�������
		return S_OK;
	};
	// �o�C�i���f�[�^���쐬����.
	template <class Out>
	static HRESULT CreateBinary(const std::string Path, Out OutData)
	{
		std::ofstream fout;

		fout.open(Path.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

		if (!fout) {
			return E_FAIL;
		}

		fout.write(reinterpret_cast<char*>(std::addressof(OutData)), sizeof(Out));
		fout.close();  //�t�@�C�������

		return S_OK;
	};

	// �e�L�X�g�t�@�C���̓ǂݍ���.
	static std::vector<std::string> TextLoading(const std::string& fileName, char delimiter = ',');

	// �e�L�X�g�t�@�C���̏����o��.
	static void TextOutPut(const std::string& fileName,const std::string& Char);
private:

};


#endif // !FILEMANAGER_H
