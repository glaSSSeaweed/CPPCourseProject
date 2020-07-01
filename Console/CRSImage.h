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
	int GetBands();		//����
	int GetSamples();	//��
	int GetLines();		//��
	unsigned char***  GetDataBuffer();
	struct Basic_Stats* bs;
	struct Hist* hs;
public:
	unsigned char*** m_pppData;	//��ά����
	int				m_nBands;		//������
	int				m_nLines;		//����
	int				m_nSamples;		//����
	std::string m_eInterleave;
	std::string m_nDataType;

};

