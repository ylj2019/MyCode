
#pragma once
#include"haffman.hpp"
#include<iostream>
#include<vector>
using namespace std;
#include<assert.h>

typedef long long LongType;
//数组内的元素类型
struct CharInfo
{
	char _ch;//字符
	LongType _count;//次数
	string  _code;//编码

	bool operator !=(const CharInfo&info)
	{
		return _count != info._count;
	}
	CharInfo operator+(const CharInfo&info)
	{
		CharInfo ret;
		ret._count = _count + info._count;
		return ret;
	}
	bool operator<(const CharInfo&info)
	{
		return _count > info._count;
	}
};

class FileCompress
{
	typedef HuffmanTreeNode<CharInfo> Node;
	struct TmpInfo//辅助文件：数组元素类型
	{
		char _ch;//字符
		LongType _count;//次数
	};
public:
	//构造函数
	FileCompress()
	{
		for (size_t i = 0; i < 256; ++i)
		{
			_infos[i]._ch = i;
			_infos[i]._count = 0;
		}
	}
	//获取哈夫曼编码
	void GenerateHuffmanCode(Node*root,string code)
	{
		if (root == NULL)
			return;
		//前序遍历生成编码
		if (root->_pLeft == NULL&&root->_pRight == NULL)
		{
			_infos[(unsigned char )root->_weight._ch]._code = code;
			return;
		}
		GenerateHuffmanCode(root->_pLeft, code+'0');
		GenerateHuffmanCode(root->_pRight, code + '1');
	}
	void Compress(const char *file)//file:源文件
	{
		//1.统计字符出现的次数
		FILE*fout = fopen(file, "rb");
		assert(fout);
		char ch = fgetc(fout);
		while (ch != EOF||feof(fout)==0)//如文件结束，则返回值为1，否则为0
		{
			//无符号
			_infos[(unsigned char)ch]._count++;
			ch = fgetc(fout);
		}

		//2.生成Huffmantree 及code
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo>tree(_infos, 256, invalid);//参数：数组，256个，无效值（出现0次）
		
		string compressfile = file;
		compressfile += ".huffman";
		FILE*fin = fopen(compressfile.c_str(),"wb");//打开压缩文件，
		//https://zhidao.baidu.com/question/104592558.html
		assert(fin);

		string code;
		GenerateHuffmanCode(tree.GetRoot(), code);
		
		//3.0写入字符出现的信息:辅助文件，为了解压缩
		int writeNum = 0;
		int objSize = sizeof(TmpInfo);
		for (rsize_t i = 0; i < 256; ++i)
		{
			if (_infos[i]._count>0)
			{
				TmpInfo info;
				info._ch = _infos[i]._ch;
				info._count = _infos[i]._count;
				//把info._ch、info._count写入压缩文件
				fwrite(&info, objSize, 1, fin);//每一次sizeof(objSize),时间消耗大
				writeNum++;
			}
		}
		//写入压缩文件：info._count=-1(出现次数)作为结束标志，区分次数和正文
		TmpInfo info;
		info._count = -1;
		fwrite(&info, objSize, 1, fin);

		//3.1压缩正文
		//重定位流上的文件指针到开始
		fseek(fout, 0, SEEK_SET);//文件指针、偏移量、参照位置
			ch = fgetc(fout);
			char value = 0;
			size_t pos = 0;
			while (ch != EOF)
			{
				string &code = _infos[(unsigned char)ch]._code;
				for (size_t i = 0; i < code.size(); ++i)
				{
					if (code[i] == '1')
						value |= (1<<pos);
					else if (code[i] == '0')
					{
						value &= ~(1<<pos);
					}
					else
					{
						assert(false);
					}
					++pos; 
					if (pos == 8)
					{
						fputc(value, fin);
						value = 0;
						pos = 0;
					}
				}
				ch = fgetc(fout);
			}
			if (pos > 0)//不够8位，直接写进去，后面根节大小（总字符个数）保底
			{
				fputc(value, fin);//写入压缩文件（fin）
			}
			fclose(fout);
			fclose(fin);
	}

	void uncompress(const char *file)
	{
		string uncompressfile = file;//file:Input.txt.huffman
		size_t pos = uncompressfile.rfind('.');//找到倒数第一个'.'
		assert(pos != string::npos);
		uncompressfile.erase(pos);//删除掉'.'后面字符串
		uncompressfile += ".unhuffman";//Input.txt+'.unhuffman'
		FILE*fin = fopen(uncompressfile.c_str(), "wb");//打开解压缩文件
		assert(fin);
		FILE*fout = fopen(file, "rb");//打开压缩文件
		assert(fout);
		//1.0读入字符出现的信息，存放在TmpInfo结构体数组中
		TmpInfo info;
		int objSize = sizeof(TmpInfo);
		fread(&info, objSize, 1, fout);
		
		while (info._count != -1)//-1为结束标志
		{

			_infos[(unsigned char)info._ch]._ch = info._ch;
			_infos[(unsigned char)info._ch]._count= info._count;

			fread(&info, objSize, 1, fout);

		}

		//2.0根据_infos结构体数据，重建huaffman树
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo>tree(_infos, 256, invalid);//参数：数组，256个，无效值（出现0次）
		Node *root = tree.GetRoot();
		Node*cur = root;
		LongType n = root->_weight._count;//所有叶子节点的和（源文件字符的个数）
		char ch = fgetc(fout);//从fout(压缩文件)读字符
		while (ch != EOF||n>0)
		{
			for (size_t i = 0; i < 8; ++i)
			{
				
				if ((ch&(1 << i)) == 0)
					cur = cur->_pLeft;
				else
					cur = cur->_pRight;
				if (cur->_pLeft == NULL&&cur->_pRight == NULL)
				{
					//cout << cur->_weight._ch;
					fputc(cur->_weight._ch, fin);//fin解压缩文件
					cur = root;
					if (--n == 0)
						break;
				}
				
			}
			ch = fgetc(fout);
		}
		fclose(fin);
		fclose(fout);

	}
protected:
	CharInfo _infos[256];
};
void TestFileCompress()
{

	FileCompress fc1;
	fc1.Compress("Input.txt");
	fc1.uncompress("Input.txt.huffman");

}
