#pragma once

namespace Interface {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	/// <summary>
	/// Summary for RadarForm
	/// </summary>
	public ref class RadarForm : public System::Windows::Forms::Form
	{
	public:
		long nTimeOut;
		bool bWork;
		int nRadarNumber;
	private: Thread^ newThread;
	public: static void DelegateThreadTask(System::Object ^obj)
			{
				RadarForm^ ob = (RadarForm^) obj;
				do
				{
					if (ob->mData) 	 { ob->mData(ob->nRadarNumber);}
					Thread::Sleep(ob->nTimeOut);
				} while (ob->bWork==true);
			}

	public: delegate void GetData(int RadarNumber);
	public: GetData^ mData;

		RadarForm(int RadarNumber)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			bWork = false;
			nTimeOut = 100;
			nRadarNumber = RadarNumber;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RadarForm()
		{
			if(bWork == true) {bWork = false;}
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  TBRadarX;
	private: System::Windows::Forms::TextBox^  TBRadarY;
	private: System::Windows::Forms::TextBox^  TBRadarH;
	private: System::Windows::Forms::TextBox^  TBRadarV;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::TextBox^  TBShipV;
	private: System::Windows::Forms::TextBox^  TBShipH;
	private: System::Windows::Forms::TextBox^  TBShipY;
	private: System::Windows::Forms::TextBox^  TBShipX;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::TextBox^  TBShipVH;
	private: System::Windows::Forms::TextBox^  TBShipVY;
	private: System::Windows::Forms::TextBox^  TBShipVX;
	private: System::Windows::Forms::Label^  label23;
	private: System::Windows::Forms::Label^  label24;
	private: System::Windows::Forms::Label^  label25;
	private: System::Windows::Forms::Label^  label26;
	private: System::Windows::Forms::Label^  label27;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::TextBox^  TBPlaneVH;
	private: System::Windows::Forms::TextBox^  TBPlaneVY;
	private: System::Windows::Forms::TextBox^  TBPlaneVX;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::TextBox^  TBPlaneV;
	private: System::Windows::Forms::TextBox^  TBPlaneH;
	private: System::Windows::Forms::TextBox^  TBPlaneY;
	private: System::Windows::Forms::TextBox^  TBPlaneX;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::Label^  label18;
	private: System::Windows::Forms::Label^  label19;
	private: System::Windows::Forms::Label^  label20;
	private: System::Windows::Forms::Label^  label21;
	private: System::Windows::Forms::Label^  label22;
	private: System::Windows::Forms::TextBox^  TBMissileVH;
	private: System::Windows::Forms::TextBox^  TBMissileVY;
	private: System::Windows::Forms::TextBox^  TBMissileVX;
	private: System::Windows::Forms::Label^  label28;
	private: System::Windows::Forms::Label^  label29;
	private: System::Windows::Forms::Label^  label30;
	private: System::Windows::Forms::TextBox^  TBMissileV;
	private: System::Windows::Forms::TextBox^  TBMissileH;
	private: System::Windows::Forms::TextBox^  TBMissileY;
	private: System::Windows::Forms::TextBox^  TBMissileX;
	private: System::Windows::Forms::Label^  label31;
	private: System::Windows::Forms::Label^  label32;
	private: System::Windows::Forms::Label^  label33;
	private: System::Windows::Forms::Label^  label34;
	private: System::Windows::Forms::Label^  label35;
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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->TBRadarX = (gcnew System::Windows::Forms::TextBox());
			this->TBRadarY = (gcnew System::Windows::Forms::TextBox());
			this->TBRadarH = (gcnew System::Windows::Forms::TextBox());
			this->TBRadarV = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->TBShipV = (gcnew System::Windows::Forms::TextBox());
			this->TBShipH = (gcnew System::Windows::Forms::TextBox());
			this->TBShipY = (gcnew System::Windows::Forms::TextBox());
			this->TBShipX = (gcnew System::Windows::Forms::TextBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->TBShipVH = (gcnew System::Windows::Forms::TextBox());
			this->TBShipVY = (gcnew System::Windows::Forms::TextBox());
			this->TBShipVX = (gcnew System::Windows::Forms::TextBox());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->TBPlaneVH = (gcnew System::Windows::Forms::TextBox());
			this->TBPlaneVY = (gcnew System::Windows::Forms::TextBox());
			this->TBPlaneVX = (gcnew System::Windows::Forms::TextBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->TBPlaneV = (gcnew System::Windows::Forms::TextBox());
			this->TBPlaneH = (gcnew System::Windows::Forms::TextBox());
			this->TBPlaneY = (gcnew System::Windows::Forms::TextBox());
			this->TBPlaneX = (gcnew System::Windows::Forms::TextBox());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->TBMissileVH = (gcnew System::Windows::Forms::TextBox());
			this->TBMissileVY = (gcnew System::Windows::Forms::TextBox());
			this->TBMissileVX = (gcnew System::Windows::Forms::TextBox());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->label30 = (gcnew System::Windows::Forms::Label());
			this->TBMissileV = (gcnew System::Windows::Forms::TextBox());
			this->TBMissileH = (gcnew System::Windows::Forms::TextBox());
			this->TBMissileY = (gcnew System::Windows::Forms::TextBox());
			this->TBMissileX = (gcnew System::Windows::Forms::TextBox());
			this->label31 = (gcnew System::Windows::Forms::Label());
			this->label32 = (gcnew System::Windows::Forms::Label());
			this->label33 = (gcnew System::Windows::Forms::Label());
			this->label34 = (gcnew System::Windows::Forms::Label());
			this->label35 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(51, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(87, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Данные радара";
			this->label1->Click += gcnew System::EventHandler(this, &RadarForm::label1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(16, 43);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(14, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"X";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(124, 43);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(14, 13);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Y";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(228, 43);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(15, 13);
			this->label4->TabIndex = 3;
			this->label4->Text = L"H";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(17, 69);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(55, 13);
			this->label5->TabIndex = 4;
			this->label5->Text = L"Скорость";
			// 
			// TBRadarX
			// 
			this->TBRadarX->Location = System::Drawing::Point(37, 39);
			this->TBRadarX->Name = L"TBRadarX";
			this->TBRadarX->Size = System::Drawing::Size(78, 20);
			this->TBRadarX->TabIndex = 5;
			// 
			// TBRadarY
			// 
			this->TBRadarY->Location = System::Drawing::Point(144, 39);
			this->TBRadarY->Name = L"TBRadarY";
			this->TBRadarY->Size = System::Drawing::Size(78, 20);
			this->TBRadarY->TabIndex = 6;
			// 
			// TBRadarH
			// 
			this->TBRadarH->Location = System::Drawing::Point(249, 39);
			this->TBRadarH->Name = L"TBRadarH";
			this->TBRadarH->Size = System::Drawing::Size(78, 20);
			this->TBRadarH->TabIndex = 7;
			// 
			// TBRadarV
			// 
			this->TBRadarV->Enabled = false;
			this->TBRadarV->Location = System::Drawing::Point(78, 65);
			this->TBRadarV->Name = L"TBRadarV";
			this->TBRadarV->Size = System::Drawing::Size(78, 20);
			this->TBRadarV->TabIndex = 8;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(345, 38);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 9;
			this->button1->Text = L"Задать";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &RadarForm::button1_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(16, 430);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(105, 13);
			this->label6->TabIndex = 10;
			this->label6->Text = L"Обновлять каждые";
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(127, 426);
			this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(67, 20);
			this->numericUpDown1->TabIndex = 11;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(201, 430);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(74, 13);
			this->label7->TabIndex = 12;
			this->label7->Text = L"миллисекунд";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(281, 425);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 13;
			this->button2->Text = L"Старт";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &RadarForm::button2_Click);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(54, 100);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(93, 13);
			this->label8->TabIndex = 14;
			this->label8->Text = L"Данные корабля";
			// 
			// TBShipV
			// 
			this->TBShipV->Location = System::Drawing::Point(78, 151);
			this->TBShipV->Name = L"TBShipV";
			this->TBShipV->Size = System::Drawing::Size(78, 20);
			this->TBShipV->TabIndex = 22;
			// 
			// TBShipH
			// 
			this->TBShipH->Location = System::Drawing::Point(249, 125);
			this->TBShipH->Name = L"TBShipH";
			this->TBShipH->Size = System::Drawing::Size(78, 20);
			this->TBShipH->TabIndex = 21;
			// 
			// TBShipY
			// 
			this->TBShipY->Location = System::Drawing::Point(144, 125);
			this->TBShipY->Name = L"TBShipY";
			this->TBShipY->Size = System::Drawing::Size(78, 20);
			this->TBShipY->TabIndex = 20;
			// 
			// TBShipX
			// 
			this->TBShipX->Location = System::Drawing::Point(37, 125);
			this->TBShipX->Name = L"TBShipX";
			this->TBShipX->Size = System::Drawing::Size(78, 20);
			this->TBShipX->TabIndex = 19;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(17, 155);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(55, 13);
			this->label9->TabIndex = 18;
			this->label9->Text = L"Скорость";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(228, 129);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(15, 13);
			this->label10->TabIndex = 17;
			this->label10->Text = L"H";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(124, 129);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(14, 13);
			this->label11->TabIndex = 16;
			this->label11->Text = L"Y";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(16, 129);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(14, 13);
			this->label12->TabIndex = 15;
			this->label12->Text = L"X";
			// 
			// TBShipVH
			// 
			this->TBShipVH->Location = System::Drawing::Point(249, 177);
			this->TBShipVH->Name = L"TBShipVH";
			this->TBShipVH->Size = System::Drawing::Size(78, 20);
			this->TBShipVH->TabIndex = 46;
			// 
			// TBShipVY
			// 
			this->TBShipVY->Location = System::Drawing::Point(144, 177);
			this->TBShipVY->Name = L"TBShipVY";
			this->TBShipVY->Size = System::Drawing::Size(78, 20);
			this->TBShipVY->TabIndex = 45;
			// 
			// TBShipVX
			// 
			this->TBShipVX->Location = System::Drawing::Point(37, 177);
			this->TBShipVX->Name = L"TBShipVX";
			this->TBShipVX->Size = System::Drawing::Size(78, 20);
			this->TBShipVX->TabIndex = 44;
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Location = System::Drawing::Point(228, 181);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(22, 13);
			this->label23->TabIndex = 43;
			this->label23->Text = L"VH";
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Location = System::Drawing::Point(124, 181);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(21, 13);
			this->label24->TabIndex = 42;
			this->label24->Text = L"VY";
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Location = System::Drawing::Point(16, 181);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(21, 13);
			this->label25->TabIndex = 41;
			this->label25->Text = L"VX";
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Location = System::Drawing::Point(162, 69);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(26, 13);
			this->label26->TabIndex = 47;
			this->label26->Text = L"м/с";
			// 
			// label27
			// 
			this->label27->AutoSize = true;
			this->label27->Location = System::Drawing::Point(162, 154);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(26, 13);
			this->label27->TabIndex = 48;
			this->label27->Text = L"м/с";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(162, 259);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(26, 13);
			this->label13->TabIndex = 64;
			this->label13->Text = L"м/с";
			// 
			// TBPlaneVH
			// 
			this->TBPlaneVH->Location = System::Drawing::Point(249, 282);
			this->TBPlaneVH->Name = L"TBPlaneVH";
			this->TBPlaneVH->Size = System::Drawing::Size(78, 20);
			this->TBPlaneVH->TabIndex = 63;
			// 
			// TBPlaneVY
			// 
			this->TBPlaneVY->Location = System::Drawing::Point(144, 282);
			this->TBPlaneVY->Name = L"TBPlaneVY";
			this->TBPlaneVY->Size = System::Drawing::Size(78, 20);
			this->TBPlaneVY->TabIndex = 62;
			// 
			// TBPlaneVX
			// 
			this->TBPlaneVX->Location = System::Drawing::Point(37, 282);
			this->TBPlaneVX->Name = L"TBPlaneVX";
			this->TBPlaneVX->Size = System::Drawing::Size(78, 20);
			this->TBPlaneVX->TabIndex = 61;
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(228, 286);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(22, 13);
			this->label14->TabIndex = 60;
			this->label14->Text = L"VH";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(124, 286);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(21, 13);
			this->label15->TabIndex = 59;
			this->label15->Text = L"VY";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(16, 286);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(21, 13);
			this->label16->TabIndex = 58;
			this->label16->Text = L"VX";
			// 
			// TBPlaneV
			// 
			this->TBPlaneV->Location = System::Drawing::Point(78, 256);
			this->TBPlaneV->Name = L"TBPlaneV";
			this->TBPlaneV->Size = System::Drawing::Size(78, 20);
			this->TBPlaneV->TabIndex = 57;
			// 
			// TBPlaneH
			// 
			this->TBPlaneH->Location = System::Drawing::Point(249, 230);
			this->TBPlaneH->Name = L"TBPlaneH";
			this->TBPlaneH->Size = System::Drawing::Size(78, 20);
			this->TBPlaneH->TabIndex = 56;
			// 
			// TBPlaneY
			// 
			this->TBPlaneY->Location = System::Drawing::Point(144, 230);
			this->TBPlaneY->Name = L"TBPlaneY";
			this->TBPlaneY->Size = System::Drawing::Size(78, 20);
			this->TBPlaneY->TabIndex = 55;
			// 
			// TBPlaneX
			// 
			this->TBPlaneX->Location = System::Drawing::Point(37, 230);
			this->TBPlaneX->Name = L"TBPlaneX";
			this->TBPlaneX->Size = System::Drawing::Size(78, 20);
			this->TBPlaneX->TabIndex = 54;
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(17, 260);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(55, 13);
			this->label17->TabIndex = 53;
			this->label17->Text = L"Скорость";
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(228, 234);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(15, 13);
			this->label18->TabIndex = 52;
			this->label18->Text = L"H";
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(124, 234);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(14, 13);
			this->label19->TabIndex = 51;
			this->label19->Text = L"Y";
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(16, 234);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(14, 13);
			this->label20->TabIndex = 50;
			this->label20->Text = L"X";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(54, 205);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(100, 13);
			this->label21->TabIndex = 49;
			this->label21->Text = L"Данные самолета";
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(162, 363);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(26, 13);
			this->label22->TabIndex = 80;
			this->label22->Text = L"м/с";
			// 
			// TBMissileVH
			// 
			this->TBMissileVH->Location = System::Drawing::Point(249, 386);
			this->TBMissileVH->Name = L"TBMissileVH";
			this->TBMissileVH->Size = System::Drawing::Size(78, 20);
			this->TBMissileVH->TabIndex = 79;
			// 
			// TBMissileVY
			// 
			this->TBMissileVY->Location = System::Drawing::Point(144, 386);
			this->TBMissileVY->Name = L"TBMissileVY";
			this->TBMissileVY->Size = System::Drawing::Size(78, 20);
			this->TBMissileVY->TabIndex = 78;
			// 
			// TBMissileVX
			// 
			this->TBMissileVX->Location = System::Drawing::Point(37, 386);
			this->TBMissileVX->Name = L"TBMissileVX";
			this->TBMissileVX->Size = System::Drawing::Size(78, 20);
			this->TBMissileVX->TabIndex = 77;
			// 
			// label28
			// 
			this->label28->AutoSize = true;
			this->label28->Location = System::Drawing::Point(228, 390);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(22, 13);
			this->label28->TabIndex = 76;
			this->label28->Text = L"VH";
			// 
			// label29
			// 
			this->label29->AutoSize = true;
			this->label29->Location = System::Drawing::Point(124, 390);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(21, 13);
			this->label29->TabIndex = 75;
			this->label29->Text = L"VY";
			// 
			// label30
			// 
			this->label30->AutoSize = true;
			this->label30->Location = System::Drawing::Point(16, 390);
			this->label30->Name = L"label30";
			this->label30->Size = System::Drawing::Size(21, 13);
			this->label30->TabIndex = 74;
			this->label30->Text = L"VX";
			// 
			// TBMissileV
			// 
			this->TBMissileV->Location = System::Drawing::Point(78, 360);
			this->TBMissileV->Name = L"TBMissileV";
			this->TBMissileV->Size = System::Drawing::Size(78, 20);
			this->TBMissileV->TabIndex = 73;
			// 
			// TBMissileH
			// 
			this->TBMissileH->Location = System::Drawing::Point(249, 334);
			this->TBMissileH->Name = L"TBMissileH";
			this->TBMissileH->Size = System::Drawing::Size(78, 20);
			this->TBMissileH->TabIndex = 72;
			// 
			// TBMissileY
			// 
			this->TBMissileY->Location = System::Drawing::Point(144, 334);
			this->TBMissileY->Name = L"TBMissileY";
			this->TBMissileY->Size = System::Drawing::Size(78, 20);
			this->TBMissileY->TabIndex = 71;
			// 
			// TBMissileX
			// 
			this->TBMissileX->Location = System::Drawing::Point(37, 334);
			this->TBMissileX->Name = L"TBMissileX";
			this->TBMissileX->Size = System::Drawing::Size(78, 20);
			this->TBMissileX->TabIndex = 70;
			// 
			// label31
			// 
			this->label31->AutoSize = true;
			this->label31->Location = System::Drawing::Point(17, 364);
			this->label31->Name = L"label31";
			this->label31->Size = System::Drawing::Size(55, 13);
			this->label31->TabIndex = 69;
			this->label31->Text = L"Скорость";
			// 
			// label32
			// 
			this->label32->AutoSize = true;
			this->label32->Location = System::Drawing::Point(228, 338);
			this->label32->Name = L"label32";
			this->label32->Size = System::Drawing::Size(15, 13);
			this->label32->TabIndex = 68;
			this->label32->Text = L"H";
			// 
			// label33
			// 
			this->label33->AutoSize = true;
			this->label33->Location = System::Drawing::Point(124, 338);
			this->label33->Name = L"label33";
			this->label33->Size = System::Drawing::Size(14, 13);
			this->label33->TabIndex = 67;
			this->label33->Text = L"Y";
			// 
			// label34
			// 
			this->label34->AutoSize = true;
			this->label34->Location = System::Drawing::Point(16, 338);
			this->label34->Name = L"label34";
			this->label34->Size = System::Drawing::Size(14, 13);
			this->label34->TabIndex = 66;
			this->label34->Text = L"X";
			// 
			// label35
			// 
			this->label35->AutoSize = true;
			this->label35->Location = System::Drawing::Point(54, 309);
			this->label35->Name = L"label35";
			this->label35->Size = System::Drawing::Size(88, 13);
			this->label35->TabIndex = 65;
			this->label35->Text = L"Данные ракеты";
			// 
			// RadarForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(439, 464);
			this->Controls->Add(this->label22);
			this->Controls->Add(this->TBMissileVH);
			this->Controls->Add(this->TBMissileVY);
			this->Controls->Add(this->TBMissileVX);
			this->Controls->Add(this->label28);
			this->Controls->Add(this->label29);
			this->Controls->Add(this->label30);
			this->Controls->Add(this->TBMissileV);
			this->Controls->Add(this->TBMissileH);
			this->Controls->Add(this->TBMissileY);
			this->Controls->Add(this->TBMissileX);
			this->Controls->Add(this->label31);
			this->Controls->Add(this->label32);
			this->Controls->Add(this->label33);
			this->Controls->Add(this->label34);
			this->Controls->Add(this->label35);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->TBPlaneVH);
			this->Controls->Add(this->TBPlaneVY);
			this->Controls->Add(this->TBPlaneVX);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->label15);
			this->Controls->Add(this->label16);
			this->Controls->Add(this->TBPlaneV);
			this->Controls->Add(this->TBPlaneH);
			this->Controls->Add(this->TBPlaneY);
			this->Controls->Add(this->TBPlaneX);
			this->Controls->Add(this->label17);
			this->Controls->Add(this->label18);
			this->Controls->Add(this->label19);
			this->Controls->Add(this->label20);
			this->Controls->Add(this->label21);
			this->Controls->Add(this->label27);
			this->Controls->Add(this->label26);
			this->Controls->Add(this->TBShipVH);
			this->Controls->Add(this->TBShipVY);
			this->Controls->Add(this->TBShipVX);
			this->Controls->Add(this->label23);
			this->Controls->Add(this->label24);
			this->Controls->Add(this->label25);
			this->Controls->Add(this->TBShipV);
			this->Controls->Add(this->TBShipH);
			this->Controls->Add(this->TBShipY);
			this->Controls->Add(this->TBShipX);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->numericUpDown1);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->TBRadarV);
			this->Controls->Add(this->TBRadarH);
			this->Controls->Add(this->TBRadarY);
			this->Controls->Add(this->TBRadarX);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Name = L"RadarForm";
			this->Text = L"RadarForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	public:
		void DisableRadarKoordEdit() {button1->Enabled = false;}
		void UpdateRadarData(int x, int y, int h, int v)
		{
			TBRadarX->Text = x.ToString();
			TBRadarY->Text = y.ToString();
			TBRadarH->Text = h.ToString();
			TBRadarV->Text = v.ToString();
		}
		void UpdateShipData(int x, int y, int h, int v, int vx, int vy, int vh)
		{
			TBShipX->Text = x.ToString();
			TBShipY->Text = y.ToString();
			TBShipH->Text = h.ToString();
			TBShipV->Text = v.ToString();
			TBShipVX->Text = vx.ToString();
			TBShipVY->Text = vy.ToString();
			TBShipVH->Text = vh.ToString();
		}
		void UpdatePlaneData(int x, int y, int h, int v, int vx, int vy, int vh)
		{
			TBPlaneX->Text = x.ToString();
			TBPlaneY->Text = y.ToString();
			TBPlaneH->Text = h.ToString();
			TBPlaneV->Text = v.ToString();
			TBPlaneVX->Text = vx.ToString();
			TBPlaneVY->Text = vy.ToString();
			TBPlaneVH->Text = vh.ToString();
		}
		void UpdateMissileData(int x, int y, int h, int v, int vx, int vy, int vh)
		{
			TBMissileX->Text = x.ToString();
			TBMissileY->Text = y.ToString();
			TBMissileH->Text = h.ToString();
			TBMissileV->Text = v.ToString();
			TBMissileVX->Text = vx.ToString();
			TBMissileVY->Text = vy.ToString();
			TBMissileVH->Text = vh.ToString();
		}

	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
				 //Обновление данных
				 if(bWork ==false)
			 {
				 bWork = true;
				 nTimeOut = System::Decimal::ToInt32(numericUpDown1->Value);
				 newThread = gcnew Thread(gcnew ParameterizedThreadStart(&DelegateThreadTask));
				 newThread->Start(this);
				 button2->Text="Стоп";
			 }
			 else
			 {
				 bWork = false;
				 button2->Text="Старт";
			 }
			 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 //Изменение расположения радара
		 }
};
}
