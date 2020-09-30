#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Cosmos {

	ref class Grid;
	/// <summary>
	/// Summary for WizCtrl
	/// </summary>
	public ref class WizCtrl : public System::Windows::Forms::UserControl
	{
	public:
		WizCtrl(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		delegate void WizFinish();
		event WizFinish^ OnWizFinish;
		void Fire_OnWizFinish()
		{
			OnWizFinish();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~WizCtrl()
		{
			if (components)
			{
				delete components;
			}
		}

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
			this->SuspendLayout();
			// 
			// WizCtrl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Name = L"WizCtrl";
			this->Load += gcnew System::EventHandler(this, &WizCtrl::WizCtrl_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
		private: System::Void WizCtrl_Load(System::Object^ sender, System::EventArgs^ e);
	};
}
