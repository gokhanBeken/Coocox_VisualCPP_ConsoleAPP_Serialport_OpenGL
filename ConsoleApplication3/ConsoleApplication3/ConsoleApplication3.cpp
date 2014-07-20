// ConsoleApplication3.cpp : main project file.
//not: yeni proje oluþturken CLR'nin altýndaki consoleyi seçerek yaptým: burdan öðrendim: http://social.msdn.microsoft.com/Forums/en-US/c7bdc372-ff73-4a04-a620-1bcc473db9bc/namespace-system-not-found
// geliþmiþ bir seri port programý: http://msdn.microsoft.com/en-us/library/system.io.ports.serialport%28VS.90%29.aspx
//basit bir seri port programý: http://msdn.microsoft.com/tr-tr/library/system.io.ports.serialport.datareceived%28v=vs.110%29.aspx



#include "stdafx.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#include <string> //katardan çevirme iþlemi için

#include <GL/glut.h>
//#define PORT_SECIMI "COM1"

GLfloat rtri;
GLfloat rquad;

//yeni denemeler
GLfloat x_derece = 0;
GLfloat y_derece = 0;
GLfloat z_derece = 0;

char seriportBuffer[30];
unsigned char seriportSayac = 0;

GLfloat x_durum, y_durum, z_durum;
static int hangiSekil = 9; //menudeki degisiklikler bu degiskene yazilir(9=demlik)


#using <System.dll>


using namespace System;
using namespace System::IO::Ports;

using namespace std;

using namespace System::Threading;



//FONKSÝYON PROTOTÝPLERÝM:
void katardanFloata(char * katar);
/////////////////////////////////


void DataReceivedHandler(Object^ sender, SerialDataReceivedEventArgs^ e)
{
	SerialPort^ sp = (SerialPort^)sender;
	//seriportBuffer[seriportSayac] = sp->ReadChar();

	String^ indata = sp->ReadLine(); //gelen veride hem \r hem de \n olmak zorunda

	//Console::Write(indata);
	//printf("%c", indata[0]);

	for (int k = 0; k < 30; k++){
		seriportBuffer[k] = 0;
	}

	for (int k = 0; k < indata->Length; k++){
		seriportBuffer[k] = indata[k];
		if (indata[k] == '\r' || indata[k] == '\n'){
			//printf("%s", seriportBuffer);
			indata = "";

			katardanFloata(seriportBuffer);
			break;
		}
	}
}

void seri_port_baglan()
{
	SerialPort^ mySerialPort = gcnew SerialPort("COM7");

	mySerialPort->BaudRate = 9600;
	mySerialPort->Parity = Parity::None;
	mySerialPort->StopBits = StopBits::One;
	mySerialPort->DataBits = 8;
	mySerialPort->Handshake = Handshake::None;

	mySerialPort->DataReceived += gcnew SerialDataReceivedEventHandler(DataReceivedHandler);

	// Set the read/write timeouts
	//mySerialPort->ReadTimeout = 500;
	//mySerialPort->WriteTimeout = 500;

	mySerialPort->Open();

	Console::WriteLine("Baðlantý kuruldu...");

	mySerialPort->Write("Merhaba\r\n");

	//Console::WriteLine();
	//Console::ReadKey();
	//mySerialPort->Close();
}



void ayarlar(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-7.0, 7.0, -2.0, 2.0, -5.0, 5.0);

	rtri = 0.0f;
}


void katardanFloata(char * katar){
	int i = 0;
	//strlen(katar)
	int numaraTut = 0, numaraTut2 = 0, numaraTut3 = 0;
	int virgulTut = 0, virgulTut2 = 0, virgulTut3 = 0;

	for (i = 0; i < strlen(katar); i++){
		if (katar[i] == '='){
			numaraTut = i + 1;
			break;
		}
	}

	for (i = numaraTut; i < strlen(katar); i++){
		if (katar[i] == '='){
			numaraTut2 = i + 1;
			break;
		}
	}

	for (i = numaraTut2; i < strlen(katar); i++){
		if (katar[i] == '='){
			numaraTut3 = i + 1;
			break;
		}
	}
	//x=45,y=0,z=0\r 2,4,7,8,11,12
	//virgüller

	for (i = numaraTut; i < strlen(katar); i++){
		if (katar[i] == ','){
			virgulTut = i;
			break;
		}
	}
	for (i = numaraTut2; i < strlen(katar); i++){
		if (katar[i] == ','){
			virgulTut2 = i;
			break;
		}
	}
	for (i = numaraTut3; i < strlen(katar); i++){
		if (katar[i] == '\r'){
			virgulTut3 = i;
			break;
		}
	}


	//çevirme iþi
	if (virgulTut - numaraTut == 1){ //opengl'in anladýðý z deðiþkeni, aslýnda yani görükürde x eksenidir,  Ama opengl nedense z ile kontrol ediyor.
		z_derece = (katar[numaraTut] - '0');
	}
	else if (virgulTut - numaraTut == 2){
		z_derece = 10 * (katar[numaraTut] - '0');
		z_derece += (katar[numaraTut + 1] - '0');
	}
	else if (virgulTut - numaraTut == 3){
		z_derece = 100 * (katar[numaraTut] - '0');
		z_derece += 10 * (katar[numaraTut + 1] - '0');
		z_derece += (katar[numaraTut + 2] - '0');
	}
	else{
	}
	//z_derece=360-z_derece; //bu eksen nedense ters gösteriyor, 360'dan çýkartarak bu sorunu aþtým



	if (virgulTut2 - numaraTut2 == 1){ //opengl'in anladýðý x deðiþkeni, aslýnda yani görükürde y eksenidir,  Ama opengl nedense x ile kontrol ediyor.
		x_derece = (katar[numaraTut2] - '0');
	}
	else if (virgulTut2 - numaraTut2 == 2){
		x_derece = 10 * (katar[numaraTut2] - '0');
		x_derece += (katar[numaraTut2 + 1] - '0');
	}
	else if (virgulTut2 - numaraTut2 == 3){
		x_derece = 100 * (katar[numaraTut2] - '0');
		x_derece += 10 * (katar[numaraTut2 + 1] - '0');
		x_derece += (katar[numaraTut2 + 2] - '0');
	}
	else{
	}



	if (virgulTut3 - numaraTut3 == 1){ //opengl'in anladýðý y deðiþkeni, aslýnda yani görükürde z eksenidir, bu yüzden cihaz hep 0 gönderiyor.
		y_derece = (katar[numaraTut3] - '0');
	}
	else if (virgulTut3 - numaraTut3 == 2){
		y_derece = 10 * (katar[numaraTut3] - '0');
		y_derece += (katar[numaraTut3 + 1] - '0');
	}
	else if (virgulTut3 - numaraTut3 == 3){
		y_derece = 100 * (katar[numaraTut3] - '0');
		y_derece += 10 * (katar[numaraTut3 + 1] - '0');
		y_derece += (katar[numaraTut3 + 2] - '0');
	}
	else{
	}

	//printf("%f , %f , %f \r\n",x_derece,y_derece,z_derece);

	printf("x=%d , y=%d \r\n", (int)z_derece, (int)x_derece);
}

void idle()
{


	float bilgi;
	//char * gelenVeri="x=45,y=0,z=0\r"; //x:dikey, y: kumpas, z: yatay

	char * gelenVeri[20];

	//x_durum=1.0; y_durum=1.0; z_durum=0.0; //bu kýsým birþey deðiþtirmiyor, aþaðýda "gosterim" fonksiyonunda ayarlanýyor çünkü

	//rtri +=0.01f;
	//rquad+=0.15f; //bunun ne iþe yaradýðýný daha çözemedim



	//memset(&gelenVeri[0], 0, sizeof(gelenVeri));
	//ReadFile(hCom, gelenVeri, sizeof(gelenVeri), &nRead, NULL);
	//if (gelenVeri[0] == 'x' || 1)katardanFloata(gelenVeri); //gelen veri bizim formatýmýzdaysa iþleme koy




	glutPostRedisplay();



}


void keyboardCB(unsigned char key, int x, int y)
{
	rtri += 0.5f;

	switch (key)
	{
	case 27: // ESCAPE
		//clearSharedMem();
		exit(0);
		break;

	case 'w':
		x_derece -= 1.0;
		x_durum = 1.0; y_durum = 0.0; z_durum = 0.0;
		break;

	case 'a':
		z_derece += 1.0;
		x_durum = 0.0; y_durum = 0.0; z_durum = 0.1;
		break;

	case 's':
		x_derece += 1.0;
		x_durum = 0.1; y_durum = 0.0; z_durum = 0.0;
		break;

	case 'd':
		z_derece -= 1.0;
		x_durum = 0.0; y_durum = 0.0; z_durum = 0.1;
		break;

		//kumpas:
	case 'z':
		y_derece -= 1.0;
		x_durum = 0.0; y_durum = 0.1; z_durum = 0.0;
		break;

	case 'x':
		y_derece += 1.0;
		x_durum = 0.0; y_durum = 0.1; z_durum = 0.0;
		break;

		//sýfýrla
	case 'o':
		x_durum = 0.1;
		y_durum = 0.1;
		z_durum = 0.1;

		x_derece = 45.0;
		y_derece = 0.0;
		z_derece = 0.0;
		break;


	default:
		//x_aci=-0.00f; y_aci=-0.00f;  z_aci=-0.00f;   
		;
	}

	//iþi saðlama alalým diye yazdýk þart deðil:
	if (x_derece == 360.0 || x_derece == -360.0)x_derece = 0.0;
	if (y_derece == 360.0 || y_derece == -360.0)x_derece = 0.0;
	if (y_derece == 360.0 || z_derece == -360.0)x_derece = 0.0;

	glutPostRedisplay();

}

void gosterim(void)
{
	glEnable(GL_DEPTH_TEST);
	glColor3f(1.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Pencereyi temizle



	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();



	//nesneyi taþýmak: Nesneyi taþýmak: glTranslate{fd}( x, y, z )
	//bilgi: http://ozlemerden.wordpress.com/category/opengl/
	//Nesneyi bir eksen etrafýnda döndürmek: glRotate{fd}( açý, x, y, z )

	glPopMatrix(); // Transformasyonlari geri yukle
	//glTranslatef(4.0f,0.0f,0.0f); 
	glPushMatrix(); // Matris durumunu kaydet ve donusu yap



	//glRotatef(rtri,x_aci,y_aci,z_aci); //açý dediði aslýnda ne kadar çok döneceðini ayarlýyor

	//if(x_durum==1)glRotatef(rtri,0.1f,0.0f,0.0f);
	//if(y_durum==1)glRotatef(rtri,0.0f,0.1f,0.0f);
	//if(z_durum==1)glRotatef(rtri,0.0f,0.0f,0.1f);


	glRotatef(x_derece, 0.1f, 0.0f, 0.0f);
	glRotatef(y_derece, 0.0f, 0.1f, 0.0f);
	glRotatef(z_derece, 0.0f, 0.0f, 0.1f);


	if (hangiSekil == 21)//derece sifirlama
	{
		x_derece = 0;
		y_derece = 0;
		z_derece = 0;
		hangiSekil = 9;
	}


	//isiklandirma ve golgelendirme (harika sonuc üretiyor)////////////////////////
	//kaynak : http://web.media.mit.edu/~gordonw/OpenGL/
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glColor4f(1.0, 1.0, 1.0, 1.0); //rengi beyaz yapar
	GLfloat	lightpos[4] = { 5.0, 15.0, 10.0, 1.0 };
	///////////////////////////////////////////////////////////////////////////////

	switch (hangiSekil)
	{
	case 1:
		glutWireSphere(1.0f, 25, 25);
		break;

	case 2:
		glutWireCube(1.0f);
		break;

	case 3:
		glutWireCone(0.30f, 1.1f, 20, 20);
		break;

	case 4:
		glutWireTorus(0.3f, 1.0f, 10, 25);
		break;

	case 5:
		glutWireDodecahedron();
		break;

	case 6:
		glutWireOctahedron();
		break;

	case 7:
		glutWireTetrahedron();
		break;

	case 8:
		glutWireIcosahedron();
		break;

	case 9:
		glutWireTeapot(2.0f);
		break;

	case 11:
		glutSolidSphere(1.0f, 25, 25);
		break;

	case 12:
		glutSolidCube(1.0f);
		break;

	case 13:
		glutSolidCone(0.30, 1.1f, 20, 20);
		break;

	case 14:
		glutSolidTorus(0.3f, 1.0f, 10, 25);
		break;

	case 15:
		glutSolidDodecahedron();
		break;

	case 16:
		glutSolidOctahedron();
		break;

	case 17:
		glutSolidTetrahedron();
		break;

	case 18:
		glutSolidIcosahedron();
		break;

	case 19:
		glutSolidTeapot(2.0f);
		break;

	case 20: //kendi cizdigim kup sekli
		glBegin(GL_QUADS);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glEnd();
		break;
	default:
		//glutSolidTeapot(1.0f);
		break;
	}



	//degisik sekiller cikartmak icin ornek: http://www.godoro.com/Divisions/Ehil/Mahzen/Programming/OpenGL/txt/html/document_AnimasyonAnd3D.html

	glPopMatrix(); // Transformasyonlari geri yukle
	glutSwapBuffers(); // Cizim komutlarini akit   	

}







void ProcessMenu(int value) //menu icin
{
	hangiSekil = value;

	glutPostRedisplay();
}

//int main()
//int main(array<System::String ^> ^args)
int main(int argc, char** argv)
{

	int i;

	int nWireMenu; //menu icin
	int nMainMenu; //menu icin
	int nSolidMenu; //menu icin   
	int nDigerMenu; //menu icin   

	printf("asd");

	seri_port_baglan();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(900, 400);
	glutCreateWindow("OpenGL Uygulamasý www.GokhanBeken.com");
	ayarlar();

	//Sað click Menu olustur/////////////////////////////////////////////////////////////////////////////
	nWireMenu = glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Kure", 1);
	glutAddMenuEntry("Kup", 2);
	glutAddMenuEntry("Koni", 3);
	glutAddMenuEntry("Torus", 4);
	glutAddMenuEntry("Dodecahedron", 5);
	glutAddMenuEntry("Sekizgen", 6);
	glutAddMenuEntry("Tetrahedron", 7);
	glutAddMenuEntry("Icosahedron", 8);
	glutAddMenuEntry("Caydanlik", 9);

	nSolidMenu = glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Kure", 11);
	glutAddMenuEntry("Kup", 12);
	glutAddMenuEntry("Koni", 13);
	glutAddMenuEntry("Torus", 14);
	glutAddMenuEntry("Dodecahedron", 15);
	glutAddMenuEntry("Sekizgen", 16);
	glutAddMenuEntry("Tetrahedron", 17);
	glutAddMenuEntry("Icosahedron", 18);
	glutAddMenuEntry("Caydanlik", 19);
	glutAddMenuEntry("Ozel Kup", 20);

	nDigerMenu = glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Sifirla", 21);

	nMainMenu = glutCreateMenu(ProcessMenu);
	glutAddSubMenu("Tel", nWireMenu);
	glutAddSubMenu("Kati", nSolidMenu);
	glutAddSubMenu("Diger", nDigerMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//////////////////////////////////////////////////////////////////////////////////////////  


	glutDisplayFunc(gosterim); //sekillerin cizildigi fonksiyon
	glutKeyboardFunc(keyboardCB); //klavye fonksiyonum
	glutIdleFunc(idle); //arka planda deðer deðiþtirmek için genel fonksiyon
	glutMainLoop();

	//PortDataReceived::Main();

	/*seri_port_baglan();
	mySerialPort.Write("merhaba\r\n");*/
	//PortDataReceived::Main();
	//getchar();
	return 0;
}

