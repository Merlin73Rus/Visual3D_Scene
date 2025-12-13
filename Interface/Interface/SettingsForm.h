#pragma once

namespace Interface {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for SettingsForm
	/// </summary>

	public ref class SettingsForm : public System::Windows::Forms::Form
	{
	public:
		bool bRadar1; bool bRadar2; bool bRadar3;
		int nCount;
		int nMaxObjectsNumber;
		int nCurrentObject;
		bool bClose;
		bool bCreated;
		bool bDefault;
		bool bLoad;
		String^ FileName;

	public: delegate void RadarData();
	public: RadarData^ mData;

	public:	array <System::Windows::Forms::DataGridView^>^  Routes;

	public: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::DataGridView^  dataGridView2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column8;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column9;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column10;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column11;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	public: System::Windows::Forms::DataGridView^  dataGridView3;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
	private: System::Windows::Forms::DataGridViewComboBoxColumn^  Column2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;
	private: System::Windows::Forms::DataGridViewCheckBoxColumn^  Column4;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column5;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column6;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column7;
	private: System::Windows::Forms::DataGridViewButtonColumn^  Color;
	private: System::Windows::Forms::DataGridViewCheckBoxColumn^  Column13;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column14;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column17;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column18;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column19;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column15;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column16;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column20;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column12;	
			 
	public:	SettingsForm()
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			nCurrentObject = -1;
			nMaxObjectsNumber = 50;
			Routes = gcnew array <System::Windows::Forms::DataGridView^> (nMaxObjectsNumber);
			for(int i = 0; i!=50; i++)
			{
				Routes[i] = gcnew System::Windows::Forms::DataGridView();
				Routes[i]->ColumnCount = 6;
			}
		
			nCount = 1;	
			bClose = false;
			bCreated = false;
			bDefault = false;
			bLoad = false;
		}

	public: int GetNextID()
		{
			int nNextID = 0;
			try{
			for each (System::Windows::Forms::DataGridViewRow^ row in dataGridView1->Rows)
			{
				if(nNextID < Int32::Parse(row->Cells[0]->Value->ToString()))
				{
					nNextID = Int32::Parse(row->Cells[0]->Value->ToString());
				}
			}
			}
			catch(...) { ;}
			nNextID++;
			return nNextID;
		}

	public:	System::Void AddObject(int nType, String^ sName, bool bRadar, int nX0, int nY0, int nH0)
		{
			dataGridView1->Rows->Add(GetNextID(), nCount, GetStringType(nType), sName, bRadar, nX0, nY0, nH0);
		}

public:	System::Void AddRadar(int nNumber, String^ sName, int nX0, int nY0, int nH0)
		{
			dataGridView3->Rows[nNumber]->Cells[0]->Value = true;
			EnabledRow(nNumber);
			dataGridView3->Rows[nNumber]->Cells[1]->Value = sName;
			dataGridView3->Rows[nNumber]->Cells[2]->Value = nX0;
			dataGridView3->Rows[nNumber]->Cells[3]->Value = nY0;
			dataGridView3->Rows[nNumber]->Cells[4]->Value = nH0;
		}

	public:	System::Void AddRoute(int nObject, int nSpeed, int nX, int nY, int nH)
		{	
			int n = 0;
			n = Routes[nObject]->Rows->Count;
			Routes[nObject]->Rows->Add(nObject,n, nSpeed, nX, nY, nH, "");
		}

	public: String^ GetStringType(int nType)
		{
			String^ sType;
			switch(nType)
				 {
				 case 0 :     //Радар
					 sType = "Радар";
					 break;
				 case 1 :		 //корабль
					 sType = "Корабль";
					 break;
				 case 2: //самолет
					  sType = "Самолет";
					 break;
				 case 3: //ракета
					  sType = "Ракета";
					 break;
				 default :
					 sType = "Радар";
					 break;
				 }
			return sType;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SettingsForm()
		{
			if (components)
			{
				delete components;
			}
		}

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
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle3 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle4 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle5 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle6 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle7 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle8 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle9 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle10 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle11 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle12 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle13 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle14 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle15 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle16 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle17 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->ID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewComboBoxColumn());
			this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column4 = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
			this->Column5 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column6 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column7 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Color = (gcnew System::Windows::Forms::DataGridViewButtonColumn());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->dataGridView2 = (gcnew System::Windows::Forms::DataGridView());
			this->Column8 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column9 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column10 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column11 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column12 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->dataGridView3 = (gcnew System::Windows::Forms::DataGridView());
			this->Column13 = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
			this->Column14 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column17 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column18 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column19 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column15 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column16 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column20 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView2))->BeginInit();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView3))->BeginInit();
			this->SuspendLayout();
			// 
			// dataGridView1
			// 
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(9) {this->ID, this->Column1, 
				this->Column2, this->Column3, this->Column4, this->Column5, this->Column6, this->Column7, this->Color});
			this->dataGridView1->Location = System::Drawing::Point(8, 20);
			this->dataGridView1->MultiSelect = false;
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::CellSelect;
			this->dataGridView1->Size = System::Drawing::Size(499, 200);
			this->dataGridView1->TabIndex = 0;
			this->dataGridView1->CellClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &SettingsForm::dataGridView1_CellClick);
			this->dataGridView1->RowEnter += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &SettingsForm::dataGridView1_RowEnter);
			this->dataGridView1->RowHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &SettingsForm::dataGridView1_RowHeaderMouseClick);
			this->dataGridView1->RowsAdded += gcnew System::Windows::Forms::DataGridViewRowsAddedEventHandler(this, &SettingsForm::dataGridView1_RowsAdded);
			this->dataGridView1->RowsRemoved += gcnew System::Windows::Forms::DataGridViewRowsRemovedEventHandler(this, &SettingsForm::dataGridView1_RowsRemoved);
			// 
			// ID
			// 
			dataGridViewCellStyle1->Format = L"N0";
			dataGridViewCellStyle1->NullValue = L"0";
			this->ID->DefaultCellStyle = dataGridViewCellStyle1;
			this->ID->HeaderText = L"ID";
			this->ID->Name = L"ID";
			this->ID->Resizable = System::Windows::Forms::DataGridViewTriState::True;
			this->ID->Visible = false;
			this->ID->Width = 43;
			// 
			// Column1
			// 
			dataGridViewCellStyle2->Format = L"N0";
			dataGridViewCellStyle2->NullValue = L"1";
			this->Column1->DefaultCellStyle = dataGridViewCellStyle2;
			this->Column1->HeaderText = L"Номер";
			this->Column1->Name = L"Column1";
			this->Column1->ReadOnly = true;
			this->Column1->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column1->ToolTipText = L"Номер объекта";
			this->Column1->Width = 47;
			// 
			// Column2
			// 
			dataGridViewCellStyle3->NullValue = L"Корабль";
			this->Column2->DefaultCellStyle = dataGridViewCellStyle3;
			this->Column2->DisplayStyle = System::Windows::Forms::DataGridViewComboBoxDisplayStyle::ComboBox;
			this->Column2->HeaderText = L"Тип";
			this->Column2->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Корабль", L"Самолет", L"Ракета"});
			this->Column2->Name = L"Column2";
			this->Column2->ToolTipText = L"Тип объекта";
			this->Column2->Width = 75;
			// 
			// Column3
			// 
			this->Column3->HeaderText = L"Имя";
			this->Column3->Name = L"Column3";
			this->Column3->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column3->ToolTipText = L"Имя объекта";
			this->Column3->Width = 35;
			// 
			// Column4
			// 
			this->Column4->HeaderText = L"С Радаром";
			this->Column4->Name = L"Column4";
			this->Column4->ReadOnly = true;
			this->Column4->ToolTipText = L"На объекта установлен радар";
			this->Column4->Width = 68;
			// 
			// Column5
			// 
			dataGridViewCellStyle4->Format = L"N0";
			dataGridViewCellStyle4->NullValue = L"0";
			this->Column5->DefaultCellStyle = dataGridViewCellStyle4;
			this->Column5->HeaderText = L"X0";
			this->Column5->Name = L"Column5";
			this->Column5->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column5->ToolTipText = L"Начальная координата X";
			this->Column5->Width = 26;
			// 
			// Column6
			// 
			dataGridViewCellStyle5->Format = L"N0";
			dataGridViewCellStyle5->NullValue = L"0";
			this->Column6->DefaultCellStyle = dataGridViewCellStyle5;
			this->Column6->HeaderText = L"Y0";
			this->Column6->Name = L"Column6";
			this->Column6->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column6->ToolTipText = L"Начальная координата Y";
			this->Column6->Width = 26;
			// 
			// Column7
			// 
			dataGridViewCellStyle6->Format = L"N0";
			dataGridViewCellStyle6->NullValue = L"0";
			this->Column7->DefaultCellStyle = dataGridViewCellStyle6;
			this->Column7->HeaderText = L"H0";
			this->Column7->Name = L"Column7";
			this->Column7->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column7->ToolTipText = L"Начальная высота";
			this->Column7->Width = 27;
			// 
			// Color
			// 
			dataGridViewCellStyle7->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
			dataGridViewCellStyle7->BackColor = System::Drawing::Color::White;
			dataGridViewCellStyle7->ForeColor = System::Drawing::Color::White;
			this->Color->DefaultCellStyle = dataGridViewCellStyle7;
			this->Color->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->Color->HeaderText = L"Цвет";
			this->Color->Name = L"Color";
			this->Color->Resizable = System::Windows::Forms::DataGridViewTriState::True;
			this->Color->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Automatic;
			this->Color->Width = 57;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(182, 4);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(95, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Список объектов";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(182, 244);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(105, 13);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Маршрут движения";
			// 
			// dataGridView2
			// 
			this->dataGridView2->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView2->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView2->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(5) {this->Column8, 
				this->Column9, this->Column10, this->Column11, this->Column12});
			this->dataGridView2->Location = System::Drawing::Point(11, 260);
			this->dataGridView2->MultiSelect = false;
			this->dataGridView2->Name = L"dataGridView2";
			this->dataGridView2->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::CellSelect;
			this->dataGridView2->Size = System::Drawing::Size(496, 215);
			this->dataGridView2->TabIndex = 2;
			this->dataGridView2->CellValueChanged += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &SettingsForm::dataGridView2_CellValueChanged);
			this->dataGridView2->RowHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &SettingsForm::dataGridView2_RowHeaderMouseClick);
			// 
			// Column8
			// 
			dataGridViewCellStyle8->Format = L"N0";
			dataGridViewCellStyle8->NullValue = L"0";
			this->Column8->DefaultCellStyle = dataGridViewCellStyle8;
			this->Column8->HeaderText = L"№ Точки";
			this->Column8->Name = L"Column8";
			this->Column8->Width = 76;
			// 
			// Column9
			// 
			dataGridViewCellStyle9->Format = L"N0";
			dataGridViewCellStyle9->NullValue = L"0";
			this->Column9->DefaultCellStyle = dataGridViewCellStyle9;
			this->Column9->HeaderText = L"Скорость";
			this->Column9->Name = L"Column9";
			this->Column9->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column9->Width = 61;
			// 
			// Column10
			// 
			dataGridViewCellStyle10->Format = L"N0";
			dataGridViewCellStyle10->NullValue = L"0";
			this->Column10->DefaultCellStyle = dataGridViewCellStyle10;
			this->Column10->HeaderText = L"X";
			this->Column10->Name = L"Column10";
			this->Column10->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column10->Width = 21;
			// 
			// Column11
			// 
			dataGridViewCellStyle11->Format = L"N0";
			dataGridViewCellStyle11->NullValue = L"0";
			this->Column11->DefaultCellStyle = dataGridViewCellStyle11;
			this->Column11->HeaderText = L"Y";
			this->Column11->Name = L"Column11";
			this->Column11->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column11->Width = 21;
			// 
			// Column12
			// 
			dataGridViewCellStyle12->Format = L"N0";
			dataGridViewCellStyle12->NullValue = L"0";
			this->Column12->DefaultCellStyle = dataGridViewCellStyle12;
			this->Column12->HeaderText = L"H";
			this->Column12->Name = L"Column12";
			this->Column12->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column12->Width = 21;
			// 
			// tabControl1
			// 
			this->tabControl1->Alignment = System::Windows::Forms::TabAlignment::Bottom;
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Location = System::Drawing::Point(2, 2);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(525, 523);
			this->tabControl1->TabIndex = 4;
			// 
			// tabPage1
			// 
			this->tabPage1->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->tabPage1->Controls->Add(this->label1);
			this->tabPage1->Controls->Add(this->label2);
			this->tabPage1->Controls->Add(this->dataGridView1);
			this->tabPage1->Controls->Add(this->dataGridView2);
			this->tabPage1->Location = System::Drawing::Point(4, 4);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(517, 497);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Объекты";
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->dataGridView3);
			this->tabPage2->Location = System::Drawing::Point(4, 4);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(517, 497);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Радары";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// dataGridView3
			// 
			this->dataGridView3->AllowUserToAddRows = false;
			this->dataGridView3->AllowUserToDeleteRows = false;
			this->dataGridView3->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView3->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView3->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(8) {this->Column13, 
				this->Column14, this->Column17, this->Column18, this->Column19, this->Column15, this->Column16, this->Column20});
			this->dataGridView3->Location = System::Drawing::Point(31, 17);
			this->dataGridView3->MultiSelect = false;
			this->dataGridView3->Name = L"dataGridView3";
			this->dataGridView3->RowHeadersVisible = false;
			this->dataGridView3->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::CellSelect;
			this->dataGridView3->Size = System::Drawing::Size(455, 464);
			this->dataGridView3->TabIndex = 0;
			this->dataGridView3->CellValueChanged += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &SettingsForm::dataGridView3_CellValueChanged);
			this->dataGridView3->CurrentCellDirtyStateChanged += gcnew System::EventHandler(this, &SettingsForm::dataGridView3_CurrentCellDirtyStateChanged);
			// 
			// Column13
			// 
			this->Column13->HeaderText = L"";
			this->Column13->Name = L"Column13";
			this->Column13->Width = 5;
			// 
			// Column14
			// 
			this->Column14->HeaderText = L"Имя";
			this->Column14->Name = L"Column14";
			this->Column14->Resizable = System::Windows::Forms::DataGridViewTriState::True;
			this->Column14->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column14->Width = 35;
			// 
			// Column17
			// 
			dataGridViewCellStyle13->Format = L"N0";
			dataGridViewCellStyle13->NullValue = L"0";
			this->Column17->DefaultCellStyle = dataGridViewCellStyle13;
			this->Column17->HeaderText = L"X";
			this->Column17->Name = L"Column17";
			this->Column17->Width = 39;
			// 
			// Column18
			// 
			dataGridViewCellStyle14->Format = L"N0";
			dataGridViewCellStyle14->NullValue = L"0";
			this->Column18->DefaultCellStyle = dataGridViewCellStyle14;
			this->Column18->HeaderText = L"Y";
			this->Column18->Name = L"Column18";
			this->Column18->Width = 39;
			// 
			// Column19
			// 
			dataGridViewCellStyle15->Format = L"N0";
			dataGridViewCellStyle15->NullValue = L"0";
			this->Column19->DefaultCellStyle = dataGridViewCellStyle15;
			this->Column19->HeaderText = L"H";
			this->Column19->Name = L"Column19";
			this->Column19->Width = 40;
			// 
			// Column15
			// 
			dataGridViewCellStyle16->Format = L"###.###.###.###";
			dataGridViewCellStyle16->NullValue = L"127.0.0.1";
			this->Column15->DefaultCellStyle = dataGridViewCellStyle16;
			this->Column15->HeaderText = L"Адрес";
			this->Column15->Name = L"Column15";
			this->Column15->Width = 63;
			// 
			// Column16
			// 
			this->Column16->HeaderText = L"Порт";
			this->Column16->Name = L"Column16";
			this->Column16->ReadOnly = true;
			this->Column16->Resizable = System::Windows::Forms::DataGridViewTriState::True;
			this->Column16->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column16->Width = 38;
			// 
			// Column20
			// 
			dataGridViewCellStyle17->Format = L"N0";
			dataGridViewCellStyle17->NullValue = L"20";
			this->Column20->DefaultCellStyle = dataGridViewCellStyle17;
			this->Column20->HeaderText = L"Интервал";
			this->Column20->Name = L"Column20";
			this->Column20->Width = 81;
			// 
			// SettingsForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(526, 531);
			this->Controls->Add(this->tabControl1);
			this->Name = L"SettingsForm";
			this->Text = L"SettingsForm";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &SettingsForm::SettingsForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &SettingsForm::SettingsForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView2))->EndInit();
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView3))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

private: System::Void buttonClick(System::Windows::Forms::Button^ button, int nRadarNumber, bool bRadar)
			 {
				 if(bRadar==false)//Начинаем передачу данных
				 {
					 button->Text = L"Остановить передачу данных";					 
				 }
				 else //Останавливаем передачу данных
				 {
					 button->Text = L"Начать передачу данных";					 
				 }
			 }

private: System::Void dataGridView1_RowHeaderMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e)
		 {
			 dataGridView1->SelectionMode =  System::Windows::Forms::DataGridViewSelectionMode::RowHeaderSelect;
			 dataGridView1->Rows[e->RowIndex]->Selected = true;
		 }

private: System::Void dataGridView1_RowsAdded(System::Object^  sender, System::Windows::Forms::DataGridViewRowsAddedEventArgs^  e)
		 {
			 
			 dataGridView1->Rows[e->RowIndex]->Cells[1]->Value = e->RowIndex + 1;
			 dataGridView1->Rows[e->RowIndex]->Cells["ID"]->Value = GetNextID();
			 dataGridView1->Rows[e->RowIndex]->Cells["Color"]->Style->BackColor=System::Drawing::Color::White; //FromArgb(0xFFFFFF00);
			 nCount++;
		 }

private: System::Void dataGridView1_RowsRemoved(System::Object^  sender, System::Windows::Forms::DataGridViewRowsRemovedEventArgs^  e) 
		 {
			 for (int i=0; i!=dataGridView1->Rows->Count; i++)
			 {
					dataGridView1->Rows[i]->Cells[1]->Value = i + 1;
			 }
			 nCount --;
		 }

private: System::Void dataGridView1_RowEnter(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e)
		 {
			 SaveNChanges();
			 nCurrentObject = e->RowIndex;
			 //MessageBox::Show(e->RowIndex.ToString());
			 ShowRoutesByN(e->RowIndex);
		 }

private: System::Void ShowRoutesByN(int nObject)
		 {
			 dataGridView2->Rows->Clear();
			 for(int i = 0; i!=Routes[nObject]->Rows->Count-1; i++)
			 {
				 dataGridView2->Rows->Add(Routes[nObject]->Rows[i]->Cells[1]->Value,
				                          Routes[nObject]->Rows[i]->Cells[2]->Value,
									      Routes[nObject]->Rows[i]->Cells[3]->Value,
									      Routes[nObject]->Rows[i]->Cells[4]->Value,
									      Routes[nObject]->Rows[i]->Cells[5]->Value);
			 }
		 }

private: System::Void SaveNChanges()
		 {
			if (nCurrentObject!=-1)
			{
				Routes[nCurrentObject]->Rows->Clear();
				dataGridView2->Sort(dataGridView2->Columns[0],ListSortDirection::Ascending);
				
				int nC = ((dataGridView2->Rows->Count)-1);

				for(int i = 0; i!=nC; i++)
				{

					Routes[nCurrentObject]->Rows->Add(nCurrentObject,
     						                          dataGridView2->Rows[i]->Cells[0]->Value,
				                                      dataGridView2->Rows[i]->Cells[1]->Value,
									                  dataGridView2->Rows[i]->Cells[2]->Value,
									                  dataGridView2->Rows[i]->Cells[3]->Value,
									                  dataGridView2->Rows[i]->Cells[4]->Value);
				}
			}
		 }

private: System::Void dataGridView2_CellValueChanged(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e)
		 {
			 try{
			 dataGridView2->Rows[e->RowIndex]->Cells[e->ColumnIndex]->Value = 
				 Int32::Parse(dataGridView2->Rows[e->RowIndex]->Cells[e->ColumnIndex]->Value->ToString());
			 }
			 catch(...) { ;}
		 }
private: System::Void dataGridView2_RowHeaderMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e)
		 {
			 dataGridView2->SelectionMode =  System::Windows::Forms::DataGridViewSelectionMode::RowHeaderSelect;
			 dataGridView2->Rows[e->RowIndex]->Selected = true;
		 }

private: System::Void SettingsForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
		 {
			 
			 SaveNChanges();
			 if(!bClose)
			 {
				 e->Cancel = true;
				 this->Hide();
			 }
		 }
private: System::Void dataGridView1_CellClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e)
		 {
			  //null checking for  the column
			 
			 if(e->ColumnIndex==8)
			 {
            if (dataGridView1->CurrentRow->Cells["Color"] != nullptr)

            {

                ColorDialog^ colorDlg = gcnew ColorDialog(); //create colordialog instance

                colorDlg->AllowFullOpen = true;

                colorDlg->AnyColor = true;

                colorDlg->ShowDialog(); //display dialog

				dataGridView1->CurrentRow->Cells["Color"]->Style->BackColor = colorDlg->Color;
				//MessageBox::Show(colorDlg->Color.ToArgb().ToString());
				dataGridView1->CurrentRow->Cells["Color"]->Selected = false;

                //dataGridView1->CurrentRow->Cells["Color"]->Value = colorDlg->Color.Name; //keep the selected value
			}
			 }
		 }
private: System::Void SettingsForm_Load(System::Object^  sender, System::EventArgs^  e)
		 {
			 for(int i = 0; i!=10; i++)
			 {
				 dataGridView3->Rows->Add(false, "Радар", 0, 0, 0, "127.0.0.1", (11001+i).ToString(), 20);
				 DisabledRow(i);				 
			 }

			 for(int i = 0; i!=10; i++)
			 {
				 dataGridView3->Rows->Add(false, "Радар", 0, 0, 0, "127.0.0.1", (12001+i).ToString(), 20);
				 DisabledRow(10+i);
			 }

			 bCreated = true;
			 if(bDefault) { mData();}
			 if(bLoad) { LoadProject(FileName); ShowRoutesByN(0); }
		 }
public:	System::Void AddObject(int nObjectNumber, int nType, String^ sName, bool bRadar, int nX0, int nY0, int nH0, int nColor)
		{
			AddObject(nType, sName, bRadar, nX0, nY0, nH0);
			dataGridView1->Rows[nObjectNumber]->Cells[8]->Style->BackColor = System::Drawing::Color::FromArgb(nColor);
		}

private: System::Void LoadProject(String^ FileName)
		 {
			 System::Xml::XmlDocument^ Xml = gcnew  System::Xml::XmlDocument();
			 System::Xml::XmlNode^ xNode; 

			 Xml->Load(FileName);
			 //xNode = Xml->DocumentElement();

			 xNode = Xml->DocumentElement->SelectSingleNode("ObjectData");
			 
			 int nObjectNumber = Convert::ToInt32(xNode->Attributes->GetNamedItem("ObjectNumber")->Value);

			 for (int i = 0; i!=nObjectNumber; i++)
			 {
				 xNode = Xml->DocumentElement->SelectSingleNode("ObjectData/ObjectData"+i.ToString());
				 AddObject(i,
					       Convert::ToInt32(xNode->Attributes->GetNamedItem("ObjectType")->Value),
					       xNode->Attributes->GetNamedItem("ObjectName")->Value, 
						   Convert::ToBoolean(xNode->Attributes->GetNamedItem("Radar")->Value),
						   Convert::ToInt32(xNode->Attributes->GetNamedItem("X0")->Value),
						   Convert::ToInt32(xNode->Attributes->GetNamedItem("Y0")->Value),
						   Convert::ToInt32(xNode->Attributes->GetNamedItem("H0")->Value),
						   Convert::ToInt32(xNode->Attributes->GetNamedItem("Color")->Value));

				 int nRoutes = Convert::ToInt32(xNode->Attributes->GetNamedItem("RoutesNumber")->Value);

				 for(int n = 0; n!=nRoutes; n++)
				 {
					 xNode = Xml->DocumentElement->SelectSingleNode("ObjectData/ObjectData"+i.ToString()+"/Route"+n.ToString());
					 AddRoute(i,
						      Convert::ToInt32(xNode->Attributes->GetNamedItem("Speed")->Value),
							  Convert::ToInt32(xNode->Attributes->GetNamedItem("X")->Value),
							  Convert::ToInt32(xNode->Attributes->GetNamedItem("Y")->Value),
							  Convert::ToInt32(xNode->Attributes->GetNamedItem("H")->Value));
				 }

			 }

			 dataGridView1->Rows[dataGridView1->Rows->Count-1]->Cells[1]->Value = dataGridView1->Rows->Count;
			 dataGridView1->Rows[dataGridView1->Rows->Count-1]->Cells[0]->Value = 0;

			 for(int i = 0; i!=20; i++)
			 {
				 xNode = Xml->DocumentElement->SelectSingleNode("RadarData/Radar"+i.ToString());
				 SetRadar(i,
					      Convert::ToBoolean(xNode->Attributes->GetNamedItem("Active")->Value),
						  xNode->Attributes->GetNamedItem("RadarName")->Value,
						  Convert::ToInt32(xNode->Attributes->GetNamedItem("X")->Value),
						  Convert::ToInt32(xNode->Attributes->GetNamedItem("Y")->Value),
						  Convert::ToInt32(xNode->Attributes->GetNamedItem("H")->Value),
						  xNode->Attributes->GetNamedItem("AddressName")->Value,
						  Convert::ToInt32(xNode->Attributes->GetNamedItem("Port")->Value),
						  Convert::ToInt32(xNode->Attributes->GetNamedItem("TimeInterval")->Value));
			 }
		 }

public:	System::Void SetRadar(int nNumber, bool bState, String^ sName, int nX0, int nY0, int nH0, String^ addressName, int nPort, int nInterval)
		{
			dataGridView3->Rows[nNumber]->Cells[0]->Value = bState;
			//if(bState) {EnabledRow(nNumber);} else {DisabledRow(nNumber);};
			dataGridView3->Rows[nNumber]->Cells[1]->Value = sName;
			dataGridView3->Rows[nNumber]->Cells[2]->Value = nX0;
			dataGridView3->Rows[nNumber]->Cells[3]->Value = nY0;
			dataGridView3->Rows[nNumber]->Cells[4]->Value = nH0;
			dataGridView3->Rows[nNumber]->Cells[5]->Value = addressName;
			dataGridView3->Rows[nNumber]->Cells[6]->Value = nPort;
			dataGridView3->Rows[nNumber]->Cells[7]->Value = nInterval;
		}

private: System::Void EnabledRow(int i)
		 {
			 for (int k = 1; k!=8; k++)
			 {
				 dataGridView3->Rows[i]->Cells[k]->ReadOnly=false;
				 dataGridView3->Rows[i]->Cells[k]->Style->BackColor=System::Drawing::Color::White;
			 }
			 dataGridView3->Rows[i]->Cells[6]->ReadOnly=true;
		 }
private: System::Void DisabledRow(int i)
		 {
			 for (int k = 1; k!=8; k++)
			 {
				 dataGridView3->Rows[i]->Cells[k]->ReadOnly=true;
				 dataGridView3->Rows[i]->Cells[k]->Style->BackColor=System::Drawing::Color::Gray;
			 }
		 }

private: System::Void dataGridView3_CurrentCellDirtyStateChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 dataGridView3->CommitEdit(DataGridViewDataErrorContexts::Commit);
		 }

private: System::Void dataGridView3_CellValueChanged(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e)
		 {
			 if(bCreated)
			 {
				 if(e->ColumnIndex==0)
				 {
					 if(Convert::ToBoolean(dataGridView3->Rows[e->RowIndex]->Cells[0]->Value)==true)
					 {
						 EnabledRow(e->RowIndex);
					 }
					 else
					 {
						 DisabledRow(e->RowIndex);
					 }
				 }
			 }
		 }
};

}
//e->Cancel = true;
			 //this->Hide();