{
    int n = 50; // ヒストグラムの分割数
    double min = 100; // フィッティングするデータの最小値
    double max = 5000; // フィッティングするデータの最大値
    string filename = "data15pre2.dat"; // データのファイル名
    double a0 = 250.0, b0 = 2000.0, c0 = 0.8,d0=0.005,e0=5.0,f0=-1.5; // フィッティングの初期値

    TH1D* hist = new TH1D("hist", "Larmor's precession", n, min, max);
    ifstream ifs(filename);
    double a, b, c;

    while (ifs >> a >> b >> c) { // 実際のデータではコメントアウトを消す
        if (min < c && c < max) {
            hist->Fill(c);
        }
    }
    // フィット関数を設定
    TF1 *f = new TF1("f", "18.4777*4.12/(1+[0])*exp(-x/1882)*(1.0+[0]*cos([1]*x+[2]))+4.12*2.531", min, max);
    f->SetParameters(/*a0,b0,*/ c0,d0/*,e0*/,f0);

    // パラメータ名を変更
    /*f->SetParName(0, "N");*/
    f->SetParName(0, "B");
    f->SetParName(1, "#omega[rad/ns]");
    f->SetParName(2, "#delta[rad]");
    /*f->SetParName(4, "bkg");*/
    // ヒストグラムをフィット
    hist->Fit(f);

    // 不要なパラメータ表示を非表示にする
    gStyle->SetOptFit(1); // デフォルトは 0111 (すべて表示)
    // フィット情報を表示
    gStyle->SetOptFit(0111); // パラメータ #tau (p1) だけを表示

    // キャンバスを作成してヒストグラムを描画
    TCanvas* c1 = new TCanvas("c1", "Histogram Canvas", 800, 600);
    hist->Draw();
    hist->Draw("same e1");
    hist->GetXaxis()->SetTitle("Time[ns]");
    hist->GetYaxis()->SetTitle("Counts");
    f->Draw("same"); // フィット結果をヒストグラムと重ねて表示

    // PNGファイルとして保存
    c1->SaveAs("fitting1_larmor.png");
    double fai=atan(3.0);
    ofstream ofs("fitting.txt");
    if (ofs.is_open()) {
        // パラメータBの値を書き込む
        double B = f->GetParameter(2);
        ofs << "α: " << 1/(sqrt(M_PI*(1+1/2*((B*fai/sin(fai))*(B*fai/sin(fai)))))) << endl;
        ofs << "β: " << B*fai/(sin(fai)*sqrt(M_PI*(1+1/2*((B*fai/sin(fai))*(B*fai/sin(fai)))))) << endl;
        ofs.close();
    } else {
        cerr << "Unable to open file for writing." << endl;
    }

    // メモリの解放
    delete hist;
    delete f;
    delete c1;
}//規格化されたαとβの値が求まる.Bの値の範囲に0が含まれていなければ,確かにパリティ対称性の破れ自体は示されている?
