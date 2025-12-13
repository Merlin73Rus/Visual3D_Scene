#pragma once

#include "AviationSpatialTraining.h"

namespace AviationSpatialTraining {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			InitAviationTraining();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		// Aviation training components
		CAviationSpatialTraining* pAviationTraining;
		System::Windows::Forms::Panel^  renderPanel;
		System::Windows::Forms::Timer^  timerUpdate;
		System::Windows::Forms::GroupBox^  grpTrainingControls;
		System::Windows::Forms::Button^  btnResetFlight;
		System::Windows::Forms::Button^  btnStartTraining;
		System::Windows::Forms::ComboBox^  cmbTrainingStage;
		System::Windows::Forms::Label^  lblCurrentStage;
		System::Windows::Forms::Label^  lblAircraftStatus;
		System::Windows::Forms::GroupBox^  grpFlightParameters;
		System::Windows::Forms::Label^  lblAltitude;
		System::Windows::Forms::Label^  lblAirspeed;
		System::Windows::Forms::Label^  lblHeading;
		System::Windows::Forms::TrackBar^  trkWeather;
		System::Windows::Forms::Label^  lblWeather;
		System::Windows::Forms::TrackBar^  trkTimeOfDay;
		System::Windows::Forms::Label^  lblTimeOfDay;
		System::Windows::Forms::Button^  btnToggleInstruments;
		System::ComponentModel::IContainer^  components;

		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->renderPanel = (gcnew System::Windows::Forms::Panel());
			this->grpTrainingControls = (gcnew System::Windows::Forms::GroupBox());
			this->btnToggleInstruments = (gcnew System::Windows::Forms::Button());
			this->trkTimeOfDay = (gcnew System::Windows::Forms::TrackBar());
			this->lblTimeOfDay = (gcnew System::Windows::Forms::Label());
			this->trkWeather = (gcnew System::Windows::Forms::TrackBar());
			this->lblWeather = (gcnew System::Windows::Forms::Label());
			this->cmbTrainingStage = (gcnew System::Windows::Forms::ComboBox());
			this->btnResetFlight = (gcnew System::Windows::Forms::Button());
			this->btnStartTraining = (gcnew System::Windows::Forms::Button());
			this->lblCurrentStage = (gcnew System::Windows::Forms::Label());
			this->grpFlightParameters = (gcnew System::Windows::Forms::GroupBox());
			this->lblHeading = (gcnew System::Windows::Forms::Label());
			this->lblAirspeed = (gcnew System::Windows::Forms::Label());
			this->lblAltitude = (gcnew System::Windows::Forms::Label());
			this->lblAircraftStatus = (gcnew System::Windows::Forms::Label());
			this->timerUpdate = (gcnew System::Windows::Forms::Timer(this->components));
			this->grpTrainingControls->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trkTimeOfDay))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trkWeather))->BeginInit();
			this->grpFlightParameters->SuspendLayout();
			this->SuspendLayout();
			// 
			// renderPanel
			// 
			this->renderPanel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->renderPanel->BackColor = System::Drawing::SystemColors::ControlDark;
			this->renderPanel->Location = System::Drawing::Point(12, 12);
			this->renderPanel->Name = L"renderPanel";
			this->renderPanel->Size = System::Drawing::Size(1044, 551);
			this->renderPanel->TabIndex = 0;
			this->renderPanel->Resize += gcnew System::EventHandler(this, &MainForm::renderPanel_Resize);
			// 
			// grpTrainingControls
			// 
			this->grpTrainingControls->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->grpTrainingControls->Controls->Add(this->btnToggleInstruments);
			this->grpTrainingControls->Controls->Add(this->trkTimeOfDay);
			this->grpTrainingControls->Controls->Add(this->lblTimeOfDay);
			this->grpTrainingControls->Controls->Add(this->trkWeather);
			this->grpTrainingControls->Controls->Add(this->lblWeather);
			this->grpTrainingControls->Controls->Add(this->cmbTrainingStage);
			this->grpTrainingControls->Controls->Add(this->btnResetFlight);
			this->grpTrainingControls->Controls->Add(this->btnStartTraining);
			this->grpTrainingControls->Controls->Add(this->lblCurrentStage);
			this->grpTrainingControls->Location = System::Drawing::Point(1062, 12);
			this->grpTrainingControls->Name = L"grpTrainingControls";
			this->grpTrainingControls->Size = System::Drawing::Size(294, 278);
			this->grpTrainingControls->TabIndex = 1;
			this->grpTrainingControls->TabStop = false;
			this->grpTrainingControls->Text = L"Training Controls";
			// 
			// btnToggleInstruments
			// 
			this->btnToggleInstruments->Location = System::Drawing::Point(16, 235);
			this->btnToggleInstruments->Name = L"btnToggleInstruments";
			this->btnToggleInstruments->Size = System::Drawing::Size(120, 23);
			this->btnToggleInstruments->TabIndex = 8;
			this->btnToggleInstruments->Text = L"Toggle Instruments";
			this->btnToggleInstruments->UseVisualStyleBackColor = true;
			this->btnToggleInstruments->Click += gcnew System::EventHandler(this, &MainForm::btnToggleInstruments_Click);
			// 
			// trkTimeOfDay
			// 
			this->trkTimeOfDay->LargeChange = 3;
			this->trkTimeOfDay->Location = System::Drawing::Point(97, 188);
			this->trkTimeOfDay->Maximum = 23;
			this->trkTimeOfDay->Name = L"trkTimeOfDay";
			this->trkTimeOfDay->Size = System::Drawing::Size(181, 45);
			this->trkTimeOfDay->TabIndex = 7;
			this->trkTimeOfDay->Value = 12;
			this->trkTimeOfDay->Scroll += gcnew System::EventHandler(this, &MainForm::trkTimeOfDay_Scroll);
			// 
			// lblTimeOfDay
			// 
			this->lblTimeOfDay->AutoSize = true;
			this->lblTimeOfDay->Location = System::Drawing::Point(13, 188);
			this->lblTimeOfDay->Name = L"lblTimeOfDay";
			this->lblTimeOfDay->Size = System::Drawing::Size(78, 13);
			this->lblTimeOfDay->TabIndex = 6;
			this->lblTimeOfDay->Text = L"Time of Day: 12";
			// 
			// trkWeather
			// 
			this->trkWeather->LargeChange = 1;
			this->trkWeather->Location = System::Drawing::Point(97, 142);
			this->trkWeather->Maximum = 2;
			this->trkWeather->Name = L"trkWeather";
			this->trkWeather->Size = System::Drawing::Size(181, 45);
			this->trkWeather->TabIndex = 5;
			this->trkWeather->Scroll += gcnew System::EventHandler(this, &MainForm::trkWeather_Scroll);
			// 
			// lblWeather
			// 
			this->lblWeather->AutoSize = true;
			this->lblWeather->Location = System::Drawing::Point(13, 142);
			this->lblWeather->Name = L"lblWeather";
			this->lblWeather->Size = System::Drawing::Size(55, 13);
			this->lblWeather->TabIndex = 4;
			this->lblWeather->Text = L"Weather: ";
			// 
			// cmbTrainingStage
			// 
			this->cmbTrainingStage->FormattingEnabled = true;
			this->cmbTrainingStage->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"Stage 1: Aircraft Familiarization", L"Stage 2: Basic Maneuvers", L"Stage 3: Visual Flight Rules", L"Stage 4: Transition to Instruments", L"Stage 5: Instrument Flight Only"});
			this->cmbTrainingStage->Location = System::Drawing::Point(16, 65);
			this->cmbTrainingStage->Name = L"cmbTrainingStage";
			this->cmbTrainingStage->Size = System::Drawing::Size(262, 21);
			this->cmbTrainingStage->TabIndex = 3;
			this->cmbTrainingStage->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::cmbTrainingStage_SelectedIndexChanged);
			// 
			// btnResetFlight
			// 
			this->btnResetFlight->Location = System::Drawing::Point(16, 108);
			this->btnResetFlight->Name = L"btnResetFlight";
			this->btnResetFlight->Size = System::Drawing::Size(120, 23);
			this->btnResetFlight->TabIndex = 2;
			this->btnResetFlight->Text = L"Reset Flight";
			this->btnResetFlight->UseVisualStyleBackColor = true;
			this->btnResetFlight->Click += gcnew System::EventHandler(this, &MainForm::btnResetFlight_Click);
			// 
			// btnStartTraining
			// 
			this->btnStartTraining->Location = System::Drawing::Point(16, 206);
			this->btnStartTraining->Name = L"btnStartTraining";
			this->btnStartTraining->Size = System::Drawing::Size(120, 23);
			this->btnStartTraining->TabIndex = 1;
			this->btnStartTraining->Text = L"Start Training";
			this->btnStartTraining->UseVisualStyleBackColor = true;
			this->btnStartTraining->Click += gcnew System::EventHandler(this, &MainForm::btnStartTraining_Click);
			// 
			// lblCurrentStage
			// 
			this->lblCurrentStage->AutoSize = true;
			this->lblCurrentStage->Location = System::Drawing::Point(13, 49);
			this->lblCurrentStage->Name = L"lblCurrentStage";
			this->lblCurrentStage->Size = System::Drawing::Size(81, 13);
			this->lblCurrentStage->TabIndex = 0;
			this->lblCurrentStage->Text = L"Current Stage: 1";
			// 
			// grpFlightParameters
			// 
			this->grpFlightParameters->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->grpFlightParameters->Controls->Add(this->lblHeading);
			this->grpFlightParameters->Controls->Add(this->lblAirspeed);
			this->grpFlightParameters->Controls->Add(this->lblAltitude);
			this->grpFlightParameters->Location = System::Drawing::Point(1062, 306);
			this->grpFlightParameters->Name = L"grpFlightParameters";
			this->grpFlightParameters->Size = System::Drawing::Size(294, 100);
			this->grpFlightParameters->TabIndex = 2;
			this->grpFlightParameters->TabStop = false;
			this->grpFlightParameters->Text = L"Flight Parameters";
			// 
			// lblHeading
			// 
			this->lblHeading->AutoSize = true;
			this->lblHeading->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->lblHeading->Location = System::Drawing::Point(13, 69);
			this->lblHeading->Name = L"lblHeading";
			this->lblHeading->Size = System::Drawing::Size(71, 16);
			this->lblHeading->TabIndex = 2;
			this->lblHeading->Text = L"Heading: 0°";
			// 
			// lblAirspeed
			// 
			this->lblAirspeed->AutoSize = true;
			this->lblAirspeed->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->lblAirspeed->Location = System::Drawing::Point(13, 46);
			this->lblAirspeed->Name = L"lblAirspeed";
			this->lblAirspeed->Size = System::Drawing::Size(92, 16);
			this->lblAirspeed->TabIndex = 1;
			this->lblAirspeed->Text = L"Airspeed: 0 km/h";
			// 
			// lblAltitude
			// 
			this->lblAltitude->AutoSize = true;
			this->lblAltitude->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->lblAltitude->Location = System::Drawing::Point(13, 23);
			this->lblAltitude->Name = L"lblAltitude";
			this->lblAltitude->Size = System::Drawing::Size(86, 16);
			this->lblAltitude->TabIndex = 0;
			this->lblAltitude->Text = L"Altitude: 0 m";
			// 
			// lblAircraftStatus
			// 
			this->lblAircraftStatus->AutoSize = true;
			this->lblAircraftStatus->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->lblAircraftStatus->ForeColor = System::Drawing::Color::ForestGreen;
			this->lblAircraftStatus->Location = System::Drawing::Point(12, 685);
			this->lblAircraftStatus->Name = L"lblAircraftStatus";
			this->lblAircraftStatus->Size = System::Drawing::Size(175, 24);
			this->lblAircraftStatus->TabIndex = 3;
			this->lblAircraftStatus->Text = L"Aircraft Status: OK";
			// 
			// timerUpdate
			// 
			this->timerUpdate->Interval = 50;
			this->timerUpdate->Tick += gcnew System::EventHandler(this, &MainForm::timerUpdate_Tick);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1368, 721);
			this->Controls->Add(this->lblAircraftStatus);
			this->Controls->Add(this->grpFlightParameters);
			this->Controls->Add(this->grpTrainingControls);
			this->Controls->Add(this->renderPanel);
			this->Name = L"MainForm";
			this->Text = L"Программа для формирования навыков пространственной ориентации у студентов гражданской авиации";
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->SizeChanged += gcnew System::EventHandler(this, &MainForm::MainForm_SizeChanged);
			this->grpTrainingControls->ResumeLayout(false);
			this->grpTrainingControls->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trkTimeOfDay))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trkWeather))->EndInit();
			this->grpFlightParameters->ResumeLayout(false);
			this->grpFlightParameters->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void InitAviationTraining(void)
	{
		pAviationTraining = new CAviationSpatialTraining();
		pAviationTraining->SetHandle(static_cast<long>(renderPanel->Handle.ToPointer()));
		
		// Initialize combo box with first item selected
		cmbTrainingStage->SelectedIndex = 0;
		
		// Start the update timer
		timerUpdate->Enabled = true;
	}

	private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e) 
	{
		// Initialize the 3D scene
		pAviationTraining->StartRendering();
	}

	private: System::Void renderPanel_Resize(System::Object^  sender, System::EventArgs^  e) 
	{
		if (pAviationTraining) {
			pAviationTraining->Resize(renderPanel->Width, renderPanel->Height);
		}
	}

	private: System::Void btnStartTraining_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		if (pAviationTraining) {
			lblAircraftStatus->Text = "Training Started...";
			lblAircraftStatus->ForeColor = System::Drawing::Color::Blue;
			
			// Start rendering if not already started
			pAviationTraining->StartRendering();
		}
	}

	private: System::Void btnResetFlight_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		if (pAviationTraining) {
			pAviationTraining->ResetFlight();
			lblAircraftStatus->Text = "Flight Reset";
			lblAircraftStatus->ForeColor = System::Drawing::Color::Orange;
		}
	}

	private: System::Void cmbTrainingStage_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		if (pAviationTraining) {
			int stage = cmbTrainingStage->SelectedIndex + 1;
			pAviationTraining->SetCurrentStage(stage);
			
			// Update label
			lblCurrentStage->Text = "Current Stage: " + stage.ToString() + 
				" - " + cmbTrainingStage->SelectedItem->ToString();
			
			lblAircraftStatus->Text = "Stage changed to: " + stage.ToString();
			lblAircraftStatus->ForeColor = System::Drawing::Color::Blue;
		}
	}

	private: System::Void timerUpdate_Tick(System::Object^  sender, System::EventArgs^  e) 
	{
		if (pAviationTraining) {
			// Update flight parameters display
			double altitude, airspeed, heading, vspeed;
			pAviationTraining->GetFlightInstruments(altitude, airspeed, heading, vspeed);
			
			lblAltitude->Text = "Altitude: " + ((int)altitude).ToString() + " m";
			lblAirspeed->Text = "Airspeed: " + ((int)airspeed).ToString() + " km/h";
			lblHeading->Text = "Heading: " + ((int)heading).ToString() + "°";
			
			// Update status if needed
			lblAircraftStatus->Text = "Training in progress...";
			lblAircraftStatus->ForeColor = System::Drawing::Color::ForestGreen;
		}
	}

	private: System::Void trkWeather_Scroll(System::Object^  sender, System::EventArgs^  e) 
	{
		if (pAviationTraining) {
			int weatherLevel = trkWeather->Value;
			String^ weatherText = "";
			
			switch(weatherLevel) {
				case 0: weatherText = "Clear"; break;
				case 1: weatherText = "Cloudy"; break;
				case 2: weatherText = "Foggy"; break;
			}
			
			lblWeather->Text = "Weather: " + weatherText;
			pAviationTraining->SetWeatherCondition(weatherLevel);
		}
	}

	private: System::Void trkTimeOfDay_Scroll(System::Object^  sender, System::EventArgs^  e) 
	{
		if (pAviationTraining) {
			int hour = trkTimeOfDay->Value;
			lblTimeOfDay->Text = "Time of Day: " + hour.ToString();
			pAviationTraining->SetTimeOfDay(hour);
		}
	}

	private: System::Void btnToggleInstruments_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		if (pAviationTraining) {
			// Toggle instruments display
			// Implementation would depend on specific needs
			lblAircraftStatus->Text = "Instruments toggled";
			lblAircraftStatus->ForeColor = System::Drawing::Color::Blue;
		}
	}

	private: System::Void MainForm_SizeChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		if (pAviationTraining) {
			pAviationTraining->Resize(renderPanel->Width, renderPanel->Height);
		}
	}
};
}