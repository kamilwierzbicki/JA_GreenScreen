#pragma once

#include<memory>
#include"GreenScreenRemove.h"
#include<thread>
#define UINT32TORGBA(source, dest) ((*(&(dest))) = *((RGBA*)(&(source))))

RGBA convertUint32ToRGBA(const uint32_t source) {
	RGBA ret;
	ret.alpha = (uint8_t)((source >> 24)& 0xff);
	ret.red = (uint8_t)((source >> 16) & 0xff);
	ret.green = (uint8_t)((source >> 8) & 0xff);
	ret.blue = (uint8_t)((source >> 0) & 0xff);
	return ret;
}
uint32_t convertRGBATouint32(const RGBA source) {
	uint32_t ret;
	ret = ((uint32_t)source.alpha) << 24;
	ret |= ((uint32_t)source.red) << 16;
	ret |= ((uint32_t)source.green) << 8;
	ret |= ((uint32_t)source.blue) << 0;

	return ret;
}

namespace JAKamilWierzbickiGreenScreenRemoval {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Podsumowanie informacji o Main
	/// </summary>
	public ref class Main : public System::Windows::Forms::Form
	{
		Bitmap^ bmp;
		RGBA** img_raw;
		RGBA** img_withoutGreen;
	private: System::Windows::Forms::TrackBar^ threads_trackBar;

	private: System::Windows::Forms::RadioButton^ ASM_button;
	private: System::Windows::Forms::RadioButton^ cpp_button;

	private: System::Windows::Forms::Button^ start_button;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ threads_label;
	private: System::Windows::Forms::TrackBar^ trackBar1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ minDif_label;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ time_label;
	private: System::Windows::Forms::Button^ save_button;
	private: System::Windows::Forms::SaveFileDialog^ saveFileDialog1;




		   Bitmap^ removed;
		
	public:
		Main(void)
		{
			InitializeComponent();
			threads_trackBar->SetRange(1, 64);
			threads_trackBar->Value = std::thread::hardware_concurrency();
			threads_label->Text = threads_trackBar->Value.ToString();

			//
			//TODO: W tym miejscu dodaj kod konstruktora
			//
		}

	protected:
		/// <summary>
		/// Wyczyœæ wszystkie u¿ywane zasoby.
		/// </summary>
		~Main()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::Button^ imageOpenButton;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::PictureBox^ pictureBox2;

	protected:


	private:
		/// <summary>
		/// Wymagana zmienna projektanta.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metoda wymagana do obs³ugi projektanta — nie nale¿y modyfikowaæ
		/// jej zawartoœci w edytorze kodu.
		/// </summary>
		void InitializeComponent(void)
		{
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->imageOpenButton = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->threads_trackBar = (gcnew System::Windows::Forms::TrackBar());
			this->ASM_button = (gcnew System::Windows::Forms::RadioButton());
			this->cpp_button = (gcnew System::Windows::Forms::RadioButton());
			this->start_button = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->threads_label = (gcnew System::Windows::Forms::Label());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->minDif_label = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->time_label = (gcnew System::Windows::Forms::Label());
			this->save_button = (gcnew System::Windows::Forms::Button());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->threads_trackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			this->SuspendLayout();
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// imageOpenButton
			// 
			this->imageOpenButton->Cursor = System::Windows::Forms::Cursors::Arrow;
			this->imageOpenButton->Location = System::Drawing::Point(75, 633);
			this->imageOpenButton->Margin = System::Windows::Forms::Padding(4);
			this->imageOpenButton->Name = L"imageOpenButton";
			this->imageOpenButton->Size = System::Drawing::Size(100, 28);
			this->imageOpenButton->TabIndex = 0;
			this->imageOpenButton->Text = L"Otwórz";
			this->imageOpenButton->UseVisualStyleBackColor = true;
			this->imageOpenButton->Click += gcnew System::EventHandler(this, &Main::button1_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(75, 15);
			this->pictureBox1->Margin = System::Windows::Forms::Padding(4);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(589, 503);
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			// 
			// pictureBox2
			// 
			this->pictureBox2->Location = System::Drawing::Point(672, 15);
			this->pictureBox2->Margin = System::Windows::Forms::Padding(4);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(615, 503);
			this->pictureBox2->TabIndex = 2;
			this->pictureBox2->TabStop = false;
			// 
			// threads_trackBar
			// 
			this->threads_trackBar->Location = System::Drawing::Point(449, 626);
			this->threads_trackBar->Margin = System::Windows::Forms::Padding(4);
			this->threads_trackBar->Name = L"threads_trackBar";
			this->threads_trackBar->Size = System::Drawing::Size(361, 56);
			this->threads_trackBar->TabIndex = 3;
			this->threads_trackBar->Scroll += gcnew System::EventHandler(this, &Main::trackBar1_Scroll);
			// 
			// ASM_button
			// 
			this->ASM_button->AutoSize = true;
			this->ASM_button->Location = System::Drawing::Point(1156, 633);
			this->ASM_button->Margin = System::Windows::Forms::Padding(4);
			this->ASM_button->Name = L"ASM_button";
			this->ASM_button->Size = System::Drawing::Size(69, 21);
			this->ASM_button->TabIndex = 4;
			this->ASM_button->Text = L"MASM";
			this->ASM_button->UseVisualStyleBackColor = true;
			// 
			// cpp_button
			// 
			this->cpp_button->AutoSize = true;
			this->cpp_button->Checked = true;
			this->cpp_button->Location = System::Drawing::Point(1156, 661);
			this->cpp_button->Margin = System::Windows::Forms::Padding(4);
			this->cpp_button->Name = L"cpp_button";
			this->cpp_button->Size = System::Drawing::Size(54, 21);
			this->cpp_button->TabIndex = 5;
			this->cpp_button->TabStop = true;
			this->cpp_button->Text = L"C++";
			this->cpp_button->UseVisualStyleBackColor = true;
			// 
			// start_button
			// 
			this->start_button->Location = System::Drawing::Point(515, 743);
			this->start_button->Margin = System::Windows::Forms::Padding(4);
			this->start_button->Name = L"start_button";
			this->start_button->Size = System::Drawing::Size(100, 28);
			this->start_button->TabIndex = 7;
			this->start_button->Text = L"START";
			this->start_button->UseVisualStyleBackColor = true;
			this->start_button->Click += gcnew System::EventHandler(this, &Main::start_button_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(449, 586);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(47, 17);
			this->label1->TabIndex = 8;
			this->label1->Text = L"W¹tki:";
			// 
			// threads_label
			// 
			this->threads_label->AutoSize = true;
			this->threads_label->Location = System::Drawing::Point(511, 586);
			this->threads_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->threads_label->Name = L"threads_label";
			this->threads_label->Size = System::Drawing::Size(16, 17);
			this->threads_label->TabIndex = 9;
			this->threads_label->Text = L"0";
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(449, 689);
			this->trackBar1->Maximum = 100;
			this->trackBar1->Minimum = 1;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(361, 56);
			this->trackBar1->TabIndex = 10;
			this->trackBar1->Value = 1;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &Main::trackBar1_Scroll_1);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(450, 663);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(125, 17);
			this->label2->TabIndex = 11;
			this->label2->Text = L"Minimalna ró¿nica:";
			// 
			// minDif_label
			// 
			this->minDif_label->AutoSize = true;
			this->minDif_label->Location = System::Drawing::Point(569, 663);
			this->minDif_label->Name = L"minDif_label";
			this->minDif_label->Size = System::Drawing::Size(16, 17);
			this->minDif_label->TabIndex = 12;
			this->minDif_label->Text = L"0";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(912, 663);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(43, 17);
			this->label3->TabIndex = 13;
			this->label3->Text = L"Czas:";
			// 
			// time_label
			// 
			this->time_label->AutoSize = true;
			this->time_label->Location = System::Drawing::Point(962, 664);
			this->time_label->Name = L"time_label";
			this->time_label->Size = System::Drawing::Size(16, 17);
			this->time_label->TabIndex = 14;
			this->time_label->Text = L"0";
			// 
			// save_button
			// 
			this->save_button->Location = System::Drawing::Point(1196, 776);
			this->save_button->Name = L"save_button";
			this->save_button->Size = System::Drawing::Size(75, 23);
			this->save_button->TabIndex = 15;
			this->save_button->Text = L"Zapisz";
			this->save_button->UseVisualStyleBackColor = true;
			this->save_button->Click += gcnew System::EventHandler(this, &Main::save_button_Click);
			// 
			// Main
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1608, 849);
			this->Controls->Add(this->save_button);
			this->Controls->Add(this->time_label);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->minDif_label);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->trackBar1);
			this->Controls->Add(this->threads_label);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->start_button);
			this->Controls->Add(this->cpp_button);
			this->Controls->Add(this->ASM_button);
			this->Controls->Add(this->threads_trackBar);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->imageOpenButton);
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"Main";
			this->Text = L"Main";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->threads_trackBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {

		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			auto stream = openFileDialog1->OpenFile();
			bmp = gcnew Bitmap(stream);
			pictureBox1->SizeMode = PictureBoxSizeMode::Zoom;
			pictureBox1->Image = bmp;
			

			img_raw = new RGBA * [bmp->Height];
			for (int i = 0; i < bmp->Height; i++) {
				img_raw[i] = new RGBA[bmp->Width];
			}
			img_withoutGreen = new RGBA * [bmp->Height];
			for (int i = 0; i < bmp->Height; i++) {
				img_withoutGreen[i] = new RGBA[bmp->Width];
			}
			for (int y = 0; y < bmp->Height; y++) {
				for (int x = 0; x < bmp->Width; x++) {
					uint32_t argb_int = bmp->GetPixel(x, y).ToArgb();
					img_raw[y][x] = convertUint32ToRGBA(argb_int);
					img_withoutGreen[y][x] = convertUint32ToRGBA(argb_int);
				}
			}
			removed = gcnew Bitmap(bmp->Width, bmp->Height);
			pictureBox2->SizeMode = PictureBoxSizeMode::Zoom;
			pictureBox2->Image = removed;


		}
	}

	private: System::Void trackBar1_Scroll(System::Object^ sender, System::EventArgs^ e) {
		threads_label->Text = threads_trackBar->Value.ToString();
		/*
		uint32_t val = threads_trackBar->Value;
		BitmapImageWithAlpha bmpImage;
		bmpImage.height = bmp->Height;
		bmpImage.width = bmp->Width;
		bmpImage.img = img_raw;
		BitmapImageWithAlpha bmpImage_withoutGreen;
		bmpImage_withoutGreen.height = bmp->Height;
		bmpImage_withoutGreen.width = bmp->Width;
		bmpImage_withoutGreen.img = img_withoutGreen;

		System::Threading::ThreadPool::SetMaxThreads(20,0);
		for (int y = 0; y < bmpImage.height; y++) {
			removeGreenScreenArgsStruct^ args = gcnew removeGreenScreenArgsStruct;
			args->dest = bmpImage_withoutGreen.img[y];
			args->source = bmpImage.img[y];
			args->minDif = val;
			args->len = bmpImage.width;
			args->bmp = removed;
			args->column = y;

			//System::Threading::ThreadPool::QueueUserWorkItem(gcnew System::Threading::WaitCallback(GreenScreen::removeGreenScreenRow_delegate), args);
			//removeGreenScreenAssembly(bmpImage.img[y], bmpImage_withoutGreen.img[y], bmpImage.width, 10);
		}
		//removeGreenScreen(bmpImage, bmpImage_withoutGreen, val);
		
		
		for (int y = 0; y < bmp->Height; y++) {
			for (int x = 0; x < bmp->Width; x++) {
				uint32_t argb_int = convertRGBATouint32(bmpImage_withoutGreen.img[y][x]);
				removed->SetPixel(x, y, System::Drawing::Color::FromArgb(argb_int));
			}
		}
		pictureBox2->SizeMode = PictureBoxSizeMode::Zoom;
		pictureBox2->Image = removed;
		*/
	}
private: System::Void start_button_Click(System::Object^ sender, System::EventArgs^ e) {
	
	if (!bmp || !img_raw) {
		MessageBox::Show("Wczytaj zdjêcie");
		return;
	}
	start_button->Enabled = false;
	uint32_t val = threads_trackBar->Value;
	BitmapImageWithAlpha bmpImage;
	bmpImage.height = bmp->Height;
	bmpImage.width = bmp->Width;
	bmpImage.img = img_raw;
	BitmapImageWithAlpha bmpImage_withoutGreen;
	bmpImage_withoutGreen.height = bmp->Height;
	bmpImage_withoutGreen.width = bmp->Width;
	bmpImage_withoutGreen.img = img_withoutGreen;

	System::Threading::ThreadPool::SetMaxThreads(threads_trackBar->Value, 0);
	GreenScreen::currentThreads = bmpImage.height;
	System::Diagnostics::Stopwatch^ stopwatch = gcnew System::Diagnostics::Stopwatch();
	stopwatch->Start();
	/*
	GreenScreen::removeGreenScreenRow(bmpImage.img[50], bmpImage_withoutGreen.img[50], bmpImage.width, 0);

	GreenScreen::removeGreenScreenRow(bmpImage.img[51], bmpImage_withoutGreen.img[51], bmpImage.width, 0);
	GreenScreen::removeGreenScreenRow(bmpImage.img[52], bmpImage_withoutGreen.img[52], bmpImage.width, 0);
	GreenScreen::removeGreenScreenRow(bmpImage.img[53], bmpImage_withoutGreen.img[53], bmpImage.width, 0);
	GreenScreen::removeGreenScreenRow(bmpImage.img[54], bmpImage_withoutGreen.img[54], bmpImage.width, 0);
	GreenScreen::removeGreenScreenRow(bmpImage.img[55], bmpImage_withoutGreen.img[55], bmpImage.width, 0);*/
	/*
	removeGreenScreenAssembly(bmpImage.img[50*5], bmpImage_withoutGreen.img[50*5], bmpImage.width*4, 0);
	removeGreenScreenAssembly(bmpImage.img[51*5], bmpImage_withoutGreen.img[51*5], bmpImage.width*4, 0);
	removeGreenScreenAssembly(bmpImage.img[52*5], bmpImage_withoutGreen.img[52*5], bmpImage.width*4, 0);
	removeGreenScreenAssembly(bmpImage.img[53*5], bmpImage_withoutGreen.img[53*5], bmpImage.width*4, 0);
	removeGreenScreenAssembly(bmpImage.img[54*5], bmpImage_withoutGreen.img[54*5], bmpImage.width*4, 0);
	removeGreenScreenAssembly(bmpImage.img[55*5], bmpImage_withoutGreen.img[55*5], bmpImage.width*4, 0);*/
	for (int y = 0; y < bmpImage.height; y++) {
		removeGreenScreenArgsStruct^ args = gcnew removeGreenScreenArgsStruct;
		args->dest = bmpImage_withoutGreen.img[y];
		args->source = bmpImage.img[y];
		args->minDif = trackBar1->Value;
		args->len = bmpImage.width;
		args->column = 0;
		args->masm = ASM_button->Checked;
		//GreenScreen::removeGreenScreenRow(bmpImage.img[y], bmpImage_withoutGreen.img[y], bmpImage.width, 0);
		System::Threading::ThreadPool::QueueUserWorkItem(gcnew System::Threading::WaitCallback(GreenScreen::removeGreenScreenRow_delegate), args);

		//removeGreenScreenAssembly(bmpImage.img[y], bmpImage_withoutGreen.img[y], bmpImage.width, 0);
		//rmGreenScreen(bmpImage.img[y], bmpImage_withoutGreen.img[y], bmpImage.width, args->minDif);

		
	}
	int a, b;
	
	while (GreenScreen::currentThreads);
	stopwatch->Stop();
	time_label->Text = stopwatch->ElapsedMilliseconds.ToString();
	for (int y = 0; y < bmp->Height; y++) {
		for (int x = 0; x < bmp->Width; x++) {
			uint32_t argb_int = convertRGBATouint32(bmpImage_withoutGreen.img[y][x]);
			removed->SetPixel(x, y, System::Drawing::Color::FromArgb(argb_int));
		}
	}
	pictureBox2->SizeMode = PictureBoxSizeMode::Zoom;
	pictureBox2->Image = removed;
	start_button->Enabled = true;
}
private: System::Void trackBar1_Scroll_1(System::Object^ sender, System::EventArgs^ e) {
	minDif_label->Text = trackBar1->Value.ToString();
}

private: System::Void save_button_Click(System::Object^ sender, System::EventArgs^ e) {
	if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		//auto stream = saveFileDialog1->OpenFile();
		removed->Save(saveFileDialog1->FileName);
	}
}
};
}
