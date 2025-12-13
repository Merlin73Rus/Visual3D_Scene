#pragma once
//#include "stdafx.h"
#include <string>

namespace Interface {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;


	/// <summary>
	/// Summary for ShipParam
	/// </summary>
	public ref class ShipParam : public System::Windows::Forms::Form
	{
	public: delegate void ShipData(int nType);
	public: ShipData^ mData;
			long nTimeOut;
	private: System::Windows::Forms::Label^  label13;
	public: 
		bool bWork;
	private: System::Windows::Forms::DataGridView^  dataGridView2;
	public: 

	private: Thread^ newThread;
	public: static void DelegateThreadTask(System::Object ^obj)
			{
				ShipParam^ ob = (ShipParam^) obj;
				do
				{
					if (ob->mData) 	 { ob->mData(ob->nObjectType);}
					Thread::Sleep(ob->nTimeOut);
				} while (ob->bWork==true);
			}

	public: delegate void StartShipMoving(int nType);
	public: StartShipMoving^ mMove;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
	public: 
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column4;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column5;
			 int nObjectType;

	public:
		
		ShipParam(int nType)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			nObjectType = nType;
			bWork = false;
			nTimeOut = 100;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ShipParam()
		{
			if(bWork == true) {bWork = false;}
			//if(newThread->IsAlive){ newThread->Abort();}
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::DataGridView^  dataGridView1;
			 Form^ parent;
	protected: 




	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::TextBox^  textBoxX;
	private: System::Windows::Forms::TextBox^  textBoxY;


	private: System::Windows::Forms::Button^  button1;

	private: System::Windows::Forms::NumericUpDown^  numericUpDown2;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Button^  button2;






	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::TextBox^  textBoxZ;
	private: System::Windows::Forms::Label^  label12;

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
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column4 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column5 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->textBoxX = (gcnew System::Windows::Forms::TextBox());
			this->textBoxY = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->textBoxZ = (gcnew System::Windows::Forms::TextBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->dataGridView2 = (gcnew System::Windows::Forms::DataGridView());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView2))->BeginInit();
			this->SuspendLayout();
			// 
			// dataGridView1
			// 
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(5) {this->Column1, 
				this->Column4, this->Column2, this->Column3, this->Column5});
			this->dataGridView1->Location = System::Drawing::Point(519, 214);
			this->dataGridView1->MultiSelect = false;
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->Size = System::Drawing::Size(350, 256);
			this->dataGridView1->TabIndex = 1;
			// 
			// Column1
			// 
			this->Column1->HeaderText = L"№";
			this->Column1->Name = L"Column1";
			this->Column1->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column1->Width = 35;
			// 
			// Column4
			// 
			this->Column4->HeaderText = L"V м/с";
			this->Column4->Name = L"Column4";
			this->Column4->Width = 65;
			// 
			// Column2
			// 
			this->Column2->HeaderText = L"X";
			this->Column2->Name = L"Column2";
			this->Column2->Width = 60;
			// 
			// Column3
			// 
			this->Column3->HeaderText = L"Y";
			this->Column3->Name = L"Column3";
			this->Column3->Width = 60;
			// 
			// Column5
			// 
			this->Column5->HeaderText = L"Z";
			this->Column5->Name = L"Column5";
			this->Column5->Width = 60;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 15);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(172, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Текущее направление (№ точки)";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(278, 15);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(130, 13);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Текущая скорость (м/c)";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(12, 48);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(116, 13);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Текущие координаты";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(134, 48);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(14, 13);
			this->label4->TabIndex = 5;
			this->label4->Text = L"X";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(242, 48);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(14, 13);
			this->label5->TabIndex = 6;
			this->label5->Text = L"Y";
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(190, 11);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(60, 21);
			this->comboBox1->TabIndex = 7;
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(433, 11);
			this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(69, 20);
			this->numericUpDown1->TabIndex = 8;
			// 
			// textBoxX
			// 
			this->textBoxX->Location = System::Drawing::Point(155, 44);
			this->textBoxX->Name = L"textBoxX";
			this->textBoxX->ReadOnly = true;
			this->textBoxX->Size = System::Drawing::Size(64, 20);
			this->textBoxX->TabIndex = 9;
			this->textBoxX->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBoxY
			// 
			this->textBoxY->Location = System::Drawing::Point(266, 44);
			this->textBoxY->Name = L"textBoxY";
			this->textBoxY->ReadOnly = true;
			this->textBoxY->Size = System::Drawing::Size(64, 20);
			this->textBoxY->TabIndex = 10;
			this->textBoxY->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(468, 43);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(133, 23);
			this->button1->TabIndex = 11;
			this->button1->Text = L"Получить координаты";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &ShipParam::button1_Click);
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->Location = System::Drawing::Point(186, 77);
			this->numericUpDown2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(70, 20);
			this->numericUpDown2->TabIndex = 13;
			this->numericUpDown2->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(263, 81);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(74, 13);
			this->label6->TabIndex = 14;
			this->label6->Text = L"миллисекунд";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(343, 76);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 15;
			this->button2->Text = L"Старт";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &ShipParam::button2_Click);
			// 
			// button3
			// 
			this->button3->Enabled = false;
			this->button3->Location = System::Drawing::Point(736, 135);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(133, 23);
			this->button3->TabIndex = 22;
			this->button3->Text = L"Камера на объект";
			this->button3->UseVisualStyleBackColor = true;
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(735, 15);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(134, 23);
			this->button4->TabIndex = 23;
			this->button4->Text = L"Начать Движение";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &ShipParam::button4_Click);
			// 
			// button5
			// 
			this->button5->Enabled = false;
			this->button5->Location = System::Drawing::Point(526, 10);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(75, 23);
			this->button5->TabIndex = 24;
			this->button5->Text = L"Обновить";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &ShipParam::button5_Click);
			// 
			// textBoxZ
			// 
			this->textBoxZ->Location = System::Drawing::Point(375, 44);
			this->textBoxZ->Name = L"textBoxZ";
			this->textBoxZ->ReadOnly = true;
			this->textBoxZ->Size = System::Drawing::Size(64, 20);
			this->textBoxZ->TabIndex = 26;
			this->textBoxZ->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(351, 48);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(14, 13);
			this->label12->TabIndex = 25;
			this->label12->Text = L"Z";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(15, 81);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(161, 13);
			this->label13->TabIndex = 27;
			this->label13->Text = L"Получать координаты каждые";
			// 
			// dataGridView2
			// 
			this->dataGridView2->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView2->Location = System::Drawing::Point(13, 214);
			this->dataGridView2->Name = L"dataGridView2";
			this->dataGridView2->Size = System::Drawing::Size(379, 256);
			this->dataGridView2->TabIndex = 28;
			// 
			// ShipParam
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(881, 482);
			this->Controls->Add(this->dataGridView2);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->textBoxZ);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->numericUpDown2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->textBoxY);
			this->Controls->Add(this->textBoxX);
			this->Controls->Add(this->numericUpDown1);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->dataGridView1);
			this->Name = L"ShipParam";
			this->Text = L"Настройки";
			this->Load += gcnew System::EventHandler(this, &ShipParam::ShipParam_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: void InitShipParam()
			 {
				 this->Text = "Настройки корабля";
				 dataGridView1->Rows->Add(0, 16, -30000, -130000, 1005);
				 dataGridView1->Rows->Add(1, 12, 130000, -130000, 1005);
				 dataGridView1->Rows->Add(2, 15, 130000, -60000,  1005);
				 dataGridView1->Rows->Add(3, 10,  0,      0,      1005);
			 }
	private: void InitPlaneParam()
			 {
				 this->Text = "Настройки самолета";
				 dataGridView1->Rows->Add(0, 160, -30000, -130000, 5000);
				 dataGridView1->Rows->Add(1, 120, 130000, -130000, 5000);
				 dataGridView1->Rows->Add(2, 150, 130000, -60000,  5000);
				 dataGridView1->Rows->Add(3, 100,  0,      0,      5000);
			 }
	private: void InitMissileParam()
			 {
				 this->Text = "Настройки ракеты";
				 dataGridView1->Rows->Add(0, 160, -30000, -130000, 5000);
				 dataGridView1->Rows->Add(1, 120, 130000, -130000, 5000);
				 dataGridView1->Rows->Add(2, 150, 130000, -60000,  5000);
				 dataGridView1->Rows->Add(3, 100,  0,      0,      5000);
			 }
	private: System::Void ShipParam_Load(System::Object^  sender, System::EventArgs^  e)
			 {
				 switch(nObjectType)
				 {
				 case 0 :
					 break;
				 case 1 :		 //корабль
					 InitShipParam();					 
					 break;
				 case 2: //самолет
					 InitPlaneParam();
					 break;
				 case 3: //ракета
					 InitMissileParam();
					 break;
				 default :
					 break;
				 }				 

				 for (int i=0; i!=dataGridView1->Rows->Count-1; i++)
				 {
					 comboBox1->Items->Add(dataGridView1->Rows[i]->Cells[0]->Value);
				 }

			 }

//Кнопка запроса координат
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if (mData) 	 { mData(nObjectType);	}
		 }
public: void SetData(int X, int Y, int H, int S)
		{
			 textBoxX->Text = X.ToString();
			 textBoxY->Text = Y.ToString();
			 textBoxZ->Text = H.ToString();
			 numericUpDown1->Value = S;
		}
// Кнопка начать движение
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			  if (mMove) 	 { mMove(nObjectType);	}
		 }

public: void GetDestination(int &X, int &Y, int &H, int &S)
 {
		S = Int32::Parse(dataGridView1->Rows[comboBox1->SelectedIndex]->Cells[1]->Value->ToString());
		X = Int32::Parse(dataGridView1->Rows[comboBox1->SelectedIndex]->Cells[2]->Value->ToString());
		Y = Int32::Parse(dataGridView1->Rows[comboBox1->SelectedIndex]->Cells[3]->Value->ToString());
		H = Int32::Parse(dataGridView1->Rows[comboBox1->SelectedIndex]->Cells[4]->Value->ToString());
	}
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {

			//DelegateThreadTask;
			 if(bWork ==false)
			 {
				 bWork = true;
				 nTimeOut = System::Decimal::ToInt32(numericUpDown2->Value);
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
private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {

		 }
};
}
