#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "kalman_shift.h"
using namespace cv;
using namespace std;
vector<float> shift;
ofstream file;
vector<float> kalman_shift;

//return data by vector
int get_Txtdata_lineByline(vector<float> &integer, char *fileName) {
	ifstream infile;
	infile.open(fileName);
	assert(infile.is_open()); //��ʧ�ܣ������������Ϣ������ֹ��������
	int number;
	while (!infile.eof()) {
		infile >> number;
		integer.push_back(number);
	}
	infile.close();
	return integer.size();
}

int print_vector(vector<float> &integer) {

	for (vector<float>::iterator it = integer.begin(); it != integer.end(); ++it) {
		cout << *it << endl;
	}

	return 0;
}

int write_vector2txt(vector<float> &integer,string file_path) {
	ofstream file;
	file.open(file_path);
	assert(file.is_open());
	for (vector<float>::iterator it= integer.begin(); it != integer.end(); it++) {
		file << *it << endl;
	}
	file.close();

	return 0;
}


int main234234() {
	int Num = get_Txtdata_lineByline(shift, "d:/imgsource/panorama/823algorithm_orbsiftbefore.txt");
	//cout << Num << endl;
	//print_vector(shift);

	const int stateNum = 3;
	const int measureNum = 1;
	KalmanFilter KF(stateNum, measureNum, 0);
	KF.transitionMatrix = (Mat_<float>(3, 3) << 1, 0, 1, 1, 0, 0, 0, 0, 1);  //ת�ƾ���A  
	//setIdentity(KF.measurementMatrix);                                 //��������H  
	KF.measurementMatrix = (Mat_<float>(1, 3) << 1, -1, 0);
	cout << KF.measurementMatrix << endl;

	setIdentity(KF.processNoiseCov, Scalar::all(1e-5));                //ϵͳ�����������Q  
	setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));            //���������������R  
	setIdentity(KF.errorCovPost, Scalar::all(1));					   //����������Э�������P
	Mat measurement = Mat::zeros(measureNum, 1, CV_32F);

	//��ʼ״ֵ̬
	KF.statePost = (Mat_<float>(3, 1) << 0,0,1);
	int i = 0;
	while (i != Num) {
		i++;
		//Ԥ��
		Mat prediction = KF.predict();
		int predict_shift = prediction.at<float>(0);
		//�������ֵ
		cout << "shift" << "\t" << shift[i] << endl;
		measurement.at<float>(0) = shift[i];

		//����
		KF.correct(measurement);
		
		cout << "predict =" << "\t" << prediction.at<float>(0) - prediction.at<float>(1) << endl;
		cout << "statePre =" << "\t" << KF.statePre.at<float>(0) - KF.statePre.at<float>(1) << endl;
		cout << "measurement=" << "\t" << measurement.at<float>(0) << endl;
		cout << "correct =" << "\t" << KF.statePost.at<float>(0)- KF.statePost.at<float>(1) << endl;
		kalman_shift.push_back(KF.statePost.at<float>(0) - KF.statePost.at<float>(1));
	}
	cout << i << endl;

	int write_result = write_vector2txt(kalman_shift,"d:/imgsource/panorama/kalman_shift_823algorithm_orbsiftbefore.txt");

	system("pause");
	return 0;
}