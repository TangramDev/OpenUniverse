#pragma once

namespace Cosmos {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for TangramNewChildForm
	/// </summary>
	public ref class NewChildForm : public System::Windows::Forms::Form
	{
	public:
		NewChildForm(void)
		{
			m_pParent = nullptr;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		System::Windows::Forms::ListView^ GetListView()
		{
			return NewTemplateListView;
		}
		Form^ m_pParent;
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~NewChildForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListView^ NewTemplateListView;
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
			this->NewTemplateListView = (gcnew System::Windows::Forms::ListView());
			this->SuspendLayout();
			// 
			// NewTemplateListView
			// 
			this->NewTemplateListView->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->NewTemplateListView->HideSelection = false;
			this->NewTemplateListView->Location = System::Drawing::Point(39, 52);
			this->NewTemplateListView->MultiSelect = false;
			this->NewTemplateListView->Name = L"NewTemplateListView";
			this->NewTemplateListView->Size = System::Drawing::Size(1085, 706);
			this->NewTemplateListView->TabIndex = 0;
			this->NewTemplateListView->UseCompatibleStateImageBehavior = false;
			this->NewTemplateListView->DoubleClick += gcnew System::EventHandler(this, &NewChildForm::NewTemplateListView_DoubleClick);
			// 
			// TangramNewChildForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(12, 25);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1162, 843);
			this->Controls->Add(this->NewTemplateListView);
			this->Name = L"NewChildForm";
			this->ShowInTaskbar = false;
			this->Text = L"NewChildForm";
			this->TopMost = true;
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void NewTemplateListView_DoubleClick(System::Object^ sender, System::EventArgs^ e);
	};
}
