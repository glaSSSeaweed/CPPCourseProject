#pragma once
#include <string>
struct Basic_Stats {
	unsigned char* max;
	unsigned char* min;
	double* mean;
	double* stdev;
};
struct Hist {
	int DN;
	unsigned int** Npts;
	unsigned int** Total;
	double** Percent;
	double** AccPct;
};
class CRSImage
{
public:
	CRSImage();
	~CRSImage();

	// Operations
	bool Open(const char* lpstrPath);
public:
	bool	ReadMetaData(const char* lpstrMetaFilePath);
	bool	InitBuffer(void);
	bool	ReadImgData(const char* lpstrImgFilePath);
	void Max(void);
	void Min(void);
	void Mean(void);
	void Stdev(void);
	void Histogram(void);
	int GetBands();		//波段
	int GetSamples();	//列
	int GetLines();		//行
	unsigned char***  GetDataBuffer();
	struct Basic_Stats* bs;
	struct Hist* hs;
public:
	unsigned char*** m_pppData;	//三维数组
	int				m_nBands;		//波段数
	int				m_nLines;		//行数
	int				m_nSamples;		//列数
	std::string m_eInterleave;
	std::string m_nDataType;
    bool FileStatus;

};

