#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <fstream>  
#include "windows.h"
#include "wincon.h"
#include "CRSImage.h"
//extern "C" HWND WINAPI GetConsoleWindow();
using namespace std;

void DisplayImage(CRSImage* pRSImg);
void ScaleImage(CRSImage* pRSImg,float k);
void RotateImage(CRSImage* pRSImg, float d);
void FilterImage(CRSImage* pRSImg ,struct matrix* mat);
void Usage();

struct matrix{
	float a11, a12, a13;
	float  a21, a22, a23;
	float  a31, a32, a33;
};

//int main() {
//	//matrix x = {1/16.0,1/8.0,1/16.0,1/8.0,1/4.0,1 / 8.0,1 / 16.0,1 / 8.0,1 / 16.0 };
//	//matrix x1 = { 0,1,0,0,-4,0,0,1,0 };
//	//matrix x2 = {1,1,1,1,-8,1,1,1,1};
//	//matrix x3 = { 0,-1,0,-1,5,-1,0,-1,0 };
//	CRSImage test;
//	test.Open("test.img");
//	//test.Max(); 
//	//test.Min();
//	//test.Mean();
//	//test.Stdev();
//	//test.Histogram();
//	//DisplayImage(&test);
//	//ScaleImage(&test,0.9);
//	//RotateImage(&test,0.5);
//	//FilterImage(&test,&x);
//	//FilterImage(&test, &x1);
//	//FilterImage(&test, &x3);
//	return 0;
//}

int main()
{
	matrix x;
	char cCmd = 0;
	char cPath[301]{0};
	bool FileStatus = false;
	float k = 1;
	float R = 0;
	int filtertype = 0;
	CRSImage test;
	
    Usage();
    do
    {
        cin >> cCmd;
		cin.get();
		system("cls");
        switch (cCmd)
        {
		case 'C':
		case 'c':
			if (FileStatus == true) {
				test.~CRSImage();
				cout << "Menu Close." << endl;
				FileStatus = false;
			}
			else {
				cout << "��ǰû��ͼ�񱻴�" << endl;
			}
			break;
		case 'D':
		case 'd':
			if (FileStatus == true) {
				system("cls");
				DisplayImage(&test);
			}
			else {
				cout << "��ǰû��ͼ�񱻴�" << endl;
			}
			break;
		case 'F':
		case 'f':
			if (FileStatus == true) {
				cin >> filtertype;
				switch (filtertype) {
				case 1:
					x = { 1 / 16.0,1 / 8.0,1 / 16.0,1 / 8.0,1 / 4.0,1 / 8.0,1 / 16.0,1 / 8.0,1 / 16.0 };
					FilterImage(&test, &x);
					break;
				case 2:
					x = { 0,1,0,0,-4,0,0,1,0 };
					FilterImage(&test, &x);
					break;
				case 3:
					x = { 1,1,1,1,-8,1,1,1,1 };
					FilterImage(&test, &x);
					break;
				case 4:
					x = { 0,1,0,1,-4,1,0,1,0 };
					FilterImage(&test, &x);
					break;
				default:
					cout << "�˲������������" << endl;
				}

			}
			else {
				cout << "��ǰû��ͼ�񱻴�" << endl;
			}
			break;
		case 'H':
		case 'h':
			if (FileStatus == true) {
				test.Histogram();
				cout << setw(12) <<
					"Histogram" << "\t"
					<< setw(10) << "DN" << "\t"
					<< setw(10) << "Npts" << "\t"
					<< setw(10) << "Total" << "\t"
					<< setw(10) << "Percent" << "\t"
					<< setw(10) << "Acc Pct" << "\t" << endl;
				for (int i = 0; i < test.GetBands(); ++i) {
					for (int j = 0; j < (test.hs->DN + 1); ++j) {
						if (j == 0) {
							cout << setw(12) << "Band " << i + 1 << "\t";
						}
						else {
							cout << setw(12) << " " << "\t";
						}
						cout << setw(10) << j << "\t"
							<< setw(10) << (int)test.hs->Npts[i][j] << "\t"
							<< setw(10) << (int)test.hs->Total[i][j] << "\t"
							<< setw(10) << test.hs->Percent[i][j] << "\t"
							<< setw(10) << test.hs->AccPct[i][j] << endl;
					}

				}

			}
			else {
				cout << "��ǰû��ͼ�񱻴�" << endl;
			}
			break;
		case 'I':
		case 'i':
			if(FileStatus == true){
				cout << "bands "	<<test.m_nBands << endl
					<< "lines " << test.m_nLines << endl
					<< "samples " << test.m_nSamples << endl
					<< "interleave " << test.m_eInterleave << endl
					<< "data type " << test.m_nDataType<<endl;
			}
			else {
				cout << "û��ͼ���ļ�����" << endl;
			}
			break;
		case 'O':
		case 'o':
			cout << "������.img�ļ�·��" << endl;
			cin.getline(cPath, 300);
			if (FileStatus = test.Open(cPath)) {
				cout << "Menu Open." << endl;
			}
			break;
		case 'R':
		case 'r':
			if (FileStatus == true) {
				cout << "��������ת�Ƕ�(����)";
				cin >> k;
				system("cls");
				RotateImage(&test, k);
			}
			else {
				cout << "��ǰû��ͼ�񱻴�" << endl;
			}
			break;
		case 'S':
		case 's':
			if (FileStatus == true) {
				test.Min();
				test.Max();
				test.Mean();
				test.Stdev();
				cout << setw(12) << 
					"Basic Stats"<<"\t"
					<< setw(10) <<"Min"<<"\t"
					<< setw(10) <<"Max"<<"\t"
					<< setw(10) <<"Mean"<<"\t"
					<< setw(10) <<"Stdev" << endl;
				for(int i = 0;i< test.GetBands();++i){
					cout << setw(12) <<
						"Band "<<i+1<< "\t"
						<< setw(10) << (int)test.bs->min[i] << "\t"
						<< setw(10) << (int)test.bs->max[i] << "\t"
						<< setw(10) << test.bs->mean[i] << "\t"
						<< setw(10) << test.bs->stdev[i] << endl;
				}

			}
			else {
				cout << "û��ͼ���ļ�����" << endl;
			}
			break;
		case 'W':
		case 'w':
			if (FileStatus == true && (test.hs->Npts !=NULL) && (test.bs->max != NULL)) {
				ofstream ofs;
				ofs.open("data.doc", ios::out);
				ofs << setw(10) <<
					"Basic Stats" 
					<< setw(10) << "Min" << "\t"
					<< setw(10) << "Max" << "\t"
					<< setw(10) << "Mean" << "\t"
					<< setw(10) << "Stdev" << endl;
				for (int i = 0; i < test.GetBands(); ++i) {
					ofs << setw(10) <<
						"Band " << i + 1
						<< setw(10) << (int)test.bs->min[i] << "\t"
						<< setw(10) << (int)test.bs->max[i] << "\t"
						<< setw(10) << test.bs->mean[i] << "\t"
						<< setw(10) << test.bs->stdev[i] << endl;
				}
				ofs << setw(10) <<
					"Histogram" 
					<< setw(10) << "DN" << "\t"
					<< setw(10) << "Npts" << "\t"
					<< setw(10) << "Total" << "\t"
					<< setw(10) << "Percent" << "\t"
					<< setw(10) << "Acc Pct" << "\t" << endl;
				for (int i = 0; i < test.GetBands(); ++i) {
					for (int j = 0; j < (test.hs->DN + 1); ++j) {
						if (j == 0) {
							ofs << setw(10) << "Band " << i + 1;
						}
						else {
							ofs << setw(10) << " " ;
						}
						ofs << setw(10) <<j << "\t"
							<< setw(10) << (int)test.hs->Npts[i][j] << "\t"
							<< setw(10) << (int)test.hs->Total[i][j] << "\t"
							<< setw(10) << test.hs->Percent[i][j] << "\t"
							<< setw(10) << test.hs->AccPct[i][j] << endl;
					}

				}
				ofs.close();
			}
			else {
				cout << "��ǰû��ͼ�񱻴� �� ��H\\Sָ��û�ж���ִ�У�" << endl;
			}
			break;
        case 'X':
        case 'x':
            cout << "Menu Exit." << endl;
            break;
		case 'Z':
		case 'z':
			if (FileStatus == true) {
				cout << "�����������|(0��2)֮���С��|";
				cin >> k;
				system("cls");
				ScaleImage(&test, k);
			}
			else {
				cout << "��ǰû��ͼ�񱻴�" << endl;
			}
			break;
		case '?':
			Usage();
			cout << "Menu Help." << endl;
			break;
        default:
            cout << "Menu None." << endl;
            Usage();
            break;
        }
        // switch - case end

    } while (cCmd != 'x' && cCmd != 'X');

    return 0;
}

void Usage()
{
    cout << "############ Remote Sensing Image Process Tools.############\n";
    
    cout << "# O �C Open \t��Ӱ���ļ�" << endl;
	cout << "# C �CClosed\t�رյ�ǰͼ��" << endl;
    cout << "# I �C Information\t�����ǰͼ���·�� ������ֵ �������� �������� ������ �͡����з�ʽ����Ϣ" << endl;
	cout << "# S �C Statistics\t���ͼ������ͳ����" << endl;
    cout << "# H �C Histogram\t���ͼ���ֱ��ͼ" << endl;
	cout << "# W �C Write\t��ͼ��ÿ�����ε����ֵ����Сֵ����ֵ�����ֱ��ͼ���浽�ı���" << endl;
	cout << "# D �C Display \t��ʾӰ���ļ�" << endl;
	cout << "# Z �C Zoom\tͼ�����ţ���������߳�" << endl;
	cout << "# R �C Rotate\tͼ����ת������Ƕ���ʱ��" << endl;	
	cout << "# F - Filter\t�����˲�����(1,2,3,4)������˲���ͼ��" << endl;
    //cout << "# A �C Save as\t���뱣����ļ�·�� �����ͼ��Ϊ�������ļ�" << endl;
    
	cout << "# ? �C Help\t�������Ϣ" << endl;
	cout << "# X �C Exit Exit\t�˳�����" << endl;
    cout << "#################################################################" << endl;
}

void FilterImage(CRSImage* pRSImg , struct matrix* mat) {
	if (pRSImg == NULL)
		return;
	HWND	hwnd = NULL;
	HDC		hdc = NULL;
	hwnd = GetConsoleWindow();
	if (hwnd == NULL)
		return;
	hdc = GetDC(hwnd);
	if (hdc == NULL)
		return;
	int		nBands = pRSImg->GetBands();
	int		nSamples = pRSImg->GetSamples();
	int		nLines = pRSImg->GetLines();
	unsigned char*** pppData = pRSImg->GetDataBuffer();
	unsigned char** ppRed = pppData[0];
	unsigned char** ppGrn = nBands > 2 ? pppData[1] : pppData[0];
	unsigned char** ppBlu = nBands > 2 ? pppData[2] : pppData[0];
	unsigned char*** dst = new unsigned char** [3];
	for (int i = 0; i < 3; ++i) {
		dst[i] = new unsigned char* [nLines];
		for (int j = 0; j < nLines; ++j) {
			dst[i][j] = new unsigned char[nSamples];
		}
	}

	for (int x = 0; x < nSamples - 1; x++) {
		for (int y = 1; y < nLines - 1; y++) {
			dst[0][y][x] =
				(ppRed[y - 1][x - 1] * (mat->a11)) + (ppRed[y - 1][x] * (mat->a12)) + (ppRed[y - 1][x + 1] * (mat->a13)) +
				(ppRed[y][x - 1] * (mat->a21)) + (ppRed[y][x] * (mat->a22)) + (ppRed[y][x + 1] * (mat->a23)) +
				(ppRed[y + 1][x - 1] * (mat->a31)) + (ppRed[y + 1][x] * (mat->a32)) + (ppRed[y + 1][x + 1] * (mat->a33));
			dst[1][y][x] =
				(ppGrn[y - 1][x - 1] * (mat->a11)) + (ppGrn[y - 1][x] * (mat->a12)) + (ppGrn[y - 1][x + 1] * (mat->a13)) +
				(ppGrn[y][x - 1] * (mat->a21)) + (ppGrn[y][x] * (mat->a22)) + (ppGrn[y][x + 1] * (mat->a23)) +
				(ppGrn[y + 1][x - 1] * (mat->a31)) + (ppGrn[y + 1][x] * (mat->a32)) + (ppGrn[y + 1][x + 1] * (mat->a33));
			dst[2][y][x] =
				(ppBlu[y - 1][x - 1] * (mat->a11)) + (ppBlu[y - 1][x] * (mat->a12)) + (ppBlu[y - 1][x + 1] * (mat->a13)) +
				(ppBlu[y][x - 1] * (mat->a21)) + (ppBlu[y][x] * (mat->a22)) + (ppBlu[y][x + 1] * (mat->a23)) +
				(ppBlu[y + 1][x - 1] * (mat->a31)) + (ppBlu[y + 1][x] * (mat->a32)) + (ppBlu[y + 1][x + 1] * (mat->a33));

		}
	}


	//20 pixel per line
	for (int i = 0; i < nLines / 20; i++)
	{
		cout << endl;
	}
	for (int i = 0; i < nLines; i++)
	{
		for (int j = 0; j < nSamples; j++)
		{
			SetPixel(hdc, j, i, RGB(dst[0][i][j], dst[1][i][j], dst[2][i][j]));
		}
	}
	for (int i = 0; i < 3; ++i) {

		for (int j = 0; j < nLines; ++j) {
			delete[] dst[i][j];
		}
		delete[] dst[i];
	}
	delete[] dst;
	ReleaseDC(hwnd, hdc);
}

void RotateImage(CRSImage* pRSImg,float d) {
	if (pRSImg == NULL)
		return;
	HWND	hwnd = NULL;
	HDC		hdc = NULL;
	hwnd = GetConsoleWindow();
	if (hwnd == NULL)
		return;
	hdc = GetDC(hwnd);
	if (hdc == NULL)
		return;

	int		nBands = pRSImg->GetBands();		
	int		nSamples = pRSImg->GetSamples();	
	int		nLines = pRSImg->GetLines();
	unsigned char*** pppData = pRSImg->GetDataBuffer();	
	unsigned char** ppRed = pppData[0];		
	unsigned char** ppGrn = nBands > 2 ? pppData[1] : pppData[0];
	unsigned char** ppBlu = nBands > 2 ? pppData[2] : pppData[0];


	unsigned char*** dst = new unsigned char** [3];
	for (int i = 0; i < 3; ++i) {
		dst[i] = new unsigned char* [nLines];
		for (int j = 0; j < nLines; ++j) {
			dst[i][j] = new unsigned char[nSamples];
		}
	}
	unsigned char rr, gg, bb;
	unsigned char r11, r12, r21, r22;
	unsigned char g11, g12, g21, g22;
	unsigned char b11, b12, b21, b22;
	int a, b;
	double xx, yy;
	double rx0 = nSamples * 0.5;
	double ry0 = nLines * 0.5;
	for (int x = 0; x < nSamples; x++) {
		for (int y = 0; y < nLines; y++) {
			xx = (x - rx0) * cos(d) - (y - ry0) * sin(d) + rx0;
			yy = (x - rx0) * sin(d) + (y - ry0) * cos(d) + ry0;
			a = (int)xx;
			b = (int)yy;

			if ((a == nSamples-1) || (b == nLines-1)) {
				dst[0][y][x] = ppRed[b][a];
				dst[1][y][x] = ppGrn[b][a];
				dst[2][y][x] = ppBlu[b][a];
			}


			if ( ((a < nSamples-1) && a>=0) && ((b< nLines-1) && b>=0)){
				r11 = ppRed[b][a];
				r12 = ppRed[b][a + 1];
				r21 = ppRed[b + 1][a];
				r22 = ppRed[b + 1][a + 1];
				rr = (unsigned char)(r11 * (a + 1 - xx) * (b + 1 - yy) + r12 * (a + 1 - xx) * (yy - b)
					+ r21 * (xx - a) * (b + 1 - yy) + r22 * (xx - a) * (yy - b));

				g11 = ppGrn[b][a];
				g12 = ppGrn[b][a + 1];
				g21 = ppGrn[b + 1][a];
				g22 = ppGrn[b + 1][a + 1];
				gg = (unsigned char)(g11 * (a + 1 - xx) * (b + 1 - yy) + g12 * (a + 1 - xx) * (yy - b)
					+ g21 * (xx - a) * (b + 1 - yy) + g22 * (xx - a) * (yy - b));

				b11 = ppBlu[b][a];
				b12 = ppBlu[b][a + 1];
				b21 = ppBlu[b + 1][a];
				b22 = ppBlu[b + 1][a + 1];
				bb = (unsigned char)(b11 * (a + 1 - xx) * (b + 1 - yy) + b12 * (a + 1 - xx) * (yy - b)
					+ b21 * (xx - a) * (b + 1 - yy) + b22 * (xx - a) * (yy - b));
				dst[0][y][x] = rr;
				dst[1][y][x] = gg;
				dst[2][y][x] = bb;
			}
			
		}
	}

	//20 pixel per line
	for (int i = 0; i < 400 / 20; i++)
	{
		cout << endl;
	}
	for (int i = 0; i < 400; i++)
	{
		for (int j = 0; j < 640; j++)
		{

			SetPixel(hdc, j, i, RGB(dst[0][i][j], dst[1][i][j], dst[2][i][j]));
		}
	}
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < nLines; ++j) {
			delete[] dst[i][j];
		}
		delete[] dst[i];
	}
	delete[] dst;
	ReleaseDC(hwnd, hdc);
}

void DisplayImage(CRSImage* pRSImg)
{
	//TODO:��������Ȥ�������޸�DisplayImage�Ľӿڣ�ʵ��ͼ��Ķ��ƻ���ʾ���磺������ϵȡ�
	// �����ж�
	if (pRSImg == NULL)
		return;
	HWND	hwnd = NULL;//��þ��
	HDC		hdc = NULL;

	// ���ھ��ΪNULL������
	hwnd = GetConsoleWindow();
	if (hwnd == NULL)
		return;

	// �豸�����ķ���NULL�����򷵻�
	hdc = GetDC(hwnd);
	if (hdc == NULL)
		return;
	int		nBands = pRSImg->GetBands();		//����
	int		nSamples = pRSImg->GetSamples();	//��
	int		nLines = pRSImg->GetLines();		//��
	unsigned char*** pppData = pRSImg->GetDataBuffer();	//��ά����ͷָ��
	unsigned char** ppRed = pppData[0];		//��ɫͨ���Ĳ���
	unsigned char** ppGrn = nBands > 2 ? pppData[1] : pppData[0];	//��ɫͨ���Ĳ���
	unsigned char** ppBlu = nBands > 2 ? pppData[2] : pppData[0];	//��ɫͨ���Ĳ���

	//20 pixel per line
	for (int i = 0; i < nLines / 20; i++)
	{
		cout << endl;
	}
	for (int i = 0; i < nLines; i++)
	{
		for (int j = 0; j < nSamples; j++)
		{
			SetPixel(hdc, j, i, RGB(ppRed[i][j], ppGrn[i][j], ppBlu[i][j]));
		}
	}

	ReleaseDC(hwnd, hdc);
}

void ScaleImage(CRSImage* pRSImg, float k) {
	if (pRSImg == NULL)
		return;
	HWND	hwnd = NULL;//��þ��
	HDC		hdc = NULL;
	// ���ھ��ΪNULL������
	hwnd = GetConsoleWindow();
	if (hwnd == NULL)
		return;
	hdc = GetDC(hwnd);
	if (hdc == NULL)
		return;
	int		nBands = pRSImg->GetBands();
	int		nSamples = pRSImg->GetSamples();
	int		nLines = pRSImg->GetLines();
	unsigned char*** pppData = pRSImg->GetDataBuffer();
	unsigned char** ppRed = pppData[0];
	unsigned char** ppGrn = nBands > 2 ? pppData[1] : pppData[0];
	unsigned char** ppBlu = nBands > 2 ? pppData[2] : pppData[0];
	int widthNew, heightNew;
	float widthScale = (float)(1.0 / k), heightScale = (float)(1.0 / k);
	widthNew = (int)(nSamples * k);
	heightNew = (int)(nLines * k);
	float xx, yy;
	int a, b;
	unsigned char rr, gg, bb;
	unsigned char r11, r12, r21, r22;
	unsigned char g11, g12, g21, g22;
	unsigned char b11, b12, b21, b22;
	unsigned char*** dst = new unsigned char** [3];
	for (int i = 0; i < 3; ++i) {
		dst[i] = new unsigned char* [heightNew];
		for (int j = 0; j < heightNew; ++j) {
			dst[i][j] = new unsigned char[widthNew];
		}
	}



	for (int x = 0; x < widthNew; x++) {
		for (int y = 0; y < heightNew; y++) {
			xx = x * widthScale;
			yy = y * heightScale;
			if (xx <= 1e-8)
				xx = 0;
			if (xx > nSamples - 2)
				xx = (float)(nSamples - 2);
			if (yy <= 1e-8)
				yy = 0;
			if (yy > nLines - 2)
				yy = (float)(nLines - 2);
			a = (int)xx;
			b = (int)yy;
			//�ֱ�õ���Ӧ���ص�R��G��Bֵ����˫���Բ�ֵ�õ������ص�R��G��Bֵ
			r11 = ppRed[b][a];
			r12 = ppRed[b][a + 1];
			r21 = ppRed[b + 1][a];
			r22 = ppRed[b + 1][a + 1];
			rr = (unsigned char)(r11 * (a + 1 - xx) * (b + 1 - yy) + r12 * (a + 1 - xx) * (yy - b)
				+ r21 * (xx - a) * (b + 1 - yy) + r22 * (xx - a) * (yy - b));

			g11 = ppGrn[b][a];
			g12 = ppGrn[b][a + 1];
			g21 = ppGrn[b + 1][a];
			g22 = ppGrn[b + 1][a + 1];
			gg = (unsigned char)(g11 * (a + 1 - xx) * (b + 1 - yy) + g12 * (a + 1 - xx) * (yy - b)
				+ g21 * (xx - a) * (b + 1 - yy) + g22 * (xx - a) * (yy - b));

			b11 = ppBlu[b][a];
			b12 = ppBlu[b][a + 1];
			b21 = ppBlu[b + 1][a];
			b22 = ppBlu[b + 1][a + 1];
			bb = (unsigned char)(b11 * (a + 1 - xx) * (b + 1 - yy) + b12 * (a + 1 - xx) * (yy - b)
				+ b21 * (xx - a) * (b + 1 - yy) + b22 * (xx - a) * (yy - b));
			dst[0][y][x] = (unsigned char)min(255, rr);
			dst[1][y][x] = (unsigned char)min(255, gg);
			dst[2][y][x] = (unsigned char)min(255, bb);
		}
	}

	//20 pixel per line
	for (int i = 0; i < heightNew / 20; i++)
	{
		cout << endl;
	}
	for (int i = 0; i < heightNew; i++)
	{
		for (int j = 0; j < widthNew; j++)
		{

			SetPixel(hdc, j, i, RGB(dst[0][i][j], dst[1][i][j], dst[2][i][j]));
		}
	}
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < heightNew; ++j) {
			delete[] dst[i][j];
		}
		delete[] dst[i];
	}
	delete[] dst;
	ReleaseDC(hwnd, hdc);

}

