#pragma once

#include "SceneVisualizer.h"
#include "ShipParam.h"
#include "RadarForm.h"
#include "SettingsForm.h"
#include "ScenesForm.h"
#include "SelectObject.h"

#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28

typedef struct tag_send_data
	{
		double rx, ry, rz;
		double x[50], y[50], z[50];
		double vx[50], vy[50], vz[50];
		char type[50];
		int cnt;
	} SEND_DATA;

namespace Interface {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;

	
public ref class MyParams : public System::Object 
	{
	public:
		int RadarNumber;
		System::Object ^obj;
		String^ HostName;
		int PortNumber;
		long nTimeOut;
	};

	/*public ref struct DataObjects
	{
		System::Windows::Forms::DataGridView^  Objects;
		int nMaxObjectsNumber;
		array <System::Windows::Forms::DataGridView^>^  Routes;

		DataObjects()
		{
			nMaxObjectsNumber = 50;

			Objects = gcnew System::Windows::Forms::DataGridView;
			Objects->ColumnCount = 7;

			Routes = gcnew array <System::Windows::Forms::DataGridView^> (nMaxObjectsNumber);
			for each (System::Windows::Forms::DataGridView^ r in Routes)
			{
				r->ColumnCount = 6;
			}
		}

		void Add(int nID, int nType, String^ sName, bool bRadar, int nX0, int nY0, int nH0)
		{
			Objects->Rows->Add(nID, nType, sName, bRadar, nX0, nY0, nH0);
		}
		void Delete (int nID)
		{
			int searchValue = nID;
			int rowIndex = -1;
			for each (System::Windows::Forms::DataGridViewRow^ row in Objects->Rows)
			{
				if(row->Cells[0]->Value->ToString()->Equals(searchValue))
				{
					rowIndex = row->Index;
					break;
				}
			}

			Objects->Rows->Remove(Objects->Rows[rowIndex]);
		}
		int GetCount() { return Objects->Rows->Count;}
		System::Windows::Forms::DataGridViewRow^ GetRow(int n)
		{
			return  Objects->Rows[n];
		}
		int GetNextID()
		{
			int nNextID = 0;
			for each (System::Windows::Forms::DataGridViewRow^ row in Objects->Rows)
			{
				if(nNextID < Int32::Parse(row->Cells[0]->Value->ToString()))
				{
					nNextID = Int32::Parse(row->Cells[0]->Value->ToString());
				}
			}
			return 0;
		}
		String^ GetStringType(int nType)
		{
			String^ sType;
			switch(nType)
				 {
				 case 0 :     //Ðàäàð
					 sType = "Ðàäàð";
					 break;
				 case 1 :		 //êîðàáëü
					 sType = "Êîðàáëü";
					 break;
				 case 2: //ñàìîëåò
					  sType = "Ñàìîëåò";
					 break;
				 case 3: //ðàêåòà
					  sType = "Ðàêåòà";
					 break;
				 default :
					 sType = "Ðàäàð";
					 break;
				 }
			return sType;
		}
		void SetAllDataTo(System::Windows::Forms::DataGridView^ dataGridView)
		{
			//Settings->dataGridView1->Rows->Add(0,1, "Êîðàáëü", "1", true, 1, 1, 1);
			//Objects->Rows->Add(nID, nType, sName, bRadar, nX0, nY0, nH0);
			for each (System::Windows::Forms::DataGridViewRow^ row in Objects->Rows)
			{
				dataGridView->Rows->Add(row->Cells[0]->Value, 1,
					                    GetStringType(Int32::Parse(row->Cells[1]->Value->ToString())),
										row->Cells[2]->Value,
										row->Cells[3]->Value,
										row->Cells[4]->Value, row->Cells[5]->Value, row->Cells[6]->Value);
			}

		}

	};*/

	
CSceneVisualizer sv;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{

	public:
		bool bWork;
		bool bDataRecive;
	private: System::Windows::Forms::ToolStripMenuItem^  ñãåíåðèðîâàòüÑöåíóToolStripMenuItem;
	public: 
	private: Thread^ newThread;
	private: array <Thread^>^ RadarThread;

	public: static void DelegateThreadReciveTask( Object^ data)
			{
				Form1^ ob = (Form1^) data;
				
				System::Net::Sockets::UdpClient^ udpClient = gcnew System::Net::Sockets::UdpClient(14001);
				System::Net::IPEndPoint^ RemoteIpEndPoint = gcnew System::Net::IPEndPoint( System::Net::IPAddress::Any,0 );
				do{
					SEND_DATA a;
					array<Byte>^receiveBytes = udpClient->Receive( RemoteIpEndPoint );
					char *ptr = (char *)&a;
					try{
					for(int i = 0; i < receiveBytes->Length; ++i)
					{	ptr[i] = receiveBytes[i]; }
					} catch(...) { ;}

					sv.DeleteAllObjectes();
					try{
					sv.SetNumberObject(a.cnt); //a.cnt
					
					
					for (int i = 0; i!=a.cnt; i++)
					{
						int ObjectType = a.type[i];
						//sv.AddObject(i, 1,	false, 0, 0, 0, System::Drawing::Color::White.ToArgb());
						sv.AddObject(i, ObjectType,	a.x[i], a.y[i], a.z[i], a.vx[i], a.vy[i], a.vz[i]);
					}
					} catch(...) { ;}
					//sv.Update();
					//sv.LookAll();
					sv.Update();

				}while (ob->bWork);
				udpClient->Close();
				MessageBox::Show("Ïðèåì äàííûõ îñòàíîâëåí");
			}
	public: static void DelegateThreadTask( Object^ data)
			{
				MyParams^ p = (MyParams^) data;
				int RadarNumber = p->RadarNumber;  
				String^ HostName = p->HostName; 
				int PortNumber = p->PortNumber; 
				long nTimeOut = p->nTimeOut;
				Form1^ ob = (Form1^) p->obj;
				
				String^ sentence;				
				System::Net::Sockets::UdpClient^ udpClient = gcnew System::Net::Sockets::UdpClient;
				udpClient->Connect( HostName, PortNumber );
				
				sentence = ob->GetRadarData(RadarNumber); //òèï îáúåêòà, x, y, z, vx, vy, vz
				
				SEND_DATA a;
				sv.GetRadarData(RadarNumber, a.rx, a.ry, a.rz);

				do
				{
					
					a.cnt = ob->GetObjectsNumber();
					for(int i = 0; i!=ob->GetObjectsNumber(); i++)
					{
						int type;
						sv.GetObjectData(i, type, a.x[i],a.y[i], a.z[i], a.vx[i], a.vy[i], a.vz[i]);
						a.type[i] = type;
												
					}	
					array<unsigned char>^sendBytes1 = gcnew cli::array<unsigned char>(sizeof(SEND_DATA));
					
					char *ptr = (char *)&a;
					for(int i = 0;i < sizeof(SEND_DATA);++i)
						sendBytes1[i] = ptr[i];

					udpClient->Send( sendBytes1, sizeof(SEND_DATA) );
					Thread::Sleep(nTimeOut);
				} while (ob->bWork==true);
				udpClient->Close();
			}

	private:
		
		ShipParam^ ShipForm;
		ShipParam^ PlaneForm;
		ShipParam^ MissileForm;
		RadarForm^ Radar1;
		RadarForm^ Radar2;
		RadarForm^ Radar3;
	public:
		bool bMouse;
		bool bCreate;
		bool bGenerated;
		System::Drawing::Point old;
		int err;
 
    //public: DataObjects^ objects;
	public:	SettingsForm^ Settings;
	private: System::Windows::Forms::ToolStripMenuItem^  ïðîåêòToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  îòêðûòüToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  íîâûéÏðîåêòToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ñîõðàíèòüÏðîåêòToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ñèìóëÿöèÿToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  íàñòðîéêèÑèìóëÿöèèToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  çàïóñòèòüÑèìóëÿöèþToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  îñòàíîâèòüÑèìóëÿöèþToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  êàìåðàToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  îáùèéÂèäToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  âèäÍàÎáúåêòToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  transToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ðåæèìÏðèåìàÄàííûõToolStripMenuItem;

		 System::Drawing::Point now;
		
public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//objects = gcnew DataObjects;
			bMouse = false;
			bCreate = false;
			bWork = false;
			bDataRecive = false;
			bGenerated = false;
			this->Text = "Âèçóàëèçàòîð 3D";

			
			RadarThread = gcnew cli::array<Thread^>(20);
			

			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{

			if (components)
			{
				delete components;
				delete Settings;
			}
			
		}
	public:
		
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->ïðîåêòToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->íîâûéÏðîåêòToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->îòêðûòüToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ñîõðàíèòüÏðîåêòToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ðåæèìÏðèåìàÄàííûõToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ñèìóëÿöèÿToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->íàñòðîéêèÑèìóëÿöèèToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ñãåíåðèðîâàòüÑöåíóToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->çàïóñòèòüÑèìóëÿöèþToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->îñòàíîâèòüÑèìóëÿöèþToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->êàìåðàToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->îáùèéÂèäToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->âèäÍàÎáúåêòToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->transToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->panel1->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->panel1->Location = System::Drawing::Point(12, 27);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(696, 360);
			this->panel1->TabIndex = 0;
			this->panel1->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &Form1::panel1_Scroll);
			this->panel1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::panel1_MouseDown);
			this->panel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::panel1_MouseMove);
			this->panel1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::panel1_MouseUp);
			this->panel1->Resize += gcnew System::EventHandler(this, &Form1::panel1_Resize);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->ïðîåêòToolStripMenuItem, 
				this->ñèìóëÿöèÿToolStripMenuItem, this->êàìåðàToolStripMenuItem, this->transToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(720, 24);
			this->menuStrip1->TabIndex = 2;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// ïðîåêòToolStripMenuItem
			// 
			this->ïðîåêòToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->íîâûéÏðîåêòToolStripMenuItem, 
				this->îòêðûòüToolStripMenuItem, this->ñîõðàíèòüÏðîåêòToolStripMenuItem, this->ðåæèìÏðèåìàÄàííûõToolStripMenuItem});
			this->ïðîåêòToolStripMenuItem->Name = L"ïðîåêòToolStripMenuItem";
			this->ïðîåêòToolStripMenuItem->Size = System::Drawing::Size(59, 20);
			this->ïðîåêòToolStripMenuItem->Text = L"Ïðîåêò";
			// 
			// íîâûéÏðîåêòToolStripMenuItem
			// 
			this->íîâûéÏðîåêòToolStripMenuItem->Name = L"íîâûéÏðîåêòToolStripMenuItem";
			this->íîâûéÏðîåêòToolStripMenuItem->Size = System::Drawing::Size(200, 22);
			this->íîâûéÏðîåêòToolStripMenuItem->Text = L"Íîâûé ïðîåêò";
			this->íîâûéÏðîåêòToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::íîâûéÏðîåêòToolStripMenuItem_Click);
			// 
			// îòêðûòüToolStripMenuItem
			// 
			this->îòêðûòüToolStripMenuItem->Name = L"îòêðûòüToolStripMenuItem";
			this->îòêðûòüToolStripMenuItem->Size = System::Drawing::Size(200, 22);
			this->îòêðûòüToolStripMenuItem->Text = L"Îòêðûòü ïðîåêò";
			this->îòêðûòüToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::îòêðûòüToolStripMenuItem_Click);
			// 
			// ñîõðàíèòüÏðîåêòToolStripMenuItem
			// 
			this->ñîõðàíèòüÏðîåêòToolStripMenuItem->Enabled = false;
			this->ñîõðàíèòüÏðîåêòToolStripMenuItem->Name = L"ñîõðàíèòüÏðîåêòToolStripMenuItem";
			this->ñîõðàíèòüÏðîåêòToolStripMenuItem->Size = System::Drawing::Size(200, 22);
			this->ñîõðàíèòüÏðîåêòToolStripMenuItem->Text = L"Ñîõðàíèòü ïðîåêò";
			this->ñîõðàíèòüÏðîåêòToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::ñîõðàíèòüÏðîåêòToolStripMenuItem_Click);
			// 
			// ðåæèìÏðèåìàÄàííûõToolStripMenuItem
			// 
			this->ðåæèìÏðèåìàÄàííûõToolStripMenuItem->Name = L"ðåæèìÏðèåìàÄàííûõToolStripMenuItem";
			this->ðåæèìÏðèåìàÄàííûõToolStripMenuItem->Size = System::Drawing::Size(200, 22);
			this->ðåæèìÏðèåìàÄàííûõToolStripMenuItem->Text = L"Ðåæèì ïðèåìà äàííûõ";
			this->ðåæèìÏðèåìàÄàííûõToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::ðåæèìÏðèåìàÄàííûõToolStripMenuItem_Click);
			// 
			// ñèìóëÿöèÿToolStripMenuItem
			// 
			this->ñèìóëÿöèÿToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->íàñòðîéêèÑèìóëÿöèèToolStripMenuItem, 
				this->ñãåíåðèðîâàòüÑöåíóToolStripMenuItem, this->çàïóñòèòüÑèìóëÿöèþToolStripMenuItem, this->îñòàíîâèòüÑèìóëÿöèþToolStripMenuItem});
			this->ñèìóëÿöèÿToolStripMenuItem->Enabled = false;
			this->ñèìóëÿöèÿToolStripMenuItem->Name = L"ñèìóëÿöèÿToolStripMenuItem";
			this->ñèìóëÿöèÿToolStripMenuItem->Size = System::Drawing::Size(82, 20);
			this->ñèìóëÿöèÿToolStripMenuItem->Text = L"Ñèìóëÿöèÿ";
			// 
			// íàñòðîéêèÑèìóëÿöèèToolStripMenuItem
			// 
			this->íàñòðîéêèÑèìóëÿöèèToolStripMenuItem->Name = L"íàñòðîéêèÑèìóëÿöèèToolStripMenuItem";
			this->íàñòðîéêèÑèìóëÿöèèToolStripMenuItem->Size = System::Drawing::Size(206, 22);
			this->íàñòðîéêèÑèìóëÿöèèToolStripMenuItem->Text = L"Íàñòðîéêè ñèìóëÿöèè";
			this->íàñòðîéêèÑèìóëÿöèèToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::íàñòðîéêèÑèìóëÿöèèToolStripMenuItem_Click);
			// 
			// ñãåíåðèðîâàòüÑöåíóToolStripMenuItem
			// 
			this->ñãåíåðèðîâàòüÑöåíóToolStripMenuItem->Name = L"ñãåíåðèðîâàòüÑöåíóToolStripMenuItem";
			this->ñãåíåðèðîâàòüÑöåíóToolStripMenuItem->Size = System::Drawing::Size(206, 22);
			this->ñãåíåðèðîâàòüÑöåíóToolStripMenuItem->Text = L"Ñãåíåðèðîâàòü ñöåíó";
			this->ñãåíåðèðîâàòüÑöåíóToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::ñãåíåðèðîâàòüÑöåíóToolStripMenuItem_Click);
			// 
			// çàïóñòèòüÑèìóëÿöèþToolStripMenuItem
			// 
			this->çàïóñòèòüÑèìóëÿöèþToolStripMenuItem->Enabled = false;
			this->çàïóñòèòüÑèìóëÿöèþToolStripMenuItem->Name = L"çàïóñòèòüÑèìóëÿöèþToolStripMenuItem";
			this->çàïóñòèòüÑèìóëÿöèþToolStripMenuItem->Size = System::Drawing::Size(206, 22);
			this->çàïóñòèòüÑèìóëÿöèþToolStripMenuItem->Text = L"Çàïóñòèòü ñèìóëÿöèþ";
			this->çàïóñòèòüÑèìóëÿöèþToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::çàïóñòèòüÑèìóëÿöèþToolStripMenuItem_Click);
			// 
			// îñòàíîâèòüÑèìóëÿöèþToolStripMenuItem
			// 
			this->îñòàíîâèòüÑèìóëÿöèþToolStripMenuItem->Enabled = false;
			this->îñòàíîâèòüÑèìóëÿöèþToolStripMenuItem->Name = L"îñòàíîâèòüÑèìóëÿöèþToolStripMenuItem";
			this->îñòàíîâèòüÑèìóëÿöèþToolStripMenuItem->Size = System::Drawing::Size(206, 22);
			this->îñòàíîâèòüÑèìóëÿöèþToolStripMenuItem->Text = L"Îñòàíîâèòü ñèìóëÿöèþ";
			this->îñòàíîâèòüÑèìóëÿöèþToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::îñòàíîâèòüÑèìóëÿöèþToolStripMenuItem_Click);
			// 
			// êàìåðàToolStripMenuItem
			// 
			this->êàìåðàToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->îáùèéÂèäToolStripMenuItem, 
				this->âèäÍàÎáúåêòToolStripMenuItem});
			this->êàìåðàToolStripMenuItem->Enabled = false;
			this->êàìåðàToolStripMenuItem->Name = L"êàìåðàToolStripMenuItem";
			this->êàìåðàToolStripMenuItem->Size = System::Drawing::Size(60, 20);
			this->êàìåðàToolStripMenuItem->Text = L"Êàìåðà";
			// 
			// îáùèéÂèäToolStripMenuItem
			// 
			this->îáùèéÂèäToolStripMenuItem->Name = L"îáùèéÂèäToolStripMenuItem";
			this->îáùèéÂèäToolStripMenuItem->Size = System::Drawing::Size(151, 22);
			this->îáùèéÂèäToolStripMenuItem->Text = L"Îáùèé âèä";
			this->îáùèéÂèäToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::îáùèéÂèäToolStripMenuItem_Click);
			// 
			// âèäÍàÎáúåêòToolStripMenuItem
			// 
			this->âèäÍàÎáúåêòToolStripMenuItem->Name = L"âèäÍàÎáúåêòToolStripMenuItem";
			this->âèäÍàÎáúåêòToolStripMenuItem->Size = System::Drawing::Size(151, 22);
			this->âèäÍàÎáúåêòToolStripMenuItem->Text = L"Âèä íà îáúåêò";
			this->âèäÍàÎáúåêòToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::âèäÍàÎáúåêòToolStripMenuItem_Click);
			// 
			// transToolStripMenuItem
			// 
			this->transToolStripMenuItem->Enabled = false;
			this->transToolStripMenuItem->Name = L"transToolStripMenuItem";
			this->transToolStripMenuItem->Size = System::Drawing::Size(145, 20);
			this->transToolStripMenuItem->Text = L"Çàïóñòèòü òðàíñëÿöèþ";
			this->transToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::transToolStripMenuItem_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(720, 399);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyDown);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void panel1_Resize(System::Object^  sender, System::EventArgs^  e) {
				 if(bGenerated) {sv.Resize(panel1->Width, panel1->Height); sv.Update();}
			 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e)
			{
				 Settings = gcnew SettingsForm();
				 Settings->mData = gcnew SettingsForm::RadarData(this, &Interface::Form1::SetDefaultRadarData);				 
			}

private: System::Void listToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 sv.StopRendering();
		 }

private: System::Void panel1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 if(bGenerated)
			 {
				 bMouse =true;
				 old = e->Location;
			 }
		 }

private: System::Void panel1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 if(bMouse)
			 {
				 now = e->Location;
				 int x = old.X - now.X;
				 int y = old.Y - now.Y;
				 old = now;
				 sv.MouseMove(x, y, 0);
				 sv.Update();
			 }
		 }

private: System::Void panel1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 bMouse = false;
		 }

private: System::Void panel1_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e)
		 {
			 if(bGenerated)
			 {		
				 int d = e->OldValue - e->NewValue;			 
				 sv.MouseMove(0, d*100, 0);
				 sv.Update();
			 }
			 
		 }

private: System::Void Form1_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if(bGenerated)
			 {
				 if(e->KeyValue ==  VK_UP)
				 {
					 sv.KeyPressed(0,0,-25);
				 }
				 else if(e->KeyValue == VK_DOWN)
				 {
					 sv.KeyPressed(0,0,25);
				 }
				 else if(e->KeyValue ==VK_LEFT)
				 {
					 sv.KeyPressed(-25,0,0);
				 }
				 else if(e->KeyValue ==VK_RIGHT)
				 {
					 sv.KeyPressed(25,0,0);
				 }
		 
				 sv.Update();	
			 }
		 }

private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
		 {
			 Settings->bClose = true;
			 if(bCreate)
			 {
				 sv.StopRendering();
				 sv.DeleteAllObjectes();
				 sv.Close();
			 }
		 }

private: System::Void ShipToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {

			 ShipForm = gcnew ShipParam(1);
			 ShipForm->mData = gcnew ShipParam::ShipData(this, &Interface::Form1::GetShipData);
			 ShipForm->mMove = gcnew ShipParam::StartShipMoving(this, &Interface::Form1::StartShipMoving);
			 ShipForm->Show();
		 }

private: System::Void GetShipData(int nType)
		 {
			 int x; int y; int h; int speed; int vk;
			 sv.GetObjectData(nType, x, y, h, speed, vk, vk, vk);
			 switch(nType)
				 {
				 case 0 :
					 break;
				 case 1 :		 //êîðàáëü
					 //sv.GetShipData(x, y, speed);					 
					 ShipForm->SetData(x, y, h, speed);
					 break;
				 case 2: //ñàìîëåò
					 //sv.GetObjectData(nType, x, y, h, speed);
					 PlaneForm->SetData(x, y, h, speed);
					 break;
				 case 3: //ðàêåòà
					 break;
				 default :
					 break;
				 }
			 
		 }
private: System::Void StartShipMoving(int nType)
		 {
			 int x; int y; int h; int speed;
			 switch(nType)
				 {
				 case 0 :
					 break;
				 case 1 :		 //êîðàáëü
					 ShipForm->GetDestination(x, y, h, speed);
					 sv.SetShipDestination(x, y, speed);
					 break;
				 case 2: //ñàìîëåò
					 PlaneForm->GetDestination(x, y, h, speed);
					 sv.SetObjectDestination(2, x, y, h, speed);
					 break;
				 case 3: //ðàêåòà
					 MissileForm->GetDestination(x, y, h, speed);
					 sv.SetObjectDestination(3, x, y, h, speed);
					 break;
				 default :
					 break;
				 }
			 
		 }
private: System::Void ñàìîëåòToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 PlaneForm = gcnew ShipParam(2);
			 PlaneForm->mData = gcnew ShipParam::ShipData(this, &Interface::Form1::GetShipData);
			 PlaneForm->mMove = gcnew ShipParam::StartShipMoving(this, &Interface::Form1::StartShipMoving);
			 PlaneForm->Show();

		 }
private: System::Void ðàêåòàToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			MissileForm = gcnew ShipParam(3);
			MissileForm->mData = gcnew ShipParam::ShipData(this, &Interface::Form1::GetShipData);
			MissileForm->mMove = gcnew ShipParam::StartShipMoving(this, &Interface::Form1::StartShipMoving);
			MissileForm->Show();
		 }
private: System::Void UpdateData(int RadarNumber)
		 {
			 int x; int y; int h; int v; int vx; int vy; int vh;
			 RadarForm^ form;
			 switch(RadarNumber)
				 {
				 case 0 :
					 break;
				 case 1 :	
					 form = Radar1;
					 break;
				 case 2: 
					 form = Radar2;
					 break;
				 case 3: 
					 form = Radar3;
					 break;
				 default :
					 break;
				 }
			 sv.GetRadarData(RadarNumber, x, y, h, v);
			 form->UpdateRadarData(x, y, h, v);
			 sv.GetObjectData(1, x, y, h, v, vx, vy, vh);
			 form->UpdateShipData(x, y, h, v, vx, vy, vh);
			 sv.GetObjectData(2, x, y, h, v, vx, vy, vh);
			 form-> UpdatePlaneData(x, y, h, v, vx, vy, vh);
			 sv.GetObjectData(3, x, y, h, v, vx, vy, vh);
			 form->UpdateMissileData(x, y, h, v, vx, vy, vh);
		 }
private: System::Void ðàäàð1ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 //Ðàäàð ¹1
			 Radar1 = gcnew RadarForm(1);
			 Radar1->mData = gcnew RadarForm::GetData(this, &Interface::Form1::UpdateData);
			 Radar1->Show();
		 }		 
private: System::Void ðàäàð2ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 //Ðàäàð ¹2
			 Radar2 = gcnew RadarForm(2);
			 Radar2->mData = gcnew RadarForm::GetData(this, &Interface::Form1::UpdateData);
			 Radar2->Show();
		 }
private: System::Void ðàäàð3ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 //Ðàäàð ¹3
			 Radar3 = gcnew RadarForm(3);
			 Radar3->DisableRadarKoordEdit();
			 Radar3->mData = gcnew RadarForm::GetData(this, &Interface::Form1::UpdateData);
			 Radar3->Show();
		 }

private: System::Void îòêðûòüToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) //Âûáîð ïðîåêòà		 
		 {
			 // Îòêðûòü ïðîåêò
			 OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
			 
			 openFileDialog1->InitialDirectory = System::IO::Directory::GetCurrentDirectory() + "\\Data";
			 openFileDialog1->Filter = "xml files (*.xml)|*.xml";
			 //openFileDialog1->FilterIndex = 2;
			 openFileDialog1->RestoreDirectory = true;
			 
			 if ( openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK )
			 {
				 Settings = gcnew SettingsForm;
				 Settings->mData = gcnew SettingsForm::RadarData(this, &Interface::Form1::SetDefaultRadarData);
				 Settings->bLoad = true;
				 Settings->FileName = openFileDialog1->FileName;
				 Settings->Show();
			 }

			 ñîõðàíèòüÏðîåêòToolStripMenuItem->Enabled = true;
			 ñèìóëÿöèÿToolStripMenuItem->Enabled = true;
		 }
private: System::Void ñîõðàíèòüÏðîåêòToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 // Ñîõðàíèòü ïðîåêò
			 SaveFileDialog^ saveFileDialog1 = gcnew SaveFileDialog;
			 saveFileDialog1->InitialDirectory = System::IO::Directory::GetCurrentDirectory() + "\\Data";
			 saveFileDialog1->Filter = "xml files (*.xml)|*.xml";
			 //saveFileDialog1->FilterIndex = 2;
			 saveFileDialog1->RestoreDirectory = true;
			 if ( saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK )
			 {
				 SaveProject(saveFileDialog1->FileName);
			 }


		 }
private: System::Void SaveProject(String^ FileName)
		 {
			  System::Xml::XmlTextWriter^ Wr = gcnew  System::Xml::XmlTextWriter( FileName, System::Text::Encoding::GetEncoding("UTF-8"));
			  System::Xml::XmlDocument^ Xml = gcnew  System::Xml::XmlDocument();

			  Wr->WriteStartDocument();
			  Wr->WriteStartElement("Data");
			  Wr->WriteStartElement("ObjectData");
			  Wr->WriteAttributeString("ObjectNumber", GetObjectsNumber().ToString());

			  for(int i = 0; i!=GetObjectsNumber(); i++)
			  {
				  Wr->WriteStartElement("ObjectData"+i.ToString());
				  Wr->WriteAttributeString("ObjectType", GetnType(Settings->dataGridView1->Rows[i]->Cells[2]->Value->ToString()).ToString());
				  Wr->WriteAttributeString("ObjectName", Settings->dataGridView1->Rows[i]->Cells[3]->Value->ToString());
				  Wr->WriteAttributeString("Radar",  Convert::ToBoolean(Settings->dataGridView1->Rows[i]->Cells[4]->Value).ToString());
				  Wr->WriteAttributeString("X0", Settings->dataGridView1->Rows[i]->Cells[5]->Value->ToString());
				  Wr->WriteAttributeString("Y0", Settings->dataGridView1->Rows[i]->Cells[6]->Value->ToString());
				  Wr->WriteAttributeString("H0", Settings->dataGridView1->Rows[i]->Cells[7]->Value->ToString());
				  Wr->WriteAttributeString("Color",  Settings->dataGridView1->Rows[i]->Cells[8]->Style->BackColor.ToArgb().ToString());
				  
				  int nRoutes = Settings->Routes[i]->Rows->Count-1;
				  Wr->WriteAttributeString("RoutesNumber", nRoutes.ToString());
				  
				  for (int n = 0; n!=nRoutes; n++)
				  {		
					  Wr->WriteStartElement("Route"+n.ToString());
					  Wr->WriteAttributeString("Speed", Settings->Routes[i]->Rows[n]->Cells[2]->Value->ToString());
					  Wr->WriteAttributeString("X", Settings->Routes[i]->Rows[n]->Cells[3]->Value->ToString());
					  Wr->WriteAttributeString("Y", Settings->Routes[i]->Rows[n]->Cells[4]->Value->ToString());
					  Wr->WriteAttributeString("H", Settings->Routes[i]->Rows[n]->Cells[5]->Value->ToString());
					  Wr->WriteEndElement(); //Route N
				  }

				  Wr->WriteEndElement(); //ObjectData N
			  }
			  Wr->WriteEndElement(); //ObjectData

			  Wr->WriteStartElement("RadarData");
			  for(int i = 0; i!=20; i++)
			  {
				   Wr->WriteStartElement("Radar"+i.ToString());
				   Wr->WriteAttributeString("Active", Convert::ToBoolean(Settings->dataGridView3->Rows[i]->Cells[0]->Value).ToString());
				   Wr->WriteAttributeString("RadarName", Settings->dataGridView3->Rows[i]->Cells[1]->Value->ToString());
				   Wr->WriteAttributeString("X", Settings->dataGridView3->Rows[i]->Cells[2]->Value->ToString());
				   Wr->WriteAttributeString("Y", Settings->dataGridView3->Rows[i]->Cells[3]->Value->ToString());
				   Wr->WriteAttributeString("H", Settings->dataGridView3->Rows[i]->Cells[4]->Value->ToString());
				   Wr->WriteAttributeString("AddressName", Settings->dataGridView3->Rows[i]->Cells[5]->Value->ToString());
				   Wr->WriteAttributeString("Port", Settings->dataGridView3->Rows[i]->Cells[6]->Value->ToString());
				   Wr->WriteAttributeString("TimeInterval", Settings->dataGridView3->Rows[i]->Cells[7]->Value->ToString());
				   				   
				   Wr->WriteEndElement(); //Radar N				  
			  }

			  Wr->WriteEndElement(); //RadarData
			  Wr->WriteEndElement(); //Data
			  Wr->WriteEndDocument();
			  Wr->Flush();
			  Wr->Close();

			  Xml->Load(FileName);
			  Xml->Save(FileName); 
		 }
private: System::Void íîâûéÏðîåêòToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 // Íîâûé ïðîåêò
		 {
			 
			 if (MessageBox::Show("Çàãðóçèòü òåñòîâûé ïðîåêò?","Ñîçäàíèå ïðîåêòà", System::Windows::Forms::MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::Yes)
			 {
				 DefaultProject();
				 
			 }
			 Settings->Show();

			 ñîõðàíèòüÏðîåêòToolStripMenuItem->Enabled = true;
			 ñèìóëÿöèÿToolStripMenuItem->Enabled = true;
		 }

private: System::Void DefaultProject()
		 {
			 Settings->bDefault = true;
			 Settings->AddObject(1, "Êîðàáëü 1", false, 0, 0, 0);
			 Settings->AddRoute(0, 16, -15000, -130000, 0);
			 Settings->AddRoute(0, 12, 130000, -130000, 0);
			 Settings->AddRoute(0, 15, 130000, -60000, 0);
			 Settings->AddRoute(0, 10, 0, 0, 0);

			 Settings->AddObject(2, "Ñàìîëåò 1", false, 130000, 130000, 5000);
			 Settings->AddRoute(1, 150, -75000, 70000, 3000);
			 Settings->AddRoute(1, 200, 0, 0, 8000);
			 Settings->AddRoute(1, 250, 60000, -90000, 10000);
			 Settings->AddRoute(1, 115, 130000, 130000, 5000);

			 Settings->AddObject(3, "Ðàêåòà 1", false, -130000, -130000, 4000);
			 Settings->AddRoute(2, 300, 60000, -90000, 3500);
			 Settings->AddRoute(2, 300, 0, 0, 3000);
			 Settings->AddRoute(2, 300, -75000, 70000, 4500);
			 Settings->AddRoute(2, 300, -130000, -130000, 4000);

			 Settings->dataGridView1->Rows[Settings->dataGridView1->Rows->Count-1]->Cells[1]->Value = 4;
			 Settings->dataGridView1->Rows[Settings->dataGridView1->Rows->Count-1]->Cells[0]->Value = 0;
		 
		 }

private : System::Void SetDefaultRadarData()
		  {
			 Settings->AddRadar(0, "Ðàäàð 1", -75000, 70000, 1495);
			 Settings->AddRadar(1, "Ðàäàð 2", 60000, -90000, 1495);
		  }

private: System::Void íàñòðîéêèÑèìóëÿöèèToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 //Íàñòðîéêè ñèìóëÿöèè
			 Settings->Show();
		 }
private: System::Void çàïóñòèòüÑèìóëÿöèþToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 //Çàïóñòèòü ñèìóëÿöèþ
			 íàñòðîéêèÑèìóëÿöèèToolStripMenuItem->Enabled = false;
			 çàïóñòèòüÑèìóëÿöèþToolStripMenuItem->Enabled = false;
			 îñòàíîâèòüÑèìóëÿöèþToolStripMenuItem->Enabled = true;
			 êàìåðàToolStripMenuItem->Enabled = true;
			 transToolStripMenuItem->Enabled = true;
			 
			 
			 err = 0;
			 try{
				 err=1;
			 AddObjectesAndRoutes();
			 err=2;
			 AddRadars();
			 }
			 catch(...) {MessageBox::Show(err.ToString());}

			 bCreate = true;
			
			 //Thread::Sleep(5000);
			 sv.LookAll();
			 sv.StartAll();
			 sv.Update();
			 sv.StartRendering();
			 
		 }

public: int GetObjectsNumber() { return Settings->dataGridView1->Rows->Count - 1; }
		

private: System::Void AddObjectesAndRoutes()
		 {
			 err=10;
			 int nObjectes = GetObjectsNumber();
			 err=12;
			 //MessageBox::Show(nObjectes.ToString());
			 sv.SetNumberObject(nObjectes);
			 err = 11;
			 for (int i = 0; i!=nObjectes; i++)
			 {
				 //MessageBox::Show(i.ToString());
				 
				 int ObjectType = GetnType(Settings->dataGridView1->Rows[i]->Cells[2]->Value->ToString());
				 err=20+i;
				 sv.AddObject(i,
					          ObjectType, 
							  Convert::ToBoolean(Settings->dataGridView1->Rows[i]->Cells[4]->Value), 
							   Convert::ToInt32(Settings->dataGridView1->Rows[i]->Cells[5]->Value),
							   Convert::ToInt32(Settings->dataGridView1->Rows[i]->Cells[6]->Value),
							   Convert::ToInt32(Settings->dataGridView1->Rows[i]->Cells[7]->Value),
							   Convert::ToInt32(Settings->dataGridView1->Rows[i]->Cells[8]->Style->BackColor.ToArgb()));
				 
				 if(ObjectType!=0)
				 {
					 int nRoutes = Settings->Routes[i]->Rows->Count-1;
					 sv.SetNRoute(i, nRoutes);
					 for (int n = 0; n!=nRoutes; n++)
					 {
						 err = 30+n;
						 sv.AddObjectRoute(i, 
						               Convert::ToInt32(Settings->Routes[i]->Rows[n]->Cells[2]->Value),
									   Convert::ToInt32(Settings->Routes[i]->Rows[n]->Cells[3]->Value),
									   Convert::ToInt32(Settings->Routes[i]->Rows[n]->Cells[4]->Value),
									   Convert::ToInt32(Settings->Routes[i]->Rows[n]->Cells[5]->Value));
					 }
				 }
			 }
		 }

private: System::Void AddRadars()
		 {
			int nObjectes = 0;
			int n = 0;
			
			for each (System::Windows::Forms::DataGridViewRow^ row in Settings->dataGridView3->Rows)
			{
				if(Convert::ToBoolean(row->Cells[0]->Value))
				{
					nObjectes++;
				}
			}

			 sv.SetNumberRadar(nObjectes);
			 for (int i = 0; i!=20; i++)
			 {
				 if(Convert::ToBoolean(Settings->dataGridView3->Rows[i]->Cells[0]->Value))
				 {
					 sv.AddRadar(n, 
							  Convert::ToInt32( Settings->dataGridView3->Rows[i]->Cells[2]->Value), //X
							  Convert::ToInt32( Settings->dataGridView3->Rows[i]->Cells[3]->Value), //Y
							  Convert::ToInt32( Settings->dataGridView3->Rows[i]->Cells[4]->Value) //H
							  );
					 n++;
				 }
			 }		 
		 }

public: String^ GetRadarData(int RadarNumber)
		{
			double x, y, h;
			sv.GetRadarData(RadarNumber, x, y, h);
		   //òèï îáúåêòà, x,                     y,                     z,                 vx, vy, vz
			System::Char a = 0;
			String ^ n = "0";
			String ^sx, ^sy, ^sh;
			sx = x.ToString();
			sx = sx->Replace(",", ".");
			sy = y.ToString();
			sy = sy->Replace(",", ".");
			sh = h.ToString();
			sh = sh->Replace(",", ".");
			String^ sentence;
			sentence = n + "," + sx + "," + sy + "," + sh + ",0.0,0.0,0.0;";
			//MessageBox::Show(sentence);
			return sentence;
		}

public: String^ GetObjectData(int ObjectNumber)
		{
			double x, y, h;
			int nType;
			double vx, vy, vh;
			sv.GetObjectData(ObjectNumber, nType, x, y, h, vx, vy, vh);
			System::Char a = nType;
			String ^ n = nType.ToString();
			String^ svx, ^ svy, ^ svh, ^sx, ^sy, ^sh;
			sx = x.ToString();
			sx = sx->Replace(",", ".");
			sy = y.ToString();
			sy = sy->Replace(",", ".");
			sh = h.ToString();
			sh = sh->Replace(",", ".");
			svx = vx.ToString();
			svx = svx->Replace(",", ".");
			svy = vy.ToString();
			svy = svy->Replace(",", ".");
			svh = vh.ToString();
			svh = svh->Replace(",", ".");
			String^ sentence;
			sentence = n + "," + sx + "," + sy + "," + sh + "," + svx + "," + svy + "," + svh + ";";
			//MessageBox::Show(sentence);
			return sentence;
		}

private: int GetnType(String^ sType)
		{
			int nType = 0;
			if(sType=="Ðàäàð")        { nType = 0; }
			else if(sType=="Êîðàáëü") { nType = 1; }
			else if(sType=="Ñàìîëåò") { nType = 2; }
			else if(sType=="Ðàêåòà")  { nType = 3; }

			return nType;
		}

private: System::Void îñòàíîâèòüÑèìóëÿöèþToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 //Îñòàíîâèòü ñèìóëÿöèþ
			 íàñòðîéêèÑèìóëÿöèèToolStripMenuItem->Enabled = true;
			 çàïóñòèòüÑèìóëÿöèþToolStripMenuItem->Enabled = true;
			 îñòàíîâèòüÑèìóëÿöèþToolStripMenuItem->Enabled = false;

			 transToolStripMenuItem->Text="Çàïóñòèòü òðàíñëÿöèþ";
			 transToolStripMenuItem->Enabled = false;
			 êàìåðàToolStripMenuItem->Enabled = false;
			 bWork = false;
			 bCreate = false;

			 sv.StopRendering();
			 sv.DeleteAllObjectes();		 
			 sv.Update();
		 }
private: System::Void transToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 //Çàïóñòèòü èëè îñòàíîâèòü òðàíñëÿöèþ äàííûõ ïî ñåòè	 
			 if(bWork==false)//Çàïóñòèòü
			 {				 
				 bWork = true;
				 int nRadar = 0;	
				 for (int i = 0; i!=20; i++)
				 {
					 if(Convert::ToBoolean(Settings->dataGridView3->Rows[i]->Cells[0]->Value))
					 {
						  RadarThread[i] = gcnew Thread(gcnew ParameterizedThreadStart(&DelegateThreadTask));
						  MyParams^ p;
						  p = gcnew MyParams();
						  p->RadarNumber = nRadar;
						  p->obj = this;
						  p->HostName = Settings->dataGridView3->Rows[i]->Cells[5]->Value->ToString();
						  p->PortNumber =  Convert::ToInt32( Settings->dataGridView3->Rows[i]->Cells[6]->Value);
						  p->nTimeOut = Convert::ToInt32 (Settings->dataGridView3->Rows[i]->Cells[7]->Value);
						  RadarThread[i]->Start(p);	
						  nRadar ++;
					 }
				 }
				 transToolStripMenuItem->Text="Îñòàíîâèòü òðàíñëÿöèþ";				 
			 }
			 else//Îñòàíîâèòü
			 {
				 bWork = false;
				 transToolStripMenuItem->Text="Çàïóñòèòü òðàíñëÿöèþ";
			 }

		 }

private: System::Void ñãåíåðèðîâàòüÑöåíóToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 ñãåíåðèðîâàòüÑöåíóToolStripMenuItem->Enabled = false;
			 çàïóñòèòüÑèìóëÿöèþToolStripMenuItem->Enabled = true;

			 if(!bGenerated) { sv.SetHandle(this->panel1->Handle.ToInt32());}
			 bGenerated = true;
			 sv.LookAll();
			 sv.Update();
			 MessageBox::Show("Ñöåíà ñãåíåðèðîâàíà!");
		 }
private: System::Void ðåæèìÏðèåìàÄàííûõToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(!bGenerated)
			 { 		 
				 sv.SetHandle(this->panel1->Handle.ToInt32());
				 MessageBox::Show("Ñöåíà ñãåíåðèðîâàíà!");			 
			 }

			 bGenerated = true;
			 êàìåðàToolStripMenuItem->Enabled = true;
			 âèäÍàÎáúåêòToolStripMenuItem->Enabled = false;
			
			 sv.DeleteAllObjectes();
			 sv.LookAll();
			 sv.Update();
			 
			 
			 
			 //Ðåæèì ïðèåìà äàííûõ
			 if(bDataRecive==false)
			 {
				 
				 bDataRecive = true;
				 ðåæèìÏðèåìàÄàííûõToolStripMenuItem->Text="Îñòàíîâèòü ðåæèì ïðèåìà äàííûõ";
				 íîâûéÏðîåêòToolStripMenuItem->Enabled = false;
				 îòêðûòüToolStripMenuItem->Enabled = false;
				 ñîõðàíèòüÏðîåêòToolStripMenuItem->Enabled = false;
				 bWork = true;

				 newThread = gcnew Thread(gcnew ParameterizedThreadStart(&DelegateThreadReciveTask));
				 newThread->Start(this);
				 
				 
			 }
			 else
			 { 
				 bWork = false;
				 bDataRecive = false;
				 ðåæèìÏðèåìàÄàííûõToolStripMenuItem->Text="Ðåæèì ïðèåìà äàííûõ";
				 íîâûéÏðîåêòToolStripMenuItem->Enabled = true;
				 îòêðûòüToolStripMenuItem->Enabled = true;
				 newThread = nullptr;
				
				 
				 System::Net::Sockets::UdpClient^ udpClient = gcnew System::Net::Sockets::UdpClient;
				 udpClient->Connect( "127.0.0.1", 14001 );					 

				 SEND_DATA a;
				 a.cnt = 0;
				 array<unsigned char>^sendBytes1 = gcnew cli::array<unsigned char>(sizeof(SEND_DATA));
				 
				 char *ptr = (char *)&a;
				 for(int i = 0;i < sizeof(SEND_DATA);++i)
					 sendBytes1[i] = ptr[i];
				 
				 udpClient->Send( sendBytes1, sizeof(SEND_DATA) );

			 }
			 
			
			
		 }
private: System::Void îáùèéÂèäToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 sv.LookAll();
			 sv.Update();
		 }
private: System::Void âèäÍàÎáúåêòToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 int n;
			 n = 0;
			 SelectObject ^ SelObForm = gcnew SelectObject();
			 int nObjectes = GetObjectsNumber();
			 for (int i = 0; i!=nObjectes; i++)
			 {
				 SelObForm->comboBox1->Items->Add(Settings->dataGridView1->Rows[i]->Cells[3]->Value->ToString());
			 }
			 
			 SelObForm->mNumber = gcnew  SelectObject::ObjectNumber(this, &Interface::Form1::LookObject);
			 SelObForm->Show();
			 
		 }
private: System::Void LookObject(int n)
		 {
			 sv.LookObject(n, false);
			 sv.Update();
		 }


};



}

